#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/signal.h>



/* struct for the socket */
typedef struct {
    int socket_id;
    char* port;
} sock_creds;

sock_creds *sock_cred_init(){
      sock_creds * newsock = (sock_creds*)malloc(sizeof(sock_creds));
      if (newsock == NULL){
        printf("ERROR: cannot allocate memory for socket. \n");  
        exit(1);
      }
      return newsock;
}

void set_port(sock_creds ** sock , char* port){
    (*sock)->port = port;
}

/* resolves hostname and helps store the ip and port */
void ret_addrinfo(struct addrinfo **bind_address , char* port){
    struct addrinfo hints;
    memset(&hints , 0 , sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
 
    if ( getaddrinfo ( NULL , port , &hints , bind_address ) )
      printf(" getaddrinfo error \n" );
}

/* just makes the socket */
int make_socket(struct addrinfo **bind_address){
  socklen_t addrlen;
  int socket_listen = socket( (*bind_address) -> ai_family , (*bind_address) -> ai_socktype ,   (*bind_address) -> ai_protocol ); 
  
  if( bind( socket_listen , (*bind_address)->ai_addr , (*bind_address)->ai_addrlen )){
      printf("error \n" );
      exit(1);
    }
  
  freeaddrinfo(*bind_address);
  if( listen(socket_listen , 10 ) < 0 ){
      printf(" listen error \n " );
      exit(1);
    }

  return socket_listen;
}

/* function to initialize the socket*/
void socket_init( sock_creds ** sock_cred){
  struct addrinfo *bind_address; 
  ret_addrinfo(&bind_address , (*sock_cred)->port);
  printf(" creating the socket: " ); 
  
  int socket_listen;
  socket_listen = make_socket(&bind_address);
  (*sock_cred)->socket_id = socket_listen;
}


