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
    struct Coord coord_nueva = {270, 34581};

    // Definición de arrays de valores para los atributos V_value2
    double valores1[] = {1.1, 2.2, 3.3};
    double valores2[] = {4.4, 5.5};
    double valores3[] = {6.6, 7.7, 8.8, 9.9};
    double valores_nuevos[] = {0.0,0.1,0.2,0.3,0.4,0.5,
                               1.0,1.1,1.2,1.3,1.4,1.5,
                               2.0,2.1,2.2,2.3,2.4,2.5,
                               3.0,3.1,3.2,3.3,3.4,3.5,
                               4.0,4.1,4.2,4.3,4.4,4.5,
                               5.0};

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

    // Eliminación del elemento Clave-2 mediante delete_key()
    printf("\nEliminando clave 5...\n");
    result = delete_key(5);
    if (result == 0) {
        printf("Se ha borrado la clave 5 correctamente.\n");
    }
    if (result == -1) {
        printf("Se produjo un error al borrar la clave 5 de la lista enlazada.\n");
    }

    // Comprobación de la eliminación del elemento Clave-2 mediante exist()
    printf("\nVerificando existencia de claves después de eliminar...\n");
    printf("¿Existe la clave 4? %s\n", exist(4) ? "Sí" : "No");
    printf("¿Existe la clave 5? %s\n", exist(5) ? "Sí" : "No");
    printf("¿Existe la clave 6? %s\n", exist(6) ? "Sí" : "No");
    printf("¿Existe la clave 7? %s\n", exist(7) ? "Sí" : "No");

    // Comprobación de error de delete_key(): eliminar clave no existente en la estructura
    printf("\nEliminando clave 5...\n");
    result = delete_key(5);
    if (result == 0) {
        printf("Algo ha ido mal... se ha borrado la clave 5 a pesar de no estar en la estructura.\n");
    }
    if (result == -1) {
        printf("No se pudo eliminar la clave 5 de la lista enlazada porque ya no existe.\n");
    }

    // Destrucción de la lista enlazada entera mediante destroy()
    printf("\nDestruyendo la lista...\n");
    destroy();

    // Verificación de la elimincación de todos los elementos
    printf("\nVerificando existencia de claves después de destruir...\n");
    printf("¿Existe la clave 4? %s\n", exist(4) ? "Sí" : "No");
    printf("¿Existe la clave 5? %s\n", exist(5) ? "Sí" : "No");
    printf("¿Existe la clave 6? %s\n", exist(6) ? "Sí" : "No");
    printf("¿Existe la clave 7? %s\n", exist(7) ? "Sí" : "No");

    // Comprobación lista vacía
    printf("\nVerificando que la lista está vacía después de destruirla...\n");
    struct tuple *temp = (struct tuple*)malloc(sizeof(struct tuple));
    memset(temp, 0, sizeof(struct tuple));
    if (temp == NULL) {
        perror("Error en malloc en app-cliente");
        exit(-1);
    }
    int i = 4;
    while(i < 7) {
        result = get_value(i, temp->value1, &(temp->N_value2), temp->V_value2, &(temp->value3));
        if (result == 0) {
            printf("Algo ha ido mal... la lista no está vacía; sigue conteniendo la clave %d.\n", i);
            i = 8;
        }
        i++;
    }
    if (i==7) {
        printf("La lista está vacía.\n"
               "A continuación se comprueba si la lista existe introduciendo un nuevo elemento.\n");
        result = set_value(9999, "Primer valor en la nueva lista", 30, valores_nuevos, coord_nueva);
        if (result != 0) {
            printf("Algo ha ido mal... la lista no existe; no se pudo insertar la clave 9999.\n");
        } else {
            printf("Se ha insertado el elemento con clave 9999 correctamente en la lista.\n"
                   "La lista existe.\n");
        }

    }

    printf("\n------------Fin del programa app-cliente-2!-------------\n");

    return 0;
}

