//
// Created by crist on 04/03/2025.
//

#ifndef MESSAGE_H
#define MESSAGE_H
#define MAX_NAME_SIZE 256

struct set_value_peticion {
    int key;
    char *value1[256];
    int N_value2;
    double V_value2[32];
    struct Coord value3;
} ;

struct get_value_respuesta {
    int status;
    char *value1[256];
    int N_value2;
    double V_value2[32];
    struct Coord value3;
} ;


#endif //MESSAGE_H
