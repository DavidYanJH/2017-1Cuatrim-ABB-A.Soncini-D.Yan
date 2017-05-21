#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;

/* Tipo de función para comparar claves. No puede ser NULL.
*/
typedef int (*abb_comparar_clave_t) (const char *, const char *);

/* Tipo de función para destruir datos. Puede ser NULL
*/
typedef void (*abb_destruir_dato_t) (void *);

/* Crea el abb. Recibe una función de comparación para las claves
 * y otra de destrucción para los datos almacenados. La función de 
 * destrucción puede ser NULL.
 * PRE: La funcion de comparacion no es NULL.
 * POST: Se crea el abb vacio. Si falla creación se devuelve NULL.
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


/* Guarda un elemento en el abb. Si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue creada.
 * Post: Se almacenó el par (clave, dato). Si se pudo guardar el
 * dato, se devuelve true. Caso contrario, false.
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb, devuelve el dato asociado. Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue creada.
 * Post: El elemento fue borrado de la estructura y se lo devolvió
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura abb fue creada.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al abb.
 * Pre: La estructura abb fue creada.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);


/* Devuelve la cantidad de datos guardados en el abb.
 * Pre: La estructura abb fue creada.
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura, llamando a la función
 * destruir para cada par (clave, dato). Si la función
 * destruir es NULL, no se destruyen los datos, pero sí 
 * la estructura.
 * Pre: La estructura abb fue creada.
 * Post: La estructura abb fue destruida y los datos 
 * destruidos o no, dependiendo de la función destruir.
 */
void abb_destruir(abb_t *arbol);

/* Recorre los datos de menor a mayor, aplicando a cada dato la función visitar.
 * Recibe una función callback "visitar" junto con el arbol y un puntero extra.
 * La misma devuelve true si se debe seguir recorriendo, false en caso contrario. El resultado de
 * la funcion es entregado al usuario mediante el puntero extra. Si el usuario no necesita
 * el extra debe pasarlo como un parametro NULL.
 * Pre: La estructura abb fue creada.
 * Post: La estructura abb fue destruida y los datos 
 * destruidos o no, dependiendo de la función destruir.
 */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

typedef struct abb_iter abb_iter_t;

/* Crea un iterador in-order del arbol.
 * Pre: El arbol fue creado.
 * Post: Devuelve un iterador nuevo para el arbol pasado por referencia.
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza in-order hacia la siguiente clave del arbol.
 * Pre: El iterador fue creado.
 * Post: Devuelve true si se pudo avanzar. Devuelve false si ya se 
 * encontraba al final del arbol o en caso de falta de memoria.
 */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Retorna la clave actual a la que apunta el iterador.
 * Pre: El iterador fue creado.
 * Post: Devuelve un puntero a la clave actual o NULL si se 
 * encontraba al final del arbol.
 */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Especifica si el iterador se encuentra al final del arbol.
 * Pre: El iterador fue creado.
 * Post: Devuelve true si iterador está al final, false en 
 * caso contrario.
 */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador de arbol.
 * Pre: El iterador fue creado.
 * Post: El iterador fue destruido.
 */
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H
