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
nodo_abb_t* buscar_nodo(nodo_abb_t* root, int (*cmp) (const char *, const char *), const char* clave) {
	if (!root) return NULL;
	int cmpresul = cmp(root->clave, clave);
	if (cmpresul == 0) return root;
	if (cmpresul < 0) return buscar_nodo(root->der, cmp, clave);
	return buscar_nodo(root->izq, cmp, clave);
}

/*nodo_abb_t* buscar_nodo(nodo_abb_t* root, int (*cmp) (const char *, const char *), const char* clave, nodo_abb_t (*nodo_crear) (const char*, nodo_abb_t*)) {
	if ((!root) && (!nodo_crear)) return NULL;
	if (!root) {
		root = nodo_crear(clave, padre);
		return root;
	}
	int cmpresul = cmp(root->clave, clave);
	if (cmpresul == 0) return root;
	if (cmpresul < 0) return buscar_nodo(root->der, cmp, clave, nodo_crear);
	return buscar_nodo(root->izq, cmp, clave, nodo_crear);
}*/

noto_abb_t* nodo_crear(const char* clave, nodo_abb_t* padre, void* dato) {
	nodo_abb_t nodo = malloc(sizeof(nodo_abb_t));
	if (!nodo) return NULL;
	nodo->clave = mallo(sizeof(char) * strlen(clave) + 1);
	if (!nodo->clave){
		free(nodo);
		return NULL;
	}
	strcpy(nodo->clave, clave);
	nodo->dato = dato;
	return nodo;
}

bool nodo_insertar(nodo_abb_t* root, int (*cmp) (const char *, const char *), const char* clave, void* dato) {
	if (!root) return false;
	int cmpresul = cmp(root->clave, clave);
	
	if (cmpresul == 0) {
		root->dato = dato;
		return true;
	}

	if (cmpresul < 0) {
		if (!root->der) {
			root->der = nodo_crear(clave, root, dato);
		}
		nodo_insertar(root->der, cmp, clave, dato);
	}
	
	if (cmpresul > 0) {
		if (!root->izq)
			root->izq = nodo_crear(clave, root, dato);
		nodo_insertar(root->izq, cmp, clave, dato);
	}
}

bool nodo_abb_t_crear(nodo_abb_t* root, int (*cmp) (const char *, const char *), const char* clave, void* dato)


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
	bool ok = nodo_insertar(abb->root, abb->comparar_clave, clave, dato);
	if (OK) abb->cantidad_nodos++;
	return ok;
}
