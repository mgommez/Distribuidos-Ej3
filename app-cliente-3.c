/* Este código se implementa como prueba del correcto funcionamiento de las funciones
 * set_value(), exist(), delete_key() y destroy() de la librería "claves" */

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

    // Definición de coordenadas de prueba
    struct Coord coord1 = {10, 20};
    struct Coord coord2 = {5, 8};
    struct Coord coord3 = {3, 6};

    // Definición de arrays de valores para los atributos V_value2
    double valores1[] = {1.1, 2.2, 3.3};
    double valores2[] = {4.4, 5.5};
    double valores3[] = {6.6, 7.7, 8.8, 9.9};

    // Inserción de valores en la lista mediante set_value()
    printf("Insertando clave 4...\n");
    result = set_value(4, "Ejemplo4", 3, valores1, coord1);
    if (result < 0) {
        printf("Error al insertar el elemento Clave-%d en la lista enlazada.\n", 4);
    }

    printf("Insertando clave 5...\n");
    result = set_value(5, "Ejemplo5", 2, valores2, coord2);
    if (result < 0) {
        printf("Error al insertar el elemento Clave-%d en la lista enlazada.\n", 5);
    }

    printf("Insertando clave 6...\n");
    result = set_value(6, "Ejemplo6", 4, valores3, coord3);
    if (result < 0) {
        printf("Error al insertar el elemento Clave-%d en la lista enlazada.\n", 6);
    }

    // Comprobación de la existencia de los elementos en la lista mediante exist()
    printf("\nVerificando existencia de claves antes de eliminar...\n");
    printf("¿Existe la clave 4? %s\n", exist(4) ? "Sí" : "No");
    printf("¿Existe la clave 5? %s\n", exist(5) ? "Sí" : "No");
    printf("¿Existe la clave 6? %s\n", exist(6) ? "Sí" : "No");
    printf("¿Existe la clave 7? %s\n", exist(7) ? "Sí" : "No");

    // Destrucción de la lista enlazada entera mediante destroy()
    printf("\nDestruyendo la lista...\n");
    destroy();

    // Verificación de la elimincación de todos los elementos
    printf("\nVerificando existencia de claves después de destruir...\n");
    printf("¿Existe la clave 4? %s\n", exist(4) ? "Sí" : "No");
    printf("¿Existe la clave 5? %s\n", exist(5) ? "Sí" : "No");
    printf("¿Existe la clave 6? %s\n", exist(6) ? "Sí" : "No");
    printf("¿Existe la clave 7? %s\n", exist(7) ? "Sí" : "No");

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

    // Comprobación de la inserción con set_value() mediante el uso de get_value()
    printf("\nLista enlazada actual:\n");
    struct tuple *temp = (struct tuple*)malloc(sizeof(struct tuple));
    int elem = 1;

    while (elem <= 3) {
        result = get_value(elem, temp->value1, &(temp->N_value2), temp->V_value2, &(temp->value3));
        if (result < 0) {
            printf("Error al recuperar el elemento Clave-%d de la lista enlazada.\n", elem);
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

    printf("\n-------------Fin del programa app-cliente-3!---------------\n");

    return 0;
}

