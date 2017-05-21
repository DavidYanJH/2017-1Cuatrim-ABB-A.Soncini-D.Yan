#include <stdlib.h>
#include <string.h>
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
	int cmp_result = cmp(root->clave, clave);
	if (cmp_result == 0) return root;
	if (cmp_result < 0) return buscar_nodo(root->der, cmp, clave);
	return buscar_nodo(root->izq, cmp, clave);
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
		if (cmp_result < 0) padre->der = nodo;
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

nodo_abb_t* buscarReemplazoDerecho(nodo_abb_t* root) {
	if (!root->izq) return root;
	return buscarReemplazoDerecho(root->izq);
}

void borrarHoja(abb_t* abb, nodo_abb_t* nodo_borrar) {
	if (nodo_borrar->padre) {
		if (nodo_borrar == nodo_borrar->padre->izq) //isLeft
			nodo_borrar->padre->izq = NULL;
		else
			nodo_borrar->padre->der = NULL;
	}
	else abb->root = NULL;
}

void borrarPadreConHijo(abb_t* abb, nodo_abb_t* nodo_borrar, nodo_abb_t* nodo_borrar_hijo) {
	if (nodo_borrar->padre) {
		if (nodo_borrar == nodo_borrar->padre->izq) //isLeft
			nodo_borrar->padre->izq = nodo_borrar_hijo;
		else
			nodo_borrar->padre->der = nodo_borrar_hijo;
	}
	else abb->root = nodo_borrar_hijo;
	nodo_borrar_hijo->padre = nodo_borrar->padre;
}

void borrarPadreDosHijos(nodo_abb_t* nodo_borrar) {
	nodo_abb_t* reemplazo = buscarReemplazoDerecho(nodo_borrar->der);
	nodo_borrar->clave = reemplazo->clave;
	nodo_borrar->dato = reemplazo->dato;
	if (reemplazo->padre != nodo_borrar)
		reemplazo->padre->izq = reemplazo->der;
	else
		nodo_borrar->der = reemplazo->der;
	if (reemplazo->der)
		reemplazo->der->padre = reemplazo->padre;
	nodo_borrar = reemplazo;
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
	nodo_abb_t* nodo = buscar_nodo(abb->root, abb->comparar_clave, clave);
	if (!nodo) return false;
	return true;
}

void *abb_obtener(const abb_t *abb, const char *clave) {
	if (!abb) return false;
	nodo_abb_t* nodo = buscar_nodo(abb->root, abb->comparar_clave, clave);
	if (!nodo) return NULL;
	return nodo->dato;
}

bool abb_guardar(abb_t *abb, const char *clave, void *dato) {
	if (!abb) return false;
	return abb_insertar(abb->root, NULL, abb, clave, dato);
}

void *abb_borrar(abb_t *abb, const char *clave) {
	nodo_abb_t* nodo_borrar = buscar_nodo(abb->root, abb->comparar_clave, clave);
	if (!nodo_borrar) return NULL;
	void* dato = nodo_borrar->dato;
	char* clave_borrar = nodo_borrar->clave;

	//Nodo a borrar no tiene hijos
	if (!nodo_borrar->izq && !nodo_borrar->der)
		borrarHoja(abb, nodo_borrar);
	
	//Nodo a borrar solo tiene hijo izquierdo
	if (nodo_borrar->izq && !nodo_borrar->der)
		borrarPadreConHijo(abb, nodo_borrar, nodo_borrar->izq);
	
	//Nodo a borrar solo tiene hijo derecho
	if (!nodo_borrar->izq && nodo_borrar->der)  
		borrarPadreConHijo(abb, nodo_borrar, nodo_borrar->der);

	//Nodo a borrar tene dos hijos
	if (nodo_borrar->izq && nodo_borrar->der)
		borrarPadreDosHijos(nodo_borrar);
	
	free(clave_borrar);
	free(nodo_borrar);
	abb->cantidad_nodos--;
	return dato;
}

