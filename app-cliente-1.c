/* Este código se implementa como prueba del correcto funcionamiento de las funciones
 * set_value(), get_value() y modify_value() de la librería "claves" */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "claves.h"


struct tuple {
    int key;
    char value1[256];
    int N_value2;
    double V_value2[32];
    struct Coord value3;
    struct tuple *next;
};

int main() {
    int result;

    // Definición de algunas coordenadas
    struct Coord coord1 = {10, 20};
    struct Coord coord2 = {5, 8};
    struct Coord coord3 = {30, 15};

    // Definición de arrays de valores para los atributos V_value2
    double valores1[] = {1.1, 2.2, 3.3};
    double valores2[] = {4.4, 5.5};
    double valores3[] = {6.6, 7.7, 8.8, 9.9};

    // Definición de cadenas de texto para value1
    char cadena_larga[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()-_=+[]{}|;:'aa,.<>?/aa`~x01x02x03\"x04x14x15x1Cx1Dx1Eaaaaaaaaaaaaaaaaaaaaax1FMore random text to ensure we exceed 255 characters.bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbpppppppppppppppppppppppppp";


    // Inserción de valores en la lista con set_value()
    printf("Insertando clave 1...\n");
    result = set_value(1, "Ejemplo1", 3, valores1, coord1);
    if (result < 0) {
        printf("Error al insertar el elemento Clave-%d en la lista enlazada.\n", 1);
    }

    printf("Insertando clave 2...\n");
    result = set_value(2, "Ejemplo2", 2, valores2, coord2);
    if (result < 0) {
        printf("Error al insertar el elemento Clave-%d en la lista enlazada.\n", 2);
    }

    printf("Insertando clave 3...\n");
    result = set_value(3, "Ejemplo3", 4, valores3, coord3);
    if (result < 0) {
        printf("Error al insertar el elemento Clave-%d en la lista enlazada.\n", 3);
    }

    // Intento de inserción de una clave duplicada
    printf("\nCOMPROBACIONES DE ERROR DE SET_VALUE()\n");
    printf("\nIntentando insertar clave duplicada 2...\n");
    result = set_value(2, "Duplicado", 2, valores2, coord2);
    if (result < 0) {
        printf("Error al insertar un valor duplicado en la lista enlazada.\n");
    }

    // Intento de insertar clave con value1 incorrecto
    printf("Intentando insertar clave 2 (value1 con más de 255 caracteres)...\n");
    result = set_value(2, cadena_larga, 2, valores2, coord2);
    if (result < 0) {
        printf("Error al insertar un valor con value1 incorrecto en la lista enlazada.\n");
    }

    // Intento de insertar clave con N_value2 incorrecto
    printf("Intentando insertar clave 2 (N_value2 = 0)...\n");
    result = set_value(2, "N_value2 incorrecto", 0, valores2, coord2);
    if (result < 0) {
        printf("Error al insertar un valor con N_value2 incorrecto en la lista enlazada.\n");
    }

    // Intento de insertar clave con N_value2 incorrecto
    printf("Intentando insertar clave 2 (N_value2 = 34)...\n");
    result = set_value(2, "N_value2 incorrecto", 34, valores2, coord2);
    if (result < 0) {
        printf("Error al insertar un valor con N_value2 incorrecto en la lista enlazada.\n");
    }

    // Comprobación de la inserción con set_value() mediante el uso de get_value()
    // Comprobación de recuperación de valores con get_value()
    printf("\nLista enlazada actual:\n");
    struct tuple *temp = (struct tuple*)malloc(sizeof(struct tuple));
    memset(temp, 0, sizeof(struct tuple));
    if (temp == NULL) {
        perror("Error en malloc en app cliente");
        exit(-1);
    }
    int elem = 1;

    while (elem <= 3) {
        result = get_value(elem, temp->value1, &(temp->N_value2), temp->V_value2, &(temp->value3));
        if (result < 0) {
            printf("Error al recuperar el elemento Clave-%d de la lista enlazada.\n", elem);
            free(temp);
            return -1;
        }

        temp->key = elem;
        printf("Clave: %d, Value1: %s, N_value2: %d, Coord: (%d, %d), ",
               temp->key, temp->value1, temp->N_value2, temp->value3.x, temp->value3.y);
        printf("V_value2: [");
        for (int i=0; i < temp->N_value2; i++) {
            if (i == (temp->N_value2-1)) {
                printf("%.1f]\n", temp->V_value2[i]);
            } else {
                printf("%.1f, ", temp->V_value2[i]);
            }
        }

        elem++;
    }

    printf("\nCOMPROBACIONES DE ERROR DE GET_VALUE()\n");
    // Comprobación de error al tratar de recuperar un valor inexistente con get_value()
    printf("\nIntentando recuperar el valor con Clave-%d (no insertado; inexistente)...\n", 404);
    result = get_value(404, temp->value1, &(temp->N_value2), temp->V_value2, &(temp->value3));
    if (result < 0) {
        printf("Error al recuperar el elemento Clave-%d de la lista enlazada.\n", 404);
    }


    // Modificación de la clave 2 mediante modify_value()
    struct Coord nueva_coord = {50, 60};
    double nuevos_valores[] = {9.9, 8.8, 7.7};

    printf("\nModificando clave 2...\n");
    result = modify_value(2, "Modificado", 3, nuevos_valores, nueva_coord);
    if (result == 0) {
        printf("Elemento Clave-2 modificado con éxito.\n");
    } else {
        printf("Error al modificar el elemento clave-2. Ha devuelto %d\n", result);
    }

    // Para comprobar el resultado de modify_value(),
    // se recupera el elemento Clave-2 en temp mediante get_value()
    temp->key = 2;

    printf("\nRecuperando elemento Clave-2 tras su modificación...\n");
    result = get_value(temp->key, temp->value1, &(temp->N_value2), temp->V_value2, &(temp->value3));
    if (result == 0) {
        printf("Clave: %d, Value1: %s, N_value2: %d, Coord: (%d, %d), ",
               temp->key, temp->value1, temp->N_value2, temp->value3.x, temp->value3.y);
        printf("V_value2: [");
        for (int i=0; i < temp->N_value2; i++) {
            if (i == (temp->N_value2-1)) {
                printf("%.1f]\n", temp->V_value2[i]);
            } else {
                printf("%.1f, ", temp->V_value2[i]);
            }
        }
    } else {
        printf("Elemento Clave-%d no encontrado.\n", temp->key);
    }

    printf("\nCOMPROBACIONES DE ERROR DE MODIFY_VALUE()\n");

    // Comprobación de errores en modify_value(): intento de modificar una clave inexistente
    printf("\nIntentando modificar clave 99...\n");
    if (modify_value(404, "NoExiste", 2, valores1, coord1) == -1) {
        printf("No se pudo modificar la clave 99 porque no existe.\n");
    }

    // Comprobación de errores en modify_value(): intento de modificar una clave con atributos incorrectos
    printf("\nIntentando modificar clave 2 (con valores incorrectos de N_value2)...\n");
    result = modify_value(2, "Incorrecta", 33, valores1, coord1);
    if (result == -1) {
        printf("No se pudo modificar la clave 2 porque no tiene valores correctos (N_value2).\n");
    }

    printf("\nIntentando modificar clave 2 (con valores incorrectos de value1)...\n");
    result = modify_value(2, cadena_larga, 2, valores1, coord1);
    if (result < 0) {
        printf("No se pudo modificar la clave 2 porque no tiene valores correctos(value1).\n");
    }

    printf("\n----------Fin del programa app-cliente-1!-----------\n");
    free(temp);
    return 0;
}
