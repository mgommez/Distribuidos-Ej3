#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "claves.h"

int main() {
    int resultado;
    double valores_validos[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    struct Coord coord1 = {10, 20};

    printf("\n=== CASO 1: Borrar en lista vacía ===\n");
    resultado = delete_key(2);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== Insertando valores en la lista ===\n");
    set_value(1, "PrimerValor", 5, valores_validos, coord1);
    set_value(2, "SegundoValor", 4, valores_validos, coord1);

    printf("\n=== CASO 2: Borrar clave existente ===\n");
    resultado = delete_key(1);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);

    printf("\n=== CASO 3: Borrar clave inexistente ===\n");
    resultado = delete_key(99);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    return 0;
}
