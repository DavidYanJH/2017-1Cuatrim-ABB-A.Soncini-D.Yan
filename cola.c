#include "cola.h"
#include <stdlib.h>

/* ******************************************************************
 *                   ANDRES ELISEO SONCINI
 *                   PADRON: 100265
 *                   CORRECTOR: Martín Buchwald
 * *****************************************************************/

typedef struct nodo nodo_t;

struct nodo {
	void* data;
	nodo_t* next;	
};

struct cola {
	nodo_t* first;
	nodo_t* last;
};

cola_t* cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if (cola == NULL){
		return cola;
	}
	cola->first = NULL;
	cola->last = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)) {
	while (!cola_esta_vacia(cola)) {
		void* dato = cola_desencolar(cola);
		if (destruir_dato != NULL) {
			destruir_dato(dato);
		}
	}
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola) {
	return (cola->first == NULL);
}

void* nodo_crear(void* valor) {
	nodo_t* nodo = malloc(sizeof(cola_t));
	if (nodo != NULL) {
		nodo->data = valor;
		nodo->next = NULL;
	}
	return nodo;
}

bool cola_encolar(cola_t *cola, void* valor) {
	nodo_t* nodo = nodo_crear(valor);
	if (cola_esta_vacia(cola)){
		cola->first = nodo;
	}
	else {
		(cola->last)->next = nodo;
	}
	cola->last = nodo;
	return true;
}

void* cola_ver_primero(const cola_t *cola) {
	if (cola_esta_vacia(cola)) {
		return NULL;
	}
	return (cola->first)->data;
}

void* cola_desencolar(cola_t *cola) {
	if (cola_esta_vacia(cola)) {
		return NULL;
	}
	nodo_t* doomed = cola->first;
	cola->first = doomed->next;
	if (cola_esta_vacia(cola)) {
		cola->last = NULL;
	}
	void* data = doomed->data;
	free(doomed);
	return data;
}