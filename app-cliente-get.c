#include <stdio.h>
#include "claves.h"


int main() {
    int resultado;
    double valores_validos[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    struct Coord coord1 = {10, 20};
    char value1[50];
    int N_value2;
    double V_value2[10];
    struct Coord value3;

    printf("\n=== CASO 1: Obtener valor de lista vacía ===\n");
    resultado = get_value(1, value1, &N_value2, V_value2, &value3);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== Insertando valores en la lista ===\n");
    set_value(1, "PrimerValor", 5, valores_validos, coord1);
    set_value(2, "SegundoValor", 4, valores_validos, coord1);

    printf("\n=== CASO 2: Obtener valor de clave existente ===\n");
    resultado = get_value(1, value1, &N_value2, V_value2, &value3);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);
    printf("Valor obtenido: %s, N_value2: %d\n", value1, N_value2);

    printf("\n=== CASO 3: Obtener valor de clave inexistente ===\n");
    resultado = get_value(99, value1, &N_value2, V_value2, &value3);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    return 0;
}