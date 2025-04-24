//
// Created by crist on 20/02/2025.
//Archivo para el almacenamiento y gestión de los datos.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "claves.h"
#include <pthread.h>

struct tuple {
    int key;
    char value1[256];
    int N_value2;
    double V_value2[32];
    struct Coord *value3;
    struct tuple *next;
};


int check_value1(char *value1){
    //Comprobación de longitud de value 1
    int i=0;
    int end=0;

    while (i<255 && end==0){
        if(value1[i] == '\0'){
            end=1;
        }
        i += 1;
    }

    if(end != 1){
        printf("Error en value1: mayor de 255 caracteres\n");
        return -1;
    }

    return 0;
}

int check_value2(int N_value2){
    //Comprobación de valor de value_2
    if (N_value2 > 32 || N_value2 < 1) {
        printf("Error en N_value2: N fuera de rango.\n");
        return -1;
    }

    return 0;
}

struct tuple *TUPLE = NULL;

//Declaración mecanismos de concurrencia
 pthread_mutex_t mutex_claves;

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    /* Función que localiza un elemento dada una key en la lista enlazada
    y guarda el contenido en las direcciones de memoria pasadas como argumentos.
    Bucle while comprobando la key hasta coincidencia.
    Si no se encuentra u otro error devolver -1, en caso de éxito devolver 0. */

    pthread_mutex_lock(&mutex_claves);
    // caso de lista enlazada vacía
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        pthread_mutex_unlock(&mutex_claves);
        return -1;
    }

    int find = 0;
    struct tuple *temp = TUPLE;

    while (find == 0) {
        if (temp->key == key) {
            find=1;
        } else if (temp->next == NULL) {
            printf("La clave no está en la lista.\n");
            pthread_mutex_unlock(&mutex_claves);
            return -1;
        } else {
            temp = temp->next;
        }
    }

    strcpy(value1, temp->value1);
    *N_value2 = temp->N_value2;
    memcpy(V_value2, temp->V_value2, *N_value2 * sizeof(double));
    *value3 = *(temp->value3);

    pthread_mutex_unlock(&mutex_claves);
    return 0;
}

int set_value (int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    /* Función que inserta un elemento al final de la lista enlazada, comprobando que no exista previamente.
    1. Recorrer lista, encontrar último elemento.
    2. Insertar cambiando next del último.
    En caso de error (que ya existiera, N_value2 fuera de rango, otro) devuelve -1, y en caso de éxito devuelve 0. */

    pthread_mutex_lock(&mutex_claves);

    // Comprobación de valores
    if ( check_value1(value1) < 0 || check_value2(N_value2) < 0) {
      	pthread_mutex_unlock(&mutex_claves);
        return -1;
    }


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
            pthread_mutex_unlock(&mutex_claves);

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

    pthread_mutex_unlock(&mutex_claves);
    return 0;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3){
    /* Modifica los valores asociados a una clave dada.
    Devuelve 0 en caso de éxito y -1 en caso de error (la clave no existe, N_value2 está fuera de rango, otro). */

    pthread_mutex_lock(&mutex_claves);

    // Comprobación de valores
    if ( check_value1(value1) < 0 || check_value2(N_value2) < 0) {
      	pthread_mutex_unlock(&mutex_claves);
        return -1;
    }


    // caso de lista vacía - error
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        pthread_mutex_unlock(&mutex_claves);
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
            pthread_mutex_unlock(&mutex_claves);
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

    pthread_mutex_unlock(&mutex_claves);
    return 0;
}

int exist(int key){
    /* Determina si existe un elemento en la lista con la clave dada.
     * Devuelve 1 si existe, 0 si no. */

    pthread_mutex_lock(&mutex_claves);
    // caso de lista vacía - error
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        pthread_mutex_unlock(&mutex_claves);
        return 0;
    }

    struct tuple *temp = TUPLE;

    while (1) {
        if (temp->key == key) {
          	pthread_mutex_unlock(&mutex_claves);
            return 1;
        }
        if (temp->next == NULL) {
            // caso de clave no existe - error
            printf("La clave no está en la lista.\n");
            pthread_mutex_unlock(&mutex_claves);
            return 0;
        }
        temp = temp->next;
    }
}

int delete_key(int key){
    /* Borra el elemento de la lista con la clave dada.
    Devuelve 0 en caso de éxito y -1 en caso de error (clave no existe, otro). */

    pthread_mutex_lock(&mutex_claves);
    // caso lista vacía - error
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        pthread_mutex_unlock(&mutex_claves);
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
            pthread_mutex_unlock(&mutex_claves);
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

    pthread_mutex_unlock(&mutex_claves);
    return 0;
}

int destroy(void){
    /* Elimina todas las tuplas almacenadas previamente.
    Devuelve 0 en caso de éxito y -1 en caso de error. */

    pthread_mutex_lock(&mutex_claves);
    if (TUPLE == NULL) {
        printf("Lista vacía.\n");
        pthread_mutex_unlock(&mutex_claves);
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

    pthread_mutex_unlock(&mutex_claves);
    return 0;
}
