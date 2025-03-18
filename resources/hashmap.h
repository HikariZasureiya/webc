#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdint.h>
#include<stdbool.h>
#include<string.h>

typedef enum{
    INT_TYPE,
    STR_TYPE,
    FLOAT_TYPE
} Keytype;

typedef struct hmap{
    void * key;
    Keytype ktype;
    int key_length;
    void * value;
}hmap_str;

typedef struct{
    int seed;
    int length;
    int hlen;
    int maxindex;
    hmap_str **map;
} HashMap;


#define getblock(p , i) (p[i])

void murmurHash32();
void hmap_str_free();
hmap_str *hmap_str_init();
HashMap *map_init();
int map_add();
int map_has();
void * map_get();
int  map_delete();
int map_destroy();

