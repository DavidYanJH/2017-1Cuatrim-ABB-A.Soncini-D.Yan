#include "abb.h"
#include "testing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/
static void prueba_crear_abb_vacio(void)
{
    abb_t* abb = abb_crear(strcmp, free);

    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}



/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * *****************************************************************/

 void pruebas_abb_alumno(void) {
 	/* Ejecuta todas las pruebas unitarias. */
    prueba_crear_abb_vacio();
    //prueba_iterar_abb_vacio();
    //prueba_abb_insertar();
    //prueba_abb_reemplazar();
    //prueba_abb_reemplazar_con_destruir();
    //prueba_abb_borrar();
    //prueba_abb_clave_vacia();
    //prueba_abb_valor_null();
    //prueba_abb_volumen(5000, true);
    //prueba_abb_iterar();
    //prueba_abb_iterar_volumen(5000);
 }