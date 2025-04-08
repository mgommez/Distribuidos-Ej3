#include <stdio.h>
#include "claves.h"

int main() {
    int resultado;
    double valores_validos[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    double valores_validos11[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 10.10, 11.11};
    struct Coord coord1 = {10, 20};

    printf("\n=== CASO 1: Insertar en una lista vacía ===\n");
    resultado = set_value(1, "PrimerValor", 5, valores_validos, coord1);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);

    printf("\n=== CASO 2: Insertar una clave existente ===\n");
    resultado = set_value(1, "Repetido", 3, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== CASO 3: Insertar una nueva clave ===\n");
    struct Coord coord2 = {30, 40};
    resultado = set_value(2, "SegundoValor", 4, valores_validos, coord2);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);

    printf("\n=== CASO 4: Insertar con N_value2 fuera de rango (N=0) ===\n");
    resultado = set_value(3, "ValorIncorrecto", 0, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== CASO 5: Insertar con N_value2 fuera de rango (N=33) ===\n");
    resultado = set_value(4, "ValorIncorrecto", 33, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    printf("\n=== CASO 5.2: Insertar con N_value2 dentro de rango (N=11) ===\n");
    resultado = set_value(4, "ValorCorrecto", 11, valores_validos11, coord1);
    printf("Resultado esperado: 0, obtenido: %d\n", resultado);

    printf("\n=== CASO 6: Insertar con value1 fuera de rango (longitud = 300) ===\n");
    resultado = set_value(4, "Esta es una cadena de caracteres larga que ocupa más de 256 bytes. Para lograrlo, he incluido texto repetitivo y sin mucho sentido. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam euismod, nisl eget aliquam ultricies, nunc nisl aliquet nunc, quis aliquam nisl nunc eu nisl. Sed euismod, nisl eget aliquam ultricies, nunc nisl aliquet nunc, quis aliquam nisl nunc eu nisl. Fin de la cadena larga.",
                          11, valores_validos, coord1);
    printf("Resultado esperado: -1, obtenido: %d\n", resultado);

    return 0;
}
