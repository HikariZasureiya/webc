#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#define MAX_ROUTE 400
#define NAME_LEN 1000
#define PATH_LEN 3000

typedef struct path{
    char route_name[NAME_LEN];
    char source_path[PATH_LEN];
    struct path* connected[MAX_ROUTE];
    int level;
    int connected_no;
} path; 

extern path * route_node;

bool file_exists();
char** url_tokens(const char *url, int *len);
path * path_init(char * route_name);
path * add_search(path ** route_node , char ** tok_arr , int len);
void register_route();
void route_dfs(); 
void register_routes();










