#include "hashmap.h"

#define ERROR(msg) fprintf(stderr, "Error: %s | File: %s | Function: %s | Line: %d\n", msg, __FILE__, __func__, __LINE__)

static uint32_t ROTL32(uint32_t x , int8_t r){
    return (x<<r) | (x >> (32 - r));
}

static  uint32_t fmix32 ( uint32_t h )
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

static int max (int a , int b){
    return (a>b) ? a : b;
}

static bool type_comparator(void * a , void * b , Keytype type){
    if(type == INT_TYPE){
        return *((int *)a) ==  *((int *)b);
    }

    else if (type == STR_TYPE){
        return strcmp((char *)a , (char*)b) == 0;
    }
}

void hmap_str_free(hmap_str ** keyval){
        if((*keyval)->key){
            free((*keyval)->key);
            (*keyval)->key = NULL;
        }
        if((*keyval)->value){
            free((*keyval)->value);
            (*keyval)->value = NULL;
        }

        free((*keyval));
        *keyval = NULL;
}

static int find_length(void * key , Keytype type){
    if(type == INT_TYPE){
        return sizeof(*((int *)key));
    }
    if(type == STR_TYPE){
        return strlen((char *)key) + 1;
    }
    
}




hmap_str *hmap_str_init(void *key, void *value, Keytype type) {

    hmap_str *new = (hmap_str *)calloc(1, sizeof(hmap_str));
   
    if (!new) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    if (type == INT_TYPE) {
        int *temp = malloc(sizeof(int));
        if (!temp) {
            printf("Memory allocation failed for key!\n");
            free(new);
            return NULL;
        }
        *temp = *((int *)key);
        new->key = temp;
        new->ktype = INT_TYPE;
        new->key_length = sizeof(*temp);
    } 
    else if (type == STR_TYPE) {
        int len = strlen((char *)key) + 1;  
        char *temp = malloc(len);
        if (!temp) {
            printf("Memory allocation failed for key!\n");
            free(new);
            return NULL;
        }
        strcpy(temp, (char *)key);
        new->key = temp;
        new->ktype = STR_TYPE;
        new->key_length = len;
    }
    // The user is responsible for allocating `value`
    new->value = value;
    return new;
}

HashMap *map_init(){    
    HashMap *new = (HashMap *) malloc(sizeof(HashMap));
    if(!new){
        printf("memory allocation failed.\n");
        exit(1);
    }
    new->maxindex = 0;
    new->hlen = 8;
    new->length = 0;
    new->seed = 40;
    new->map = (hmap_str **)malloc(8*sizeof(hmap_str *));

    if (!new->map) {
        printf("Memory allocation for map failed.\n");
        free(new);
        exit(1);
    }
    for (int i = 0; i < new->hlen; i++) {
        new->map[i] = NULL;
    }

    return new;
}


int map_add(HashMap **hmap , void * key , void * value , Keytype type){
    
    hmap_str *keyval;
    keyval = hmap_str_init(key , value , type);
    
    if(!(*hmap)){
        *hmap = map_init();
    }

    int seed = (*hmap)->seed;

    if( (*hmap)->length >= ((*hmap)->hlen / 3)*2 ){
        hmap_str** tofree = (*hmap)->map;
        (*hmap)->hlen *= 2; 
        hmap_str **newblk = (hmap_str **)calloc((*hmap)->hlen,sizeof(hmap_str *));
        int maxidx = (*hmap)->maxindex;
        (*hmap)->maxindex = 0;
        for(int i=0; i<=((*hmap)->hlen) / 2 ; i++){
            uint32_t idx;
            if((*hmap)->map[i]){
                
                murmurHash32(((*hmap)->map[i])->key ,  ((*hmap)->map[i])->key_length, seed , &idx);
                // printf("key: %s , hash :%d %d %d \n" , (char *)(*hmap)->map[i]->key , abs(idx) ,keyval->key_length , seed);
                int index = abs(idx)%((*hmap)->hlen);
                if(newblk[index]){
                    while(newblk[index]){
                        index = (index+1)%((*hmap)->hlen);
                    }
                }
            newblk[index]= ((*hmap)->map[i]);    
            }        
        }
        (*hmap)->map = newblk;
        free(tofree);
    }

    uint32_t idx;
    murmurHash32(keyval->key , keyval->key_length , seed , &idx);
    int index = abs(idx)%((*hmap)->hlen);
    if((*hmap)->map[index]){
        while((*hmap)->map[index]){
            if(type_comparator((*hmap)->map[index]->key , key , type)){
                hmap_str * tempmap = (*hmap)->map[index];
                (*hmap)->map[index] = keyval;
                free(tempmap);
                return 0;
            }
            index = (index+1)%((*hmap)->hlen);
        }
    };

    (*hmap)->map[index] = keyval;
    (*hmap)->length += 1;
    return 0;
}

int map_has(HashMap **hmap , void * key ,Keytype type){
    int key_length = find_length(key , type);
    if (!(*hmap) || !(*hmap)->map){
        printf("key not found\n");
        return -1;
    }
    
    uint32_t idx;
    murmurHash32(key , key_length , (*hmap)->seed , &idx);
    int index = abs(idx)%((*hmap)->hlen);
    if((*hmap)->map[index]){    
        while((*hmap)->map[index]){
            if(type_comparator((*hmap)->map[index]->key , key , type)){
                return index;
                }
            index = (index+1)%((*hmap)->hlen);
        }
    }
    return -1;
}

void * map_get(HashMap **hmap , void * key  ,Keytype type){

    int key_length = find_length(key , type);
    if(!(*hmap)){
        printf("key not found\n");
        return NULL;
    }
    int index = map_has(hmap , key , type);
    if(index == -1){
        return NULL; 
    }
    return (*hmap)->map[index]->value;
}


int  map_delete(HashMap **hmap , void *key , Keytype type){
    int  index = map_has(hmap , key , STR_TYPE);
    if(index != -1){
        hmap_str_free(&((*hmap)->map[index]));
        (*hmap)->length -=1 ;
        return 0;
    }
    else{
        ERROR("Key doesn't exist");
        exit(1);
        return 1;
    }
}

int map_destroy(HashMap **hmap){
    if(!(*hmap)){
        printf("map is empty \n");
        return -1;
    }
    for(int k=0 ; k < (*hmap)->hlen; k++){
        if((*hmap)->map[k]){
            hmap_str_free(&(*hmap)->map[k]);
        }
    }
    return 0;
}

void murmurHash32 ( const void * key, int len, uint32_t seed, void * out ){
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 4;
    int i;
    
    uint32_t h1 = seed;
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

    for(i = -nblocks; i; i++){
        uint32_t k1 = getblock(blocks,i);

        k1 *= c1;
        k1 = ROTL32(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1,13); 
        h1 = h1*5+0xe6546b64;
        }

    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

    uint32_t k1 = 0;

    switch(len & 3){
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
        k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;};

    h1 ^= len;

    h1 = fmix32(h1);

    *(uint32_t*)out = h1;
} 


//usage

// int main(){
//     HashMap *map = NULL;
//     int * i = (int *)malloc(sizeof(int));
//     *i = 5;
//     map_add(&map , "hello" , i , STR_TYPE);
//     int * j = (int *)malloc(sizeof(int));
//     *j = 10;
//     map_add(&map , "skibidi" , j , STR_TYPE); 

        //adding if already exists modifies value
//     int * what = (int *)malloc(sizeof(int));
//     *what = 69;
//     printf("%d  %d\n" , map_has(&map , "skibidi" ,  STR_TYPE) ,*(int *)map_get(&map , "skibidi" , STR_TYPE)); 
//     map_add(&map , "skibidi" , what , STR_TYPE);
//     map_delete(&map , "skibidi" , STR_TYPE);
//     map_delete(&map , "hello" , STR_TYPE);
//     map_destroy(&map);
// }
