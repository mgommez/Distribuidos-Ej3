//
// Created by crist on 20/02/2025.
//

// Simula la API, pero en realidad envía mensajes al servidor mediante sockets.

#include "claves.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include "comm.h"


int assigned_host = 0;
int assigned_port = 0;
char *host;
char *port;

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

int send_recv (struct peticion *p, struct respuesta *r) {
    int ret;
    char buffer[350];
    char *endptr;

    //Inicialización del socket
    int sd_client;


    //Lectura de las variables de entorno

    if (assigned_host == 0)
    {
        host = getenv("IP_TUPLAS");
        if (host == NULL){
            printf("IP_TUPLAS no definida\n");
            return -1;
        }
        assigned_host = 1;
    }

    if (assigned_port == 0)
    {
        port = getenv("PORT_TUPLAS");
        if (port == NULL){
            printf("PORT_TUPLAS no definida\n");
            return -1;
        }
        assigned_port = 1;
    }
    if (host == NULL || port == NULL)
    {
        printf("Variables de entorno no asignadas. Abortando operación...\n");
        return -1;
    }

    //Abrir socket del cliente
    long port_num = strtol(port, &endptr, 10);
    if (*endptr != '\0' || port_num <= 0 || port_num > 65535) {
        perror("Error: puerto inválido.\n");
        return -1;
    }

    sd_client = clientSocket(host,(int)port_num);
    if (sd_client < 0 ) {
        perror("Error al abrir clientSocket\n");
        return -1;
    }

    //Manda el código de operación al servidor
    sprintf(buffer, "%d", p->op);

    ret = writeLine(sd_client, (char *)buffer);
    if (ret < 0) {
        perror("Error en writeLine del servidor\n");
        closeSocket(sd_client);
        return -2;
    }
    memset(buffer, '\0', sizeof(buffer));

    switch(p->op) {

        // No se envía nada más en el caso 0 (destroy())
        case 1:
            //Envío de parámetros de la función set_value()
            //Envío de key
            sprintf(buffer, "%d", p->key);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a set_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            //Envío value1
            ret = writeLine(sd_client, p->value1);
            if (ret < 0) {
                perror("Error al escribir información relativa a set_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            //Envío N_value2
            sprintf(buffer, "%d", p->N_value2);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a set_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            //Envío V_value2
            for (int i = 0; i < p->N_value2; i++) {
                sprintf(buffer, "%.1f", p->V_value2[i]);
                ret = writeLine(sd_client, (char *)buffer);
                if (ret < 0) {
                    perror("Error al escribir información relativa a set_value()\n");
                    closeSocket(sd_client);
                    return -2;
                }
                memset(buffer, '\0', sizeof(buffer));
            }

            //Envío value3
            sprintf(buffer, "%d", p->value3.x);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a set_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            sprintf(buffer, "%d", p->value3.y);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a set_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));


            // Lectura del resultado
            ret = readLine(sd_client, (char *)buffer, sizeof(buffer));
            if (ret < 0) {
                perror("Error al leer el resultado\n");
                closeSocket(sd_client);
                return -2;
            }

            r->status = strtol(buffer, &endptr, 10) ;
            if (endptr[0] != '\0') {
                printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                closeSocket(sd_client);
                return -1 ;
            }
            memset(buffer, '\0', sizeof(buffer));

            break;

        case 2:
            // Envío de parámetros de la función get_value()
            // Envío de key
            sprintf(buffer, "%d", p->key);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a get_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            // Lectura de la respuesta
            // Lectura del resultado de la operación
            ret = readLine(sd_client, (char *)buffer, sizeof(buffer));
            if (ret < 0) {
                perror("Error al leer el resultado\n");
                closeSocket(sd_client);
                return -2;
            }

            r->status = strtol(buffer, &endptr, 10) ;
            if (endptr[0] != '\0') {
                printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                closeSocket(sd_client);
                return -1 ;
            }
            memset(buffer, '\0', sizeof(buffer));

            // Asignación del resto de valores sólo si la operación fue exitosa
            if (r->status == 0){
                // Lectura y asignación de value1
                ret = readLine(sd_client, r->value1, sizeof(buffer));
                if (ret < 0) {
                    perror("Error al leer parámetro del cliente: value1\n");
                    closeSocket(sd_client);
                    return -2;
                }

                // Lectura y asignación de N_value2
                ret = readLine(sd_client, (char *)buffer, sizeof(buffer)); // 2 caracteres (32 máx.) + 1 (fin de cadena)
                if (ret < 0) {
                    perror("Error al leer parámetro del cliente: N_value2\n");
                    closeSocket(sd_client);
                    return -2;
                }

                r->N_value2 = strtol(buffer, &endptr, 10) ;
                if (endptr[0] != '\0') {
                    printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                    closeSocket(sd_client);
                    return -1 ;
                }
                memset(buffer, '\0', sizeof(buffer));

                // Lectura y asignación de V_value2
                for (int i = 0; i < r->N_value2; i++) {
                    ret = readLine(sd_client, (char*)buffer, sizeof(buffer)); // 30 caracteres por double como máximo
                    if (ret < 0) {
                        perror("Error al leer parámetro del cliente: V_value2\n");
                        closeSocket(sd_client);
                        return -2;
                    }
                    r->V_value2[i] = strtod(buffer, NULL);
                    memset(buffer, '\0', sizeof(buffer));
                }

                // Lectura y asignación de value3
                ret = readLine(sd_client, (char *)buffer, sizeof(buffer));
                if (ret < 0) {
                    perror("Error al leer parámetro del cliente: value3.x\n");
                    closeSocket(sd_client);
                    return -2;
                }
                r->value3.x = strtol(buffer, &endptr, 10) ;
                if (endptr[0] != '\0') {
                    printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                    closeSocket(sd_client);
                    return -1 ;
                }
                memset(buffer, '\0', sizeof(buffer));

                ret = readLine(sd_client, (char *)buffer, sizeof(buffer));
                if (ret < 0) {
                    perror("Error al leer parámetro del cliente: value3.y\n");
                    closeSocket(sd_client);
                    return -2;
                }
                r->value3.y = strtol(buffer, &endptr, 10) ;
                if (endptr[0] != '\0') {
                    printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                    closeSocket(sd_client);
                    return -1 ;
                }
                memset(buffer, '\0', sizeof(buffer));
            }

            break;
        case 3:
            //Envío de parámetros de modify value()
            //Envío de key
            sprintf(buffer, "%d", p->key);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a modify_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            //Envío value1
            ret = writeLine(sd_client, p->value1);
            if (ret < 0) {
                perror("Error al escribir información relativa a modify_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            //Envío N_value2
            sprintf(buffer, "%d", p->N_value2);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a modify_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            //Envío V_value2
            for (int i = 0; i < p->N_value2; i++) {
                sprintf(buffer, "%.1f", p->V_value2[i]);
                ret = writeLine(sd_client, (char *)buffer);
                if (ret < 0) {
                    perror("Error al escribir información relativa a modify_value()\n");
                    closeSocket(sd_client);
                    return -2;
                }
                memset(buffer, '\0', sizeof(buffer));
            }

            //Envío value3
            sprintf(buffer, "%d", p->value3.x);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a modify_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            sprintf(buffer, "%d", p->value3.y);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a modify_value()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));


            // Lectura del resultado
            ret = readLine(sd_client, (char *)buffer, sizeof(buffer));
            if (ret < 0) {
                perror("Error al leer parámetro del cliente: key\n");
                closeSocket(sd_client);
                return -2;
            }
            r->status = strtol(buffer, &endptr, 10) ;
            if (endptr[0] != '\0') {
                printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                closeSocket(sd_client);
                return -1;
            }
            memset(buffer, '\0', sizeof(buffer));
            break;
        case 4:
            //Envío de parámetros de delete_key()
            //Envío de key
            sprintf(buffer, "%d", p->key);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a delete_key()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            // Lectura del resultado
            ret = readLine(sd_client, (char *)buffer, sizeof(buffer));
            if (ret < 0) {
                perror("Error al leer parámetro del cliente: key\n");
                closeSocket(sd_client);
                return -2;
            }
            r->status = strtol(buffer, &endptr, 10) ;
            if (endptr[0] != '\0') {
                printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                closeSocket(sd_client);
                return -1;
            }
            memset(buffer, '\0', sizeof(buffer));

            break;
        case 5:
            //Envío de parámetros de exist()
            //Envío de key
            sprintf(buffer, "%d", p->key);
            ret = writeLine(sd_client, (char *)buffer);
            if (ret < 0) {
                perror("Error al escribir información relativa a exist()\n");
                closeSocket(sd_client);
                return -2;
            }
            memset(buffer, '\0', sizeof(buffer));

            // Lectura del resultado
            ret = readLine(sd_client, (char *)buffer, sizeof(buffer));
            if (ret < 0) {
                perror("Error al leer parámetro del cliente: key\n");
                closeSocket(sd_client);
                return -2;
            }
            r->status = strtol(buffer, &endptr, 10) ;
            if (endptr[0] != '\0') {
                printf("Error: %s no es un número en base %d\n", buffer, 10) ;
                closeSocket(sd_client);
                return -1;
            }
            memset(buffer, '\0', sizeof(buffer));

            break;
    }
    closeSocket(sd_client);
    return r->status;
}

int destroy(void){
  	//Creación de la estructura de petición
  	struct peticion *p = (struct peticion*)malloc(sizeof(struct peticion));
    if (p == NULL) {
        perror("Error en malloc de peticion - destroy proxy\n");
        return -1;
    }
    p->op = 0;

    //Creación de la estructura de respuesta
    struct respuesta *r = (struct respuesta*)malloc(sizeof(struct respuesta));
    if (r == NULL) {
        perror("Error en malloc de respuesta - destroy proxy\n");
        free(p);
        return -1;
    }

    //Llamada a send-receive
    r->status = send_recv(p, r);
    int status = r->status;
    free(p);
    free(r);
    if (status == -2) {
        perror("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }
    return status;
}


int set_value (int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    //Comprobación de valores
    if ( check_value1(value1) < 0 || check_value2(N_value2) < 0) {
        return -1;
    }

  	//Creación de la estructura de petición
  	struct peticion *p = (struct peticion*)malloc(sizeof(struct peticion));
    if (p == NULL) {
        perror("Error en malloc de peticion - set_value proxy\n");
        return -1;
    }

    p->op= 1;
    p->key = key;
    strcpy(p->value1, value1);
    p->N_value2 = N_value2;
    memcpy(p->V_value2, V_value2, N_value2 * sizeof(double));
    p->value3 = value3;

    //Creación de la estructura de respuesta
    struct respuesta *r = (struct respuesta*)malloc(sizeof(struct respuesta));
    if (r == NULL) {
        perror("Error en malloc de respuesta - set_value proxy\n");
        free(p);
        return -1;
    }

    //Llamada a send-receive
    r->status = send_recv(p, r);

    //liberar estructuras y devolver
    int status = r-> status;
    free(p);
    free(r);

    if (status == -2) {
        printf("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    return status;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2, struct Coord *value3) {

    //Creación de la estructura de petición
  	struct peticion *p = (struct peticion*)malloc(sizeof(struct peticion));
    if (NULL == p) {
        perror("Error en malloc de peticion - get_value proxy\n");
        return -1;
    }

    p->op= 2;
    p->key = key;

    //Creación de la estructura de respuesta
    struct respuesta *r = (struct respuesta*)malloc(sizeof(struct respuesta));
    if (NULL == r) {
        perror("Error en malloc de respuesta - get_value proxy\n");
        free(p);
        return -1;
    }

    //Llamada a send-receive
    r->status = send_recv(p, r);

    //liberar estructuras y devolver
    int status = r->status;
    free(p);

    if (status == -2) {
        free(r);
        printf("Error de comunicación: retorno -2 a cliente\n");
        return -2;
    }

    if (status == 0) {
        strcpy(value1, r->value1);
        *N_value2 = r->N_value2;
        memcpy(V_value2, r->V_value2, *N_value2 * sizeof(double));
        *value3 = r->value3;
    }

	free(r);
    return status;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2, struct Coord value3) {
    //Comprobación de valores
    if ( check_value1(value1) < 0 || check_value2(N_value2) < 0) {
        return -1;
    }

  	//Creación de la estructura de petición
  	struct peticion *p = (struct peticion*)malloc(sizeof(struct peticion));
    if (NULL == p) {
        perror("Error en malloc de peticion - modify_value proxy\n");
        return -1;
    }

    p-> op= 3;
    p-> key = key;
    strcpy(p->value1, value1);
    p-> N_value2 = N_value2;
    memcpy(p->V_value2, V_value2, N_value2 * sizeof(double));
    p-> value3 = value3;

    //Creación de la estructura de respuesta
    struct respuesta *r = (struct respuesta*)malloc(sizeof(struct respuesta));
    if (NULL == r) {
        perror("Error en malloc de respuesta - modify_value proxy\n");
        free(p);
        return -1;
    }

    //Llamada a send-receive
    r-> status = send_recv(p, r);

    //liberar estructuras y devolver
    int status = r->status;
    free(p);
    free(r);

    if (status == -2) {
        printf("Error de comunicación: retorno -2 a cliente\n");
        return(-2);
    }
    return status;
}


int delete_key(int key) {
    //Creación de la estructura de petición
  	struct peticion *p = (struct peticion*)malloc(sizeof(struct peticion));
    if (NULL == p) {
        perror("Error en malloc de peticion - delete_key proxy\n");
        return -1;
    }

    p-> op= 4;
    p-> key = key;

    //Creación de la estructura de respuesta
    struct respuesta *r = (struct respuesta*)malloc(sizeof(struct respuesta));
    if (NULL == r) {
        perror("Error en malloc de respuesta - delete_key proxy\n");
        free(p);
        return -1;
    }

    //Llamada a send-receive
    r->status = send_recv(p, r);

    //liberar estructuras y devolver
    int status = r->status;
    free(p);
    free(r);

    if (status == -2) {
        printf("Error de comunicación: retorno -2 a cliente\n");
        return(-2);
    }
    return status;
}


int exist(int key) {
    struct peticion *p = (struct peticion*)malloc(sizeof(struct peticion));
    if (NULL == p) {
        perror("Error en malloc de peticion - exist proxy\n");
        return -1;
    }

    p-> op= 5;
    p-> key = key;

    //Creación de la estructura de respuesta
    struct respuesta *r = (struct respuesta*)malloc(sizeof(struct respuesta));
    if (NULL == r) {
        perror("Error en malloc de respuesta - exist proxy\n");
        free(p);
        return -1;
    }

    //Llamada a send-receive
    r->status = send_recv(p, r);

    //liberar estructuras y devolver
    int status = r-> status;
    free(p);
    free(r);

    if (status == -2) {
        printf("Error de comunicación: retorno -2 a cliente\n");
        return(-2);
    }
    return status;
}


