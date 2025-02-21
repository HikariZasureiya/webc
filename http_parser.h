#include <llhttp.h>

typedef struct{
  llhttp_t parser;
  llhttp_settings_t settings;
} llhttp_ps;


llhttp_ps* llhttp_ps_init(){
      llhttp_ps* newps = (llhttp_ps*)malloc(sizeof(llhttp_ps));
      if (newps == NULL){
        printf("ERROR: cannot allocate memory for parser settings. \n");  
        exit(1);
      }
      return newps;
}



int something( llhttp_t* parser){
  fprintf(stdout , "Message complete\n");
  return  0;
}

int something1( llhttp_t* parser){
  fprintf(stdout , "method parsed\n");
  printf("idk man: %s\n" ,  llhttp_method_name(parser->method));
  return  0;
}

int on_url(llhttp_t* parser , const char *at , size_t length){
  printf("url: %.*s\n", (int)length , at);
  return 0;
}



int http_parser_init( llhttp_ps *http_ps ){
  llhttp_settings_init(&(http_ps->settings));
  (http_ps->settings).on_message_complete = something;
  
  (http_ps->settings).on_method_complete = something1;
  
  (http_ps->settings).on_url = on_url;
  

  llhttp_init(&(http_ps->parser) , HTTP_REQUEST , &(http_ps->settings));
}

int parser_execute(llhttp_t *parser , char *request){
    enum llhttp_errno err = llhttp_execute(parser , request , strlen(request));

    if(err == HPE_OK){
      fprintf(stdout , "parsed :confetti: \n");
     
      llhttp_reset(parser);
    }else{
      fprintf(stderr , "parsing error: %s\n" , llhttp_errno_name(err));
      exit(1);
    }
}

