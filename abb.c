//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <stdbool.h>
//#include <stddef.h>
#include "abb.h"

typedef struct nodo nodo_abb_t;

struct nodo {
	char* clave;
	void* dato;
	nodo_abb_t* padre;
	nodo_abb_t* izq;
	nodo_abb_t* der;
};

struct abb {
	nodo_abb_t* root;
	abb_destruir_dato_t destruir_dato;
	abb_comparar_clave_t comparar_clave;
	size_t cantidad_nodos;
};


/* ******************************************************************
 *            DECLARACION DE LAS FUNCIONES AUXILIARES
 * *****************************************************************/
nodo_abb_t* buscar_nodo(nodo_abb_t* nodo, int (*cmp) (const char*, const char*), const char* clave) {
	if (!nodo) return NULL;
	int cmp_result = cmp(nodo->clave, clave);
	if (cmp_result > 0) return buscar_nodo(nodo->izq, cmp, clave);
	if (cmp_result < 0) return buscar_nodo(nodo->der, cmp, clave);
	return nodo;
}


nodo_abb_t* nodo_abb_crear(abb_t* abb, nodo_abb_t* padre, const char* clave, void* dato) 
{
	nodo_abb_t* nodo = malloc(sizeof(nodo_abb_t));
	if (!nodo) return NULL;
	nodo->clave = malloc(sizeof(char) * (strlen(clave) + 1));
	if (!nodo->clave)
	{
		free(nodo);
		return NULL;
	}
	strcpy(nodo->clave, clave);
	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->padre = padre;
	if (padre != NULL)
	{
		int cmp_result = abb->comparar_clave(padre->clave, nodo->clave);
		if (cmp < 0) padre->der = nodo;
		else padre->izq = nodo;
	}
	++abb->cantidad_nodos;
	return nodo;
}


bool abb_insertar(nodo_abb_t* nodo, nodo_abb_t* padre, abb_t* abb, const char* clave, void* dato)
{
	if (!nodo) 
	{
		nodo = nodo_abb_crear(abb, padre, clave, dato);
		return (nodo != NULL);
	}
	int cmp_result = abb->comparar_clave(nodo->clave, clave);
	if (cmp_result > 0) 
		return abb_insertar(nodo->izq, nodo->padre, abb, clave, dato);
	if (cmp_result < 0) 
		return abb_insertar(nodo->der, nodo->padre, abb, clave, dato);
	
	// Caso Clave Ya Existente en el ABB
	if (abb->destruir_dato && nodo->dato) 
		abb->destruir_dato(nodo->dato);
	nodo->dato = dato;
	return true;
}

/* ******************************************************************
 *            PRIMITIVAS DEL ARBOL BINARIO DE BUSQUEDA
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	if (!cmp) return NULL;
	abb_t* abb = malloc(sizeof(abb_t));
	if (!abb) return NULL;
	abb->root = NULL;
	abb->comparar_clave = cmp;
	abb->destruir_dato = destruir_dato;
	abb->cantidad_nodos = 0;
	return abb;
}

size_t abb_cantidad(abb_t *abb) {
	if (!abb) return 0;
	return abb->cantidad_nodos;
}

bool abb_pertenece(const abb_t *abb, const char *clave) {
	if (!abb) return false;
	nodo_abb_t* nodo = buscar_nodo(abb->root, abb->comparar_clave, clave, NULL);
	if (!nodo) return false;
	return true;
}

void *abb_obtener(const abb_t *abb, const char *clave) {
	if (!abb) return false;
	nodo_abb_t* nodo = buscar_nodo(abb->root, abb->comparar_clave, clave);
	if (!nodo) return NULL;
	return nodo->dato;
}

/* Guarda un elemento en el abb. Si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue creada.
 * Post: Se almacenó el par (clave, dato). Si se pudo guardar el
 * dato, se devuelve true. Caso contrario, false.
 */
bool abb_guardar(abb_t *abb, const char *clave, void *dato) {
	if (!abb) return false;
	return abb_insertar(abb->root, NULL, abb, clave, dato);
}
