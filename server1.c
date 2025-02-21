#include "sock_head.h"
#include "http_parser.h"
#define MAX_SIZE 16192

sock_creds * socket_cred;
llhttp_ps * ps; 
void interrupt_handler(){
    printf("\nInterrupt detected.Exiting...\n");
    free(socket_cred);
    free(ps);
    exit(0);
}


int main(){

  /* makes the socket*/ 
  socket_cred = sock_cred_init();
  set_port(&socket_cred , "8080");
  socket_init(&socket_cred);

 
  /* initializes the parser */
  ps = llhttp_ps_init();
  http_parser_init(ps);
  

  FILE * fptr;  
  printf(" waiting for client : " );
  signal(SIGINT , interrupt_handler);  
  while(1){
      struct sockaddr_storage client_address;
      socklen_t client_len = sizeof( client_address );
      int sock_client = accept( socket_cred->socket_id , (struct sockaddr*) &client_address , &client_len);
      
      /* socket accepts */
      /* TODO add epoll*/
      printf("client is connected \n" );
      fptr = fopen("index.html" , "r");
      char request[MAX_SIZE];
      int bytes_rec = recv(sock_client , request , MAX_SIZE , 0);
      printf("Recieved %d bytes. %s\n" , bytes_rec , request );
      request[bytes_rec] = '\0';
      
      //char * test =  "GET /path HTTP/1.1\r\nHost: example.com\r\n\r\n";
      /* parsing here */ 
      
      parser_execute(&(ps->parser) ,request);
      
      llhttp_reset(&(ps->parser));
      printf("sending response: \n");
      char html_file[10294];
      fread(html_file , 1 , sizeof(html_file)-1 , fptr);

      const char* response = "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/html\r\n\r\n";
    

      int bytes_sent = send(sock_client , response , strlen(response) , 0);
      send(sock_client , html_file , strlen(html_file) , 0);
      close(sock_client);
    }

return 0;
}
