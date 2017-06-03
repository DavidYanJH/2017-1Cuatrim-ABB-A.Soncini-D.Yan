#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "abb.h"
#include "cola.h"
#include "testing.h"

/* ******************************************************************
 *                   FUNCIONES DE PRUEBA AUXILIARES
 * *****************************************************************/
bool visitar_test(const char* clave, void* dato, void* extra) {
	char* datocpy = malloc(sizeof(char)* strlen(dato) + 1);
	strcpy(datocpy, dato);
	return cola_encolar((cola_t*) extra, datocpy);
}

bool visitar_test_interrupt(const char* clave, void* dato, void* extra) {
	char* datocpy = malloc(sizeof(char)* strlen(dato) + 1);
	strcpy(datocpy, dato);
	//Itero hasta el nodo raiz
	if (strcmp(clave, "110") == 0)
		return !cola_encolar((cola_t*) extra, datocpy);
	return cola_encolar((cola_t*) extra, datocpy);
}

int strcmpaux (const char* clave, const char* straux) {
	int intclave = atoi(clave);
	int intstraux = atoi(straux);

	if (intclave < intstraux)
		return -1;
	if (intclave > intstraux)
		return 1;
	else return 0;
}

bool visitar_print(const char* clave, void* dato, void* aux) {
    printf(" %s |", clave);
    return true;
}

void prueba_abb_eliminar_hojas_aux(cola_t* extra, char** ordenados) {
    int i = 0;
    bool ok = true;
    char* aux = NULL;
    while (!cola_esta_vacia(extra) && ok) {
        aux = (char*) cola_desencolar(extra);
        ok = (strcmp(aux, ordenados[i++]) == 0);
        free(aux);
    }
    print_test("Se itero todo el arbol luego de una eliminacion", ok);
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/
static void prueba_crear_abb_vacio(void)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

static void prueba_abb_insertar(void)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    abb_destruir(abb);
}

static void prueba_abb_reemplazar(void)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("aaaPrueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

static void prueba_abb_reemplazar_con_destruir(void)
{
    abb_t* abb = abb_crear(strcmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

static void prueba_abb_borrar()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    /* Inserta 3 valores y luego los borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));

    /* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
    print_test("Prueba abb pertenece clave3, es verdadero", abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave3, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb borrar clave3, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave3, es falso", !abb_pertenece(abb, clave3));
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb pertenece clave1, es verdadero", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb borrar clave1, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave1, es falso", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);

    print_test("Prueba abb pertenece clave2, es verdadero", abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave2, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb borrar clave2, es NULL", !abb_borrar(abb, clave3));
    print_test("Prueba abb pertenece clave2, es falso", !abb_pertenece(abb, clave2));
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_clave_vacia(void)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_valor_null(void)
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

static void prueba_abb_iterar_int_completo()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"5555", "2222", "8888", "6666", "5666", "7777", "7788", "7789", "9888", "9997", "9999", "1111", "110", "111", "1233", "1122", "1113", "1444", "1237", "1235", "1236", "1445", "1456", "3333", "4444"};
    char *valores[] = {"5555", "2222", "8888", "6666", "5666", "7777", "7788", "7789", "9888", "9997", "9999", "1111", "110", "111", "1233", "1122", "1113", "1444", "1237", "1235", "1236", "1445", "1456", "3333", "4444"};

    //Este vector ordenado contiene las claves odenadas de menor a mayor. El resultado del in-order debe ser igual al orden de este vector.
    char *ordenados[] = {"110", "111", "1111", "1113", "1122", "1233", "1235", "1236", "1237", "1444", "1445", "1456", "2222", "3333", "4444", "5555", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    

    /* Inserta 25 valores */
    int i = 0;
    bool ok = true;
    while ((i < 25) && ok) {
    	//inserto mismo char* clave / valor
    	ok = abb_guardar(abb, claves[i], valores[i]);
    	i++;
   	}

    print_test("Se insertaron todos las duplas valor / clave", ok);
    print_test("Cantidad de elementos insertados es 25", 25 == abb_cantidad(abb));

    cola_t* extra = cola_crear();

    abb_in_order(abb, visitar_test, extra);

    i = 0;
    ok = true;
    while (!cola_esta_vacia(extra) && ok) {
    	char* aux = (char*) cola_desencolar(extra);
    	ok = (strcmp(aux, ordenados[i++]) == 0);
    	free(aux);
    }

    print_test("Se iteró inorder correctamente", ok);
    
    cola_destruir(extra, free);

   	abb_destruir(abb);
}

static void prueba_abb_iterar_int_interrupt()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    char *claves[] = {"5555", "2222", "8888", "6666", "5666", "7777", "7788", "7789", "9888", "9997", "9999", "1111", "110", "111", "1233", "1122", "1113", "1444", "1237", "1235", "1236", "1445", "1456", "3333", "4444"};
    char *valores[] = {"5555", "2222", "8888", "6666", "5666", "7777", "7788", "7789", "9888", "9997", "9999", "1111", "110", "111", "1233", "1122", "1113", "1444", "1237", "1235", "1236", "1445", "1456", "3333", "4444"};

    //Este vector ordenado contiene las claves odenadas de menor a mayor. El resultado del in-order debe ser igual al orden de este vector.
    char *ordenados[] = {"110", "111", "1111", "1113", "1122", "1233", "1235", "1236", "1237", "1444", "1445", "1456", "2222", "3333", "4444", "5555", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    
    cola_t* extra = cola_crear();

    /* Inserta 25 valores */
    int i = 0;
    bool ok = true;
    while ((i < 25) && ok) {
    	//inserto mismo char* clave / valor
    	ok = abb_guardar(abb, claves[i], valores[i]);
    	i++;
   	}

    print_test("Se insertaron todos las duplas valor / clave", ok);
    print_test("Cantidad de elementos insertados es 25", 25 == abb_cantidad(abb));

    abb_in_order(abb, visitar_test_interrupt, extra);

    i = 0;
    ok = true;
    char* aux = NULL;
    while (!cola_esta_vacia(extra) && ok) {
    	free(aux);
    	aux = (char*) cola_desencolar(extra);
    	ok = (strcmp(aux, ordenados[i++]) == 0);
    }

    print_test("Se iteró hasta nodo menor", strcmp(aux, "110") == 0);
    free(aux);
    
    cola_destruir(extra, free);

   	abb_destruir(abb);
}

void prueba_abb_iterar_int_volumen(int size_test) {
	srand((int) time(NULL));   // should only be called once	
	int vector[size_test];
	int i;
	for (i = 0; i < size_test; i++) {
		vector[i] = (int) rand();      // returns a pseudo-random integer between 0 and RAND_MAX	
	}
	char* strvec[size_test];
	for (i = 0; i < size_test; i++) {
		char aux1[20];
		sprintf(aux1, "%d", vector[i]);
		strvec[i] = malloc(sizeof(char) * strlen(aux1) + 1);
		strcpy(strvec[i], aux1);
	}
	abb_t* abb = abb_crear(strcmpaux, free);
	i = 0;
	bool ok = true;
	while ((i < size_test) && ok) {
		ok = abb_guardar(abb, strvec[i], strvec[i]);
		i++;
	}
	printf("Cantidad de nodos en abb: %d\n", (int) abb_cantidad(abb));
   	cola_t* extra = cola_crear();
    abb_in_order(abb, visitar_test, extra);
    ok = true;
    char* aux = (char*) cola_desencolar(extra);
    while (!cola_esta_vacia(extra) && ok) {
    	char* aux2 = (char*) cola_desencolar(extra);
    	ok = (strcmpaux(aux, aux2) < 0);
    	free(aux);
    	aux = aux2;
    }
    free(aux);
    print_test("Cola esta vacia", cola_esta_vacia(extra));
    print_test("Se iteró inorder correctamente", ok);  
    cola_destruir(extra, free);
   	abb_destruir(abb);		
}

static void prueba_abb_iterar_ext_vacio()
{
    abb_t* abb = abb_crear(strcmp, NULL);
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iter crear iterador abb vacio", iter);
    print_test("Prueba abb iter esta al final", abb_iter_in_al_final(iter));
    print_test("Prueba abb iter avanzar es false", !abb_iter_in_avanzar(iter));
    print_test("Prueba abb iter ver actual es NULL", !abb_iter_in_ver_actual(iter));

    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

void prueba_abb_iterar_ext_volumen (int size_test) {
    /********************************************************/
    /*    INICIO DE CREACION DE COLA INORDER DE TESTEO      */
    /********************************************************/
    srand((int) time(NULL));   // should only be called once    
    int vector[size_test];
    int i;
    for (i = 0; i < size_test; i++) {
        vector[i] = (int) rand();      // returns a pseudo-random integer between 0 and RAND_MAX    
    }
    char* strvec[size_test];
    for (i = 0; i < size_test; i++) {
        char aux1[20];
        sprintf(aux1, "%d", vector[i]);
        strvec[i] = malloc(sizeof(char) * strlen(aux1) + 1);
        strcpy(strvec[i], aux1);
    }
    abb_t* abb = abb_crear(strcmpaux, free);
    i = 0;
    bool ok = true;
    while ((i < size_test) && ok) {
        ok = abb_guardar(abb, strvec[i], strvec[i]);
        i++;
    }
    printf("Cantidad de nodos en abb: %d\n", (int) abb_cantidad(abb));
    cola_t* extra = cola_crear();
    abb_in_order(abb, visitar_test, extra);

    /********************************************************/
    /*     FIN DE CREACION DE COLA INORDER DE TESTEO        */
    /********************************************************/

    abb_iter_t* iter = abb_iter_in_crear(abb);
    ok = true;
    while (!abb_iter_in_al_final(iter) && !cola_esta_vacia(extra) && ok) {
        char* aux = (char*) cola_desencolar(extra);
        char* aux2 = (char*) abb_iter_in_ver_actual(iter);
        ok = (strcmpaux(aux, aux2) == 0);
        free(aux);
        abb_iter_in_avanzar(iter);
    }

    print_test("Cola esta vacia", cola_esta_vacia(extra));
    print_test("El iterador externo inorder iteró correctamente", ok);  
    print_test("El iterador externo inorder esta al final",abb_iter_in_al_final(iter));

    cola_destruir(extra, free);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);      
}

static void prueba_abb_eliminar_hojas()
{
    abb_t* abb = abb_crear(strcmp, NULL);

    int n = 25;

    char *claves[] = {"5555", "2222", "8888", "6666", "5666", "7777", "7788", "7789", "9888", "9997", "9999", "1111", "110", "111", "1233", "1122", "1113", "1444", "1237", "1235", "1236", "1445", "1456", "3333", "4444"};
    char *valores[] = {"5555", "2222", "8888", "6666", "5666", "7777", "7788", "7789", "9888", "9997", "9999", "1111", "110", "111", "1233", "1122", "1113", "1444", "1237", "1235", "1236", "1445", "1456", "3333", "4444"};
    //Este vector ordenado contiene las claves odenadas de menor a mayor. El resultado del in-order debe ser igual al orden de este vector.
    char *ordenados[] = {"110", "111", "1111", "1113", "1122", "1233", "1235", "1236", "1237", "1444", "1445", "1456", "2222", "3333", "4444", "5555", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    char *ordenados1[] = {"110", "111", "1111", "1113", "1122", "1235", "1236", "1237", "1444", "1445", "1456", "2222", "3333", "4444", "5555", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    char *ordenados2[] = {"110", "111", "1111", "1113", "1122", "1235", "1236", "1237", "1444", "1445", "1456", "2222", "3333", "4444", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    char *ordenados3[] = {"110", "111", "1111", "1113", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "4444", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    char *ordenados4[] = {"110", "111", "1111", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "4444", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    char *ordenados5[] = {"110", "1111", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "4444", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997", "9999"};
    char *ordenados6[] = {"110", "1111", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "4444", "5666", "6666", "7777", "7788", "7789", "8888", "9888", "9997"};
    char *ordenados7[] = {"110", "1111", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "4444", "5666", "6666", "7777", "7788", "7789", "9888", "9997"};
    char *ordenados8[] = {"110", "1111", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "4444", "6666", "7777", "7788", "7789", "9888", "9997"};
    char *ordenados9[] = {"110", "1111", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "4444", "6666", "7777", "7788", "7789", "9997"};
    char *ordenados10[] = {"110", "1111", "1122", "1235", "1236", "1237", "1444", "1445", "2222", "3333", "6666", "7777", "7788", "7789", "9997"};

    /* Inserta n valores */
    int i = 0;
    bool ok = true;
    while ((i < n) && ok) {
        //inserto mismo char* clave / valor
        ok = abb_guardar(abb, claves[i], valores[i]);
        i++;
    }

    print_test("Se insertaron todos las duplas valor / clave", ok);
    print_test("Se insertaron 25 elementos en el arbol", 25 == i);

    cola_t* extra = cola_crear();

    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados);
    
    //Elimino raiz de subarbol izquierdo con dos nodos hijos.
    char* borrado = abb_borrar(abb, "1233");
    print_test("Se elimino nodo con clave 1233", strcmp(borrado, "1233") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados1);
    
    //Elimino nodo raiz
    borrado = abb_borrar(abb, "5555");
    print_test("Se elimino nodo con clave 5555", strcmp(borrado, "5555") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados2);
    
    //Elimino nodo hoja derecha
    borrado = abb_borrar(abb, "1456");
    print_test("Se elimino nodo con clave 1456", strcmp(borrado, "1456") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados3);
    
    //Elimino nodo hoja izquierda
    borrado = abb_borrar(abb, "1113");
    print_test("Se elimino nodo con clave 1113", strcmp(borrado, "1113") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados4);
    
    //Elimino hoja izquierda del menor elemento del arbol
    borrado = abb_borrar(abb, "111");
    print_test("Se elimino nodo con clave 111", strcmp(borrado, "111") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados5);
    
    //Elimino hoja derecha, mayor elemento del arbol.
    borrado = abb_borrar(abb, "9999");
    print_test("Se elimino nodo con clave 9999", strcmp(borrado, "9999") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados6);
    
    //Elimino root del subarbol derecho mayor.
    borrado = abb_borrar(abb, "8888");
    print_test("Se elimino nodo con clave 8888", strcmp(borrado, "8888") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados7);
    
    //Elimino hoja izquierda de nodo con dos hijos (menor elemento de subarbol derecho)
    borrado = abb_borrar(abb, "5666");
    print_test("Se elimino nodo con clave 5666", strcmp(borrado, "5666") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados8);
    
    //Vuelvo a eliminar root del subarbol derecho mayor, luego de un reemplazo anterior.
    borrado = abb_borrar(abb, "9888");
    print_test("Se elimino nodo con clave 9888", strcmp(borrado, "9888") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados9);
    
    //Vuelvo a eliminar raiz del arbol
    borrado = abb_borrar(abb, "4444");
    print_test("Se elimino nodo con clave 4444", strcmp(borrado, "4444") == 0);
    abb_in_order(abb, visitar_test, extra);
    prueba_abb_eliminar_hojas_aux(extra, ordenados10);

    cola_destruir(extra, free);
    abb_destruir(abb);

    abb = abb_crear(strcmp, NULL);
    ok = abb_guardar(abb, "3", "3");
    ok = abb_guardar(abb, "2", "2");
    ok = abb_guardar(abb, "1", "1");

    borrado = abb_borrar(abb, "3");
    borrado = abb_borrar(abb, "1");
    print_test("Se elimino nodo con clave 1", strcmp(borrado, "1") == 0);

    abb_destruir(abb);

    abb = abb_crear(strcmp, NULL);
    ok = abb_guardar(abb, "3", "3");
    ok = abb_guardar(abb, "4", "4");
    ok = abb_guardar(abb, "5", "5");

    borrado = abb_borrar(abb, "3");
    borrado = abb_borrar(abb, "5");
    print_test("Se elimino nodo con clave 5", strcmp(borrado, "5") == 0);

    abb_destruir(abb);
}


/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

 void pruebas_abb_alumno(void) {
 	/* Ejecuta todas las pruebas unitarias. */
    prueba_crear_abb_vacio();
    prueba_abb_insertar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_iterar_int_completo();
    prueba_abb_iterar_int_interrupt();
    prueba_abb_iterar_int_volumen(5000);
    prueba_abb_iterar_ext_vacio();
    prueba_abb_iterar_ext_volumen(5000);
    prueba_abb_eliminar_hojas();
 }