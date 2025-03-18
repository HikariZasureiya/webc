/* TODO 
.check how to send static files
.HTTP
*/

#include "headers/sock_head.h"
#include "headers/http_parser.h"
#include <sys/epoll.h>
#define MAX_SIZE 16192
#include<fcntl.h>
#include<errno.h>
#include<sys/sendfile.h>
#include"headers/url_register.h"
#include "headers/url_dist.h"
#define MAX_EVENTS 100

struct epoll_event ev, events[MAX_EVENTS];
path * route_node = NULL;
int epollfd, nfds, event_count = 0;
sock_creds *socket_cred;
llhttp_ps *ps;


void sigpipe_handler(int signum) {
    printf("Caught SIGPIPE!\n");
}


void interrupt_handler() {
    printf("\nInterrupt detected. Exiting...\n");
    close(socket_cred->socket_id);
    free(socket_cred);
    free(ps);
    close(epollfd);
    exit(0);
}

void set_nonblocking(int sock) {
    int flag = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flag | O_NONBLOCK);
}

int send_file_to_client(char * filename , int client_sock ){
    int fptr = open(filename, O_RDONLY);
    if (!fptr) {
        perror("Failed to open file\n");
        return 1;
        }

        if (fcntl(client_sock, F_GETFD) == -1 && errno == EBADF) {
            printf("Skipping send: Socket %d is closed.\n", client_sock);
            } else {
                const char *response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Connection: close\r\n\r\n";

                send(client_sock, response, strlen(response), 0);
                sendfile(client_sock , fptr , NULL , MAX_SIZE);
                epoll_ctl(epollfd, EPOLL_CTL_DEL, client_sock, NULL);
                close(client_sock);
        }
       close(fptr); 
    return 0;
}

int main() {
    register_routes();
    route_dfs(route_node);
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll failed");
        exit(1);
    }

    socket_cred = sock_cred_init();
    set_port(&socket_cred, "8080");
    socket_init(&socket_cred);

    ev.events = EPOLLIN;
    ev.data.fd = socket_cred->socket_id;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_cred->socket_id, &ev) == -1) {
        perror("epoll_ctl failed");
        close(socket_cred->socket_id);
        free(socket_cred);
        close(epollfd);
        exit(1);
    }

    ps = llhttp_ps_init();
    http_parser_init(ps);

    printf("Waiting for clients...\n");
    signal(SIGPIPE, sigpipe_handler);
    signal(SIGINT, interrupt_handler);

    while (1) {
        event_count = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if (event_count == -1) {
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == socket_cred->socket_id) {
                struct sockaddr_storage client_address;
                socklen_t client_len = sizeof(client_address);
                // Accept new client
                int sock_client = accept(socket_cred->socket_id, (struct sockaddr*)&client_address, &client_len);
                if (sock_client == -1) {
                    perror("accept failed");
                    continue;
                }

                set_nonblocking(sock_client);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = sock_client;

                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_client, &ev) == -1) {
                    perror("epoll_ctl add client failed");
                    close(sock_client);
                }
                printf("New client connected: %d\n", sock_client);
            } else {
                int client_sock = events[i].data.fd;
                printf("Client %d is connected\n", client_sock);

                char request[MAX_SIZE] = {0};
                int tot_size = 0;
                while (tot_size < MAX_SIZE) {
                    int bytes_rec = recv(client_sock, request + tot_size, MAX_SIZE - tot_size, 0);
                    if (bytes_rec == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            printf("Buffer full, waiting...\n");
                            break;
                        } else {
                            perror("recv failed");
                            break;
                        }
                    }
                    if (bytes_rec == 0) {
                        printf("\n\nClient %d disconnected...\n", client_sock);
                        epoll_ctl(epollfd, EPOLL_CTL_DEL, client_sock, NULL);
                        close(client_sock);
                        break;
                    }
                    tot_size += bytes_rec;
                }
                request[tot_size] = '\0';


                parsed_creds new_creds = parser_execute(&(ps->parser), request);
                llhttp_reset(&(ps->parser));
        
                char filename[MAX_SIZE];
                snprintf(filename, MAX_SIZE, "%s", find_route(new_creds.url));
                if(strcmp(filename , "") == 0){
                    printf("file not found\n");
                    int err = send_file_to_client("templates/err.html" , client_sock);
                    if(err){
                        close(client_sock);
                        continue;
                    }              
                   
                }
                    
                else{
                    printf("filename : %s\n" , filename);
                    int err = send_file_to_client(filename , client_sock);
                    if(err){
                        close(client_sock);
                        continue;
                    }                   
                }
            }
        }
    }
    interrupt_handler();
    return 0;
}
