#include "url_register.h"

path * search_url(path * route_node ,  char ** tok_arr , int len){
    if( route_node == NULL ){
        return NULL;
    }
    int i=0;
    path * current = route_node;
    while( i < len){
        int temp = current->connected_no;
        int flag = 0;
        for(int j=0; j<temp; j++){
            if(current->connected[j]){
                if(strcmp(current->connected[j]->route_name , tok_arr[i]) == 0 ){
                    current = current->connected[j];
                    flag = 1;
                    break;
                }
            }
        }
        if(flag == 0){
            return NULL;
        }
        i++;
    }
    return current;
}

char* find_route(char * url){
    int len;
    char ** tok_arr = url_tokens(url , &len);
    path * route_curr = search_url(route_node , tok_arr , len);
    if(route_curr)
        return route_curr->source_path;
    
    return "";
}
