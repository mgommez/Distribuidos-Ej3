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


int main (int argc, char **argv) {
    int resultado;
    double valores_validos[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    struct Coord coord1 = {10, 20};

    printf("\n=== CASO 1: Modificar en una lista vacía ===\n");
    resultado = modify_value(1, "NuevoValor", 3, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== Agregando una tupla con clave 1 ===\n");
    set_value(1, "ValorInicialConValue1ExtremadaEInnecesariamenteLargoParaProbar",
              5, valores_validos, coord1);
    printf("Tupla agregada con clave 1.\n");

    printf("\n=== CASO 2: Modificar una tupla existente (Éxito) con value1 menor ===\n");
    resultado = modify_value(1, "Modificado", 3, valores_validos, coord1);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);

    printf("\n=== CASO 3: Modificar una clave inexistente ===\n");
    resultado = modify_value(99, "NoExiste", 2, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== CASO 4: Modificar con N_value2 fuera de rango (N=0) ===\n");
    resultado = modify_value(1, "ValorIncorrecto", 0, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== CASO 5: Modificar con N_value2 fuera de rango (N=33) ===\n");
    resultado = modify_value(1, "ValorIncorrecto", 33, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    //Impresión del elemento de la lista
    struct tuple *temp = (struct tuple*)malloc(sizeof(struct tuple));
    int result;
    memset(temp, 0, sizeof(struct tuple));
    if (temp == NULL) {
        perror("Error en malloc en app cliente");
        exit(-1);
    }

    result = get_value(1, temp->value1, &(temp->N_value2), temp->V_value2, &(temp->value3));
    if (result < 0) {
        printf("Error al recuperar el elemento Clave-%d de la lista enlazada.\n", 1);
        free(temp);
        return -1;
    }
    temp->key = 1;
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

    return 0;
}

