#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;

/* Prototipo de función de comparación pasado como parámetro imprescindible
 * a la primitiva de creación del abb. La función de comparación de 
 * claves debe recibir dos punteros de tipo char* y el valor de retorno de
 * la misma está especificado de la siguiente manera...
 * 	cuando a < b --- un int negativo
 * 	cuando a = b --- 0
 *    cuando a > b --- un int positivo
 * El puntero a la función de comparación *abb_comparar_clave debe ser 
 * distinto a NULL necesariamente.
*/
typedef int (*abb_comparar_clave_t) (const char* a, const char* b);

/* Prototipo de función de destruir los datos almacenados en el abb.
 * El puntero de la función de destrucción puede ser NULL en cuyo caso
 * los datos almacenados en el abb no se destruyen.
*/
typedef void (*abb_destruir_dato_t) (void *);

/* Crear el abb. Recibe una función de comparación para las claves
 * y una función de destrucción para los datos almacenados.
 * PRE: La funcion de comparacion debe ser distinto a NULL.
 * POST: Se crea el abb vacio. Si falla la creación se devuelve NULL.
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);


/* Guarda un elemento en el abb. Si la clave ya se encuentra en la
 * estructura de abb, reemplaza el dato asociado.
 * Pre: La estructura abb fue creada.
 * Post: En caso de haber guardado el par clave-dato retorna true, caso contrario false.
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb devolviendo el dato asociado a la clave,
 * NULL si la clave no se encuentra en el abb.
 * Pre: La estructura abb fue creada.
 * Post: El elemento fue borrado y devuelto al usuario si estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del abb asociado a la clave pasada por referencia.
 * En caso de que la clave no se encuentra en el abb retorna NULL.
 * Pre: La estructura abb fue creada.
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si una clave pertenece o no al abb.
 * Pre: La estructura abb fue creada.
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);


/* Nos informa la cantidad de pares clave-dato guardados en el abb.
 * Pre: La estructura abb fue creada.
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura, llamando a la función destruir para cada par (clave, dato). 
 * Si la función destruir es NULL, no se destruyen los datos, pero sí la estructura.
 * Pre: La estructura abb fue creada.
 * Post: La estructura abb fue destruida, los datos dependiendo de la funcion destruir. 
 */
void abb_destruir(abb_t *arbol);

/* Recorre los datos de menor a mayor, aplicando a cada dato la función visitar.
 * Recibe una función callback "visitar" junto con el arbol y un puntero extra.
 * La misma devuelve true si se debe seguir recorriendo, false en caso contrario. 
 * El resultado de la funcion es entregado al usuario mediante el puntero extra. 
 * Si el usuario no necesita el extra debe pasarlo como un parametro NULL.
 * Pre: La estructura abb fue creada.
 * Post: La estructura abb fue destruida y los datos 
 * destruidos o no, dependiendo de la función destruir.
 */
void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra);

typedef struct abb_iter abb_iter_t;

/* Crea un iterador in-order del arbol.
 * Pre: El arbol binario de busqueda fue creado.
 * Post: Retorna un iterador nuevo para el arbol pasado por referencia.
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Avanza in-order hacia el siguiente dato del arbol.
 * Pre: El iterador del abb fue creado.
 * Post: Retorna true si se pudo avanzar, false si ya se encuentra al final del arbol.
 */
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Retorna la clave actual a la que apunta el iterador.
 * Pre: El iterador del abb fue creado.
 * Post: Retorna un puntero a la clave actual o NULL si se 
 * encuentra al final del arbol.
 */
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Especifica si el iterador se encuentra al final del arbol.
 * Pre: El iterador del abb fue creado.
 * Post: Retorna true si iterador está al final, false en caso contrario.
 */
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Destruye el iterador de arbol.
 * Pre: El iterador del abb fue creado.
 * Post: El iterador del abb fue destruido.
 */
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H