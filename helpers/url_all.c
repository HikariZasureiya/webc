#include "../headers/url_register.h"
#include "../headers/url_dist.h"

bool file_exists(char * filename){
    if(access(filename , R_OK) == -1 ){
        return false;
    }
    return true;
}

char** url_tokens(const char *url, int *len) {
    int i = 0;
    char *token;
    char *saveptr;
    char *url_copy = strdup(url);
    if (!url_copy) {
        perror("Memory allocation failed");
        return NULL;
    }
    char **tok_arr = (char **)malloc(sizeof(char *));
    if (!tok_arr) {
        perror("Memory allocation failed");
        free(url_copy);
        return NULL;
    }
    token = strtok_r(url_copy, "/", &saveptr);
    while (token != NULL) {
        char **tmp = realloc(tok_arr, (i + 1) * sizeof(char *));
        if (!tmp) {
            perror("Memory allocation failed");
            free(tok_arr);
            free(url_copy);
            return NULL;
        }
        tok_arr = tmp;
        tok_arr[i++] = strdup(token);  
        token = strtok_r(NULL, "/", &saveptr);
    }
    free(url_copy); 
    *len = i;
    return tok_arr;
}


path * path_init(char * route_name){
    path * new_path = (path * ) malloc(sizeof(path));
    if(!new_path){
        printf("path initialize failed\n");   
    }
    else{
        memset(new_path, 0, sizeof(path));
        strcpy(new_path->route_name , route_name);
        strcpy(new_path->source_path , "");
        new_path->connected_no = 0; 
    }
    return new_path;
}

path * add_search(path ** route_node , char ** tok_arr , int len){
        if( *route_node == NULL ){
            *route_node = path_init("");
        }
        int i=0;
        path * current = *route_node;
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
                path * newnode = path_init(tok_arr[i]);
                current->connected[(current->connected_no)] = newnode;
                current->connected_no +=1;
                current = newnode;
            }
            i++;
        }
        return current;
}

void register_route(char * url , char * source){
    int len;
    char ** tok_arr = url_tokens(url , &len);
    path * route_curr = add_search(&route_node , tok_arr , len);
    if(!file_exists(source)){
        printf("file: %s doesn't exist" , source);
        exit(1);
    }
    strncpy(route_curr->source_path , source , strlen(source));
    (route_curr->source_path)[strlen(source)] = '\0';
}

void route_dfs(path * route_node){
    if(route_node == NULL)
        return;
    printf(" this is: %s \t %s\n" , route_node->route_name , route_node->source_path);
    for(int i=0; i<route_node->connected_no; i++){
        route_dfs(route_node->connected[i]);
    }   
    return;
} 

/* url_dist.h*/

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


