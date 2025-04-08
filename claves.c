//
// Created by crist on 20/02/2025.
//Archivo para el almacenamiento y gestión de los datos.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "claves.h"

struct tuple {
    int key;
    char value1[256];
    int N_value2;
    double V_value2[32];
    struct Coord *value3;
    struct tuple *next;
};

struct tuple *TUPLE = NULL;

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    /* Función que localiza un elemento dada una key en la lista enlazada
    y guarda el contenido en las direcciones de memoria pasadas como argumentos.
    Bucle while comprobando la key hasta coincidencia.
    Si no se encuentra u otro error devolver -1, en caso de éxito devolver 0. */

    // caso de lista enlazada vacía
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        return -1;
    }

    int find = 0;
    struct tuple *temp = TUPLE;

    while (find == 0) {
        if (temp->key == key) {
            find=1;
        } else if (temp->next == NULL) {
            printf("La clave no está en la lista.\n");
            return -1;
        } else {
            temp = temp->next;
        }
    }

    strcpy(value1, temp->value1);
    *N_value2 = temp->N_value2;
    memcpy(V_value2, temp->V_value2, *N_value2 * sizeof(double));
    *value3 = *(temp->value3);

    return 0;
}

int set_value (int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    /* Función que inserta un elemento al final de la lista enlazada, comprobando que no exista previamente.
    1. Recorrer lista, encontrar último elemento.
    2. Insertar cambiando next del último.
    En caso de error (que ya existiera, N_value2 fuera de rango, otro) devuelve -1, y en caso de éxito devuelve 0. */

    int empty = 0;
    int find = 0;

    // caso de lista enlazada vacía
    if (NULL == TUPLE) {
        empty = 1;
        find = 1;
    }

    struct tuple *temp = TUPLE;

    //Bucle para recorrer la lista enlazada
    while (find == 0) {
        if (temp == NULL) break;
        // caso de clave ya en la lista
        if (temp->key == key) {
            printf("La clave %d ya se encuentra en la lista.\n", key);
            return -1;
        }
        if (temp->next == NULL) {
            find = 1;
        } else {
            temp = temp->next;
        }
    }

    struct tuple *new_elem = (struct tuple*)malloc(sizeof(struct tuple));
    new_elem->key = key;
    strcpy(new_elem->value1, value1);
    new_elem->N_value2 = N_value2;
    memcpy(new_elem->V_value2, V_value2, N_value2 * sizeof(double));

    new_elem->value3 = (struct Coord*) malloc(sizeof(struct Coord));
    *(new_elem->value3) = value3;

    if (empty) {
        TUPLE = new_elem;
    } else {
        // insertar el nuevo elemento al inicio de la lista enlazada
        temp->next = new_elem;
    }
    new_elem->next = NULL;

    return 0;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    /* Modifica los valores asociados a una clave dada.
    Devuelve 0 en caso de éxito y -1 en caso de error (la clave no existe, N_value2 está fuera de rango, otro). */

    // caso de lista vacía - error
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        return -1;
    }

    int find = 0;
    struct tuple *temp = TUPLE;

    while (find == 0) {
        if (temp->key == key) {
            find=1;
        } else if (temp->next == NULL) {
            // caso de clave no encontrada - error
            printf("La clave no está en la lista.\n");
            return -1;
        } else {
            temp = temp->next;
        }
    }

    strcpy(temp->value1, value1);
    temp->N_value2 = N_value2;
    memcpy(temp->V_value2, V_value2, N_value2 * sizeof(double));

    //temp->value3 = (struct Coord*) malloc(sizeof(struct Coord));
    *(temp->value3) = value3;

    return 0;
}

int exist(int key){
    /* Determina si existe un elemento en la lista con la clave dada.
     * Devuelve 1 si existe, 0 si no. */

    // caso de lista vacía - error
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        return 0;
    }

    struct tuple *temp = TUPLE;

    while (1) {
        if (temp->key == key) {
            return 1;
        }
        if (temp->next == NULL) {
            // caso de clave no existe - error
            printf("La clave no está en la lista.\n");
            return 0;
        }
        temp = temp->next;
    }
}

int delete_key(int key){
    /* Borra el elemento de la lista con la clave dada.
    Devuelve 0 en caso de éxito y -1 en caso de error (clave no existe, otro). */

    // caso lista vacía - error
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        return -1;
    }

    int find = 0;
    struct tuple *temp = TUPLE;
    struct tuple *prev = NULL;

    while (find == 0) {
        if (temp->key == key) {
            find=1;
        } else if (temp->next == NULL) {
            // caso clave no existe - error
            printf("La clave no está en la lista.\n");
            return -1;
        } else {
            prev = temp;
            temp = temp->next;
        }
    }

    // caso de que se borre el primer elemento
    if (prev == NULL) {
        TUPLE = temp->next;
    } else {
        prev->next = temp->next;
    }

    // liberar memoria dinámica
    free(temp->value3);
    free(temp);

    return 0;
}

int destroy(void){
    /* Elimina todas las tuplas almacenadas previamente.
    Devuelve 0 en caso de éxito y -1 en caso de error. */

    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        return 0;
    }

    struct tuple *temp = TUPLE;
    struct tuple *next = TUPLE->next;

    while (temp != NULL) {
        next = temp->next;  // Guarda el siguiente antes de liberar
        free(temp->value3);
        free(temp);
        temp = next;  // Mueve al siguiente nodo
    }
    TUPLE = NULL;
    return 0;
}
