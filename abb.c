#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "pila.h"


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
	else
	{
		abb->root = nodo;
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
		return abb_insertar(nodo->izq, nodo, abb, clave, dato);
	if (cmp_result < 0) 
		return abb_insertar(nodo->der, nodo, abb, clave, dato);
	
	// Caso Clave Ya Existente en el ABB
	if (abb->destruir_dato && nodo->dato) 
		abb->destruir_dato(nodo->dato);
	nodo->dato = dato;
	return true;
}

bool isleft(abb_t* abb, nodo_abb_t* hijo, nodo_abb_t* padre) {
	if (!padre->izq) return false;
	return (abb->comparar_clave(hijo->clave, padre->izq->clave) == 0);
}

nodo_abb_t* buscarReemplazoDerecho(nodo_abb_t* root) {
	if (!root->izq) return root;
	return buscarReemplazoDerecho(root->izq);
}

nodo_abb_t* buscarReemplazoIzquierdo(nodo_abb_t* root) {
	if (!root->der) return root;
	return buscarReemplazoIzquierdo(root->der);
}

void borrarHoja(abb_t* abb, nodo_abb_t* nodo_borrar) {
	if (nodo_borrar->padre) {
		if (isleft(abb, nodo_borrar, nodo_borrar->padre)) //isLeft
			nodo_borrar->padre->izq = NULL;
		else
			nodo_borrar->padre->der = NULL;
	}
	else abb->root = NULL;
}

void eliminar_nodo_abb(abb_t* abb, nodo_abb_t* root) {
	if (!root) return;
	eliminar_nodo_abb(abb, root->izq);
	eliminar_nodo_abb(abb, root->der);
	if (abb->destruir_dato)
		abb->destruir_dato(root->dato);
	free(root->clave);
	free (root);
}

void iterar_nodo_abb_in(nodo_abb_t* root, bool visitar(const char *, void *, void *), void *extra) {
	if (!root) return;
	if (root->izq) {
		iterar_nodo_abb_in(root->izq, visitar, extra);
	}
	if (visitar(root->clave, root->dato, extra))
		iterar_nodo_abb_in(root->der, visitar, extra);	
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

void* abb_borrar(abb_t *abb, const char *clave) {
	nodo_abb_t* nodo_borrar = buscar_nodo(abb->root, abb->comparar_clave, clave);
	if (!nodo_borrar) return NULL;
	void* dato = nodo_borrar->dato;
	char* clave_borrar = nodo_borrar->clave;

	//Nodo a borrar no tiene hijos
	if (!nodo_borrar->izq && !nodo_borrar->der)
		borrarHoja(abb, nodo_borrar);

	else { 
		nodo_abb_t* reemplazo = NULL;
	
		//Nodo a borrar solo tiene hijo izquierdo
		if (nodo_borrar->izq && !nodo_borrar->der)
			reemplazo = buscarReemplazoIzquierdo(nodo_borrar->izq);
	
		//Nodo a borrar solo tiene hijo derecho
		else if (!nodo_borrar->izq && nodo_borrar->der)  
			reemplazo = buscarReemplazoDerecho(nodo_borrar->der);

		//Nodo a borrar tene dos hijos
		else if (nodo_borrar->izq && nodo_borrar->der)
			reemplazo = buscarReemplazoDerecho(nodo_borrar->der);

		if (isleft(abb, reemplazo, reemplazo->padre))
			reemplazo->padre->izq = reemplazo->der;
		else
			reemplazo->padre->der = reemplazo->der;
		if (reemplazo->der)
				reemplazo->der->padre = reemplazo->padre;
		nodo_borrar->clave = reemplazo->clave;
		nodo_borrar->dato = reemplazo->dato;
		nodo_borrar = reemplazo;
	}
	free(clave_borrar);
	free(nodo_borrar);
	abb->cantidad_nodos--;
	return dato;
}

void abb_destruir(abb_t* abb) {
	if (!abb) return;
	if (abb->root)
		eliminar_nodo_abb(abb, abb->root);
	free(abb);
}

/* ******************************************************************
 *            ITERADOR DEL ARBOL BINARIO DE BUSQUEDA
 * *****************************************************************/

// Declaracion de la estructura abb_iter_in
struct abb_iter {
	pila_t* pila_abb;
};

// Funcion Auxiliar Recursviva
bool pila_cargar_inorder(nodo_abb_t* nodo_actual, pila_t* pila_abb)
{
	if (nodo_actual == NULL) return true;
	if (!pila_apilar(pila_abb, nodo_actual)) return false;
	return pila_cargar_inorder(nodo_actual->izq, pila_abb);
}

/* ******************************************************************
 *            PRIMITIVAS DEL ITERADOR EXTERNO DEL ABB
 * *****************************************************************/
abb_iter_t* abb_iter_in_crear(const abb_t* abb)
{
	if (!abb || abb->cantidad_nodos == 0) return NULL;
	abb_iter_t* abb_iter = malloc(sizeof(abb_iter_t));
	if (!abb_iter) return NULL;
	abb_iter->pila_abb = pila_crear();
	if (!abb_iter->pila_abb)
	{
		abb_iter_in_destruir(abb_iter);
		return NULL;
	}
	if (!pila_cargar_inorder(abb->root, abb_iter->pila_abb))
	{
		abb_iter_in_destruir(abb_iter);
		return NULL;
	}
	return abb_iter;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* abb_iter)
{
	if (!abb_iter) return NULL;
	if (abb_iter_in_al_final(abb_iter)) return NULL;
	nodo_abb_t* nodo_actual = pila_ver_tope(abb_iter->pila_abb);
	return nodo_actual->clave;
}

bool abb_iter_in_avanzar(abb_iter_t* abb_iter)
{
	if (!abb_iter) return false;
	if (abb_iter_in_al_final(abb_iter)) return false;
	nodo_abb_t* nodo_actual = pila_desapilar(abb_iter->pila_abb);
	if (nodo_actual->der) 
		return pila_cargar_inorder(nodo_actual->der, abb_iter->pila_abb);
	return true;
}

bool abb_iter_in_al_final(const abb_iter_t* abb_iter)
{
	if (!abb_iter) return true;
	return (pila_esta_vacia(abb_iter->pila_abb));
}

void abb_iter_in_destruir(abb_iter_t* abb_iter)
{
	if (!abb_iter) return;
	if (abb_iter->pila_abb) pila_destruir(abb_iter->pila_abb);
	free(abb_iter);
}

/* ******************************************************************
 *          ITERADOR INTERNO DEL ARBOL BINARIO DE BUSQUEDA
 * *****************************************************************/
 void abb_in_order(abb_t *abb, bool visitar(const char *, void *, void *), void *extra) {
	if (abb->root)
		iterar_nodo_abb_in(abb->root, visitar, extra);
}