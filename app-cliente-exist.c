#include <stdio.h>
#include "claves.h"

int main() {
    printf("\n=== CASO 1: Lista vacía ===\n");
    int resultado = exist(1);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);

    printf("\n=== Agregando una tupla con clave 1 ===\n");
    struct Coord coord1 = {10, 20};
    double valores_validos[] = {1.1, 2.2, 3.3};
    set_value(1, "Valor1", 3, valores_validos, coord1);

    printf("\n=== CASO 2: Buscar una clave existente ===\n");
    resultado = exist(1);
    printf("Resultado esperado: 1, obtenido: %d\n", resultado);

    printf("\n=== CASO 3: Buscar una clave inexistente ===\n");
    resultado = exist(99);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);

    return 0;
}

