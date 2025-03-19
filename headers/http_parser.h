#include <llhttp.h>
#define MAX_VALUE 10000
#define METHOD_VAL 20
typedef struct{
  llhttp_t parser;
  llhttp_settings_t settings;
} llhttp_ps;

char urlval[MAX_VALUE];

typedef struct{
  char url[MAX_VALUE];
  char method[METHOD_VAL];
} parsed_creds;

llhttp_ps* llhttp_ps_init(){
      llhttp_ps* newps = (llhttp_ps*)malloc(sizeof(llhttp_ps));
      if (newps == NULL){
        printf("ERROR: cannot allocate memory for parser settings. \n");  
        exit(1);
      }
      return newps;
}

int completion( llhttp_t* parser){
  return  0;
}

int method( llhttp_t* parser){
  // fprintf(stdout , "method parsed\n");
  return  0;
}

int on_url(llhttp_t* parser , const char *at , size_t length){
  printf("url: %.*s\n", (int)length , at);
  if (length >= sizeof(urlval)) {
      length = sizeof(urlval) - 1; 
  }
  strncpy(urlval, at, length);
  urlval[length] = '\0'; 
  return 0;
}



int http_parser_init( llhttp_ps *http_ps ){
  llhttp_settings_init(&(http_ps->settings));
  (http_ps->settings).on_message_complete = completion;
  (http_ps->settings).on_method_complete = method;
  (http_ps->settings).on_url = on_url;
  llhttp_init(&(http_ps->parser) , HTTP_REQUEST , &(http_ps->settings));
  return 0;
}

parsed_creds parser_execute(llhttp_t *parser , char *request){
    enum llhttp_errno err = llhttp_execute(parser , request , strlen(request));
    parsed_creds new_cred;
    if(err == HPE_OK){
      char * this_method = (char *)llhttp_method_name(parser->method);
      snprintf(new_cred.method, METHOD_VAL, "%s", this_method);
      snprintf(new_cred.url, MAX_VALUE, "%s", urlval);
      llhttp_reset(parser);
      return new_cred;
    }else{
      fprintf(stderr , "parsing error: %s\n" , llhttp_errno_name(err));
      exit(1);
    }
}



