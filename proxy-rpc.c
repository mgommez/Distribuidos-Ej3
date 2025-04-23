//
// Created by crist on 20/02/2025.
//

// Simula la API, pero en realidad envía mensajes al servidor mediante sockets.

#include "claves.h"
//#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include "claves_rpc.h"


int assigned_host = 0;
char *host;


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

int destroy(void){
    CLIENT *clnt;
    enum clnt_stat retval;
    int result;

    // Asignación de dirección IP de host si no está ya asignado
    if (assigned_host == 0) {
        host = getenv("IP_TUPLAS");
        if (host == NULL){
            printf("IP_TUPLAS no definida\n");
            return -1;
        }
        assigned_host = 1;
    }

    // 1. Localizar al servidor
    clnt = clnt_create (host, CLAVES, CLAVESVER, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        return -2; // error de comunicación
    }

    // 2. Invocar al procedimiento remoto
    retval = destroy_server_1(&result, clnt);
    if (retval != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        return -2;
    }

    // 3. Destruir
    clnt_destroy (clnt);

    if (result == -2) {
        perror("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    return result;
}


int set_value (int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    // Comprobación de valores
    if ( check_value1(value1) < 0 || check_value2(N_value2) < 0) {
        return -1;
    }

    // Creación de la estructura de petición
  	struct set_value_peticion *p = (struct set_value_peticion*)malloc(sizeof(struct set_value_peticion));
    if (p == NULL) {
        perror("Error en malloc de peticion - set_value proxy\n");
        return -1;
    }

    // Asignación de valores pasados en los argumentos
    p->key = key;
    strcpy(p->value1, value1);
    p->N_value2 = N_value2;
    memcpy(p->V_value2, V_value2, N_value2 * sizeof(double));
    p->value3.x = value3.x;
    p->value3.y = value3.y;

    CLIENT *clnt;
    enum clnt_stat retval;
    int result;

    // Asignación de dirección IP de host si no está ya asignado
    if (assigned_host == 0) {
        host = getenv("IP_TUPLAS");
        if (host == NULL){
            printf("IP_TUPLAS no definida\n");
            return -1;
        }
        assigned_host = 1;
    }

    // 1. Localizar al servidor
    clnt = clnt_create (host, CLAVES, CLAVESVER, "udp");
    if (clnt == NULL) {
        free(p);
        clnt_pcreateerror (host);
        return -2; // error de comunicación
    }

    // 2. Invocar al procedimiento remoto
    retval = set_value_server_1(*p, &result, clnt);
    if (retval != RPC_SUCCESS) {
        free(p);
        clnt_perror (clnt, "call failed");
        return -2;
    }

    // 3. Destruir
    clnt_destroy (clnt);

    if (result == -2) {
        free(p);
        perror("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    free(p);

    return result;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {
    // Creación de la estructura de petición
  	struct get_value_respuesta result;

    CLIENT *clnt;
    enum clnt_stat retval;

    // Asignación de dirección IP de host si no está ya asignado
    if (assigned_host == 0) {
        host = getenv("IP_TUPLAS");
        if (host == NULL){
            printf("IP_TUPLAS no definida\n");
            return -1;
        }
        assigned_host = 1;
    }

    // 1. Localizar al servidor
    clnt = clnt_create (host, CLAVES, CLAVESVER, "udp");
    if (clnt == NULL) {
        free(result);
        clnt_pcreateerror (host);
        return -2; // error de comunicación
    }

    // 2. Invocar al procedimiento remoto
    retval = get_value_server_1(key, &result, clnt);
    if (retval != RPC_SUCCESS) {
        free(result);
        clnt_perror (clnt, "call failed");
    }

    // 3. Destruir
    clnt_destroy (clnt);

    if (result->status == -2) {
        free(result);
        perror("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    // Guardar los valores recibidos en result en las direcciones de memoria pasadas como argumentos
    if (result->status == 0) {
        strcpy(value1, result->value1);
        *N_value2 = result->N_value2;
        memcpy(V_value2, result->V_value2, *N_value2 * sizeof(double));
        value3->x = result->value3.x;
        value3->y = result->value3.y;
    }

    return result.status;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    // Comprobación de valores
    if ( check_value1(value1) < 0 || check_value2(N_value2) < 0) {
        return -1;
    }

    // Creación de la estructura de petición
    struct set_value_peticion *p = (struct set_value_peticion*)malloc(sizeof(struct set_value_peticion));
    if (p == NULL) {
        perror("Error en malloc de peticion - set_value proxy\n");
        return -1;
    }

    // Asignación de valores pasados en los argumentos
    p->key = key;
    strcpy(p->value1, value1);
    p->N_value2 = N_value2;
    memcpy(p->V_value2, V_value2, N_value2 * sizeof(double));
    p->value3.x = value3.x;
    p->value3.y = value3.y;

    CLIENT *clnt;
    enum clnt_stat retval;
    int result;

    // Asignación de dirección IP de host si no está ya asignado
    if (assigned_host == 0) {
        host = getenv("IP_TUPLAS");
        if (host == NULL){
            printf("IP_TUPLAS no definida\n");
            return -1;
        }
        assigned_host = 1;
    }

    // 1. Localizar al servidor
    clnt = clnt_create (host, CLAVES, CLAVESVER, "udp");
    if (clnt == NULL) {
        free(p);
        clnt_pcreateerror (host);
        return -2; // error de comunicación
    }

    // 2. Invocar al procedimiento remoto
    retval = modify_value_server_1(*p, &result, clnt);
    if (retval != RPC_SUCCESS) {
        free(p);
        clnt_perror (clnt, "call failed");
        return -2;
    }

    // 3. Destruir
    clnt_destroy (clnt);

    if (result == -2) {
        free(p);
        perror("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    free(p);

    return result;
}


int delete_key(int key) {
    CLIENT *clnt;
    enum clnt_stat retval;
    int result;

    // Asignación de dirección IP de host si no está ya asignado
    if (assigned_host == 0) {
        host = getenv("IP_TUPLAS");
        if (host == NULL){
            printf("IP_TUPLAS no definida\n");
            return -1;
        }
        assigned_host = 1;
    }

    // 1. Localizar al servidor
    clnt = clnt_create (host, CLAVES, CLAVESVER, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        return -2; // error de comunicación
    }

    // 2. Invocar al procedimiento remoto
    retval = delete_key_server_1(key, &result, clnt);
    if (retval != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        return -2;
    }

    // 3. Destruir
    clnt_destroy (clnt);

    if (result == -2) {
        perror("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    return result;
}


int exist(int key) {
    CLIENT *clnt;
    enum clnt_stat retval;
    int result;

    // Asignación de dirección IP de host si no está ya asignado
    if (assigned_host == 0) {
        host = getenv("IP_TUPLAS");
        if (host == NULL){
            printf("IP_TUPLAS no definida\n");
            return -1;
        }
        assigned_host = 1;
    }

    // 1. Localizar al servidor
    clnt = clnt_create (host, CLAVES, CLAVESVER, "udp");
    if (clnt == NULL) {
        clnt_pcreateerror (host);
        return -2; // error de comunicación
    }

    // 2. Invocar al procedimiento remoto
    retval = exist_server_1(key, &result, clnt);
    if (retval != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
        return -2;
    }

    // 3. Destruir
    clnt_destroy (clnt);

    if (result == -2) {
        perror("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    return result;
}


