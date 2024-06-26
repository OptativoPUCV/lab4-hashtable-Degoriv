#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include <stdbool.h>


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
    char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr); }

    return hash % capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
	long indice = hash(key, map->capacity);
	while (true)
		{
			if (map->buckets[indice] == NULL){ //Osea esta vacio
				map->buckets[indice] = createPair(key, value);
				map->size++;
				map->current = indice; 
				break;}
			else {
				indice++;
				if (indice == map->capacity) indice = 0; }
		}
}

void enlarge(HashMap * map) {
	enlarge_called = 1; //no borrar (testing purposes)

	Pair** aux = map->buckets;
	map->capacity *= 2;
	map->buckets = (Pair**) malloc(sizeof(Pair*) * (map->capacity));
	map->size = 0;

	for (int i = 0 ; i < map->capacity / 2 ; i++)
	{
		if (aux[i] != NULL) {
			insertMap(map, aux[i]->key, aux[i]->value); }
	}
	free(aux);
}


HashMap * createMap(long capacity) {
	HashMap *map = (HashMap *)malloc(sizeof(HashMap));
	map->buckets = (Pair **)malloc(sizeof(Pair*)*capacity);
	for(long i = 0 ; i < capacity ; i++) map->buckets[i] = NULL;
	
	map->size = 0;
	map->current = -1;
	map->capacity = capacity;
	return map;
}

void eraseMap(HashMap * map,  char * key) {    
	long indice = hash(key, map->capacity);
	while (map->buckets[indice] != NULL && map->buckets[indice]->key != NULL)
	{
		if (is_equal(map->buckets[indice]->key, key) == 1){
			map->buckets[indice]->key = NULL;
			map->size -= 1;
			return; }
		else {
		indice = (indice + 1) % map->capacity; }
	}
}

Pair * searchMap(HashMap * map,  char * key) {
	long indice = hash(key, map->capacity);
	Pair *pair = map->buckets[indice];
	while (pair != NULL)
	{
		if (is_equal(pair->key, key)) {
			map->current = indice;
			return pair; }
			
		else if (!is_equal(pair->key, key)) {
			indice += 1; 
			if (indice == map->capacity) indice = 0;}
		  pair = map->buckets[indice];
		
		pair = map->buckets[indice];
	}
	return NULL;
}

Pair * firstMap(HashMap * map) {
	if (map == NULL) return NULL;
	for (int i = 0 ; i < map->capacity ; i++)
		{
			if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
				map->current = i;
				return map->buckets[i];
			}
		}
	return NULL;
}

Pair * nextMap(HashMap * map) {
	if (map == NULL) return NULL;
	if (map->current == -1) return NULL;
	
	for (long i = map->current + 1 ; i < map->capacity ; i++)
		{
			if (map->buckets[i] != NULL){
				map->current = i;
				return map->buckets[i]; }
		}
  return NULL;
}
