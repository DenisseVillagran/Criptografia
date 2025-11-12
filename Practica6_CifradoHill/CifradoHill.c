#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int charToNum(char c){
    //Convertir mayusculas a minusculas y luego le resta 'a' para numero
    return tolower(c)-'a';
}

char numToChar(int n){
    //Convierte numero a caracter sumando 'a'
    return (char)(n + 'a');
}

//Funcion para multiplicar matrices
void multiplicarMatrices(int **matrizLlave, int *vectorMensaje, int *vectorCifrado, int tamano){
    for(int i = 0; i < tamano; i++){
        vectorCifrado[i] = 0;
        for(int j = 0; j < tamano; j++){
            vectorCifrado[i] += matrizLlave[i][j] * vectorMensaje[j];
        }
        vectorCifrado[i] = vectorCifrado[i] % 26; //Modulo 26 para mantener en rango de letras
    }
}

void imprimirMatriz(int **matriz, int tamano){
    for(int i = 0; i < tamano; i++){
        for(int j = 0; j < tamano; j++){
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void determinanteMatriz(int **matriz, int tamano, int *determinante){
    if(tamano == 1){
        *determinante = matriz[0][0];
        return;
    }
    if(tamano == 2){
        *determinante = matriz[0][0]*matriz[1][1] - matriz[0][1]*matriz[1][0];
        return;
    }
    int det = 0;
    for(int c = 0; c < tamano; c++){
        //Crear submatriz
        int **submatriz = (int **)malloc((tamano-1) * sizeof(int *));
        for(int i = 0; i < tamano-1; i++){
            submatriz[i] = (int *)malloc((tamano-1) * sizeof(int));
        }
        for(int i = 1; i < tamano; i++){
            int subcol = 0;
            for(int j = 0; j < tamano; j++){
                if(j == c) continue;
                submatriz[i-1][subcol] = matriz[i][j];
                subcol++;
            }
        }
        int subdet;
        determinanteMatriz(submatriz, tamano-1, &subdet);
        //Actualizar determinante
        det += (c % 2 == 0 ? 1 : -1) * matriz[0][c] * subdet;

        //Liberar memoria de submatriz
        for(int i = 0; i < tamano-1; i++){
            free(submatriz[i]);
        }
        free(submatriz);
    }
    *determinante = det;
}

void modInverse(int a, int m, int *inv){
    //Calcular inverso modular de a mod m usando busqueda exhaustiva
    a = a % m;
    for(int x = 1; x < m; x++){
        if((a * x) % m == 1){
            *inv = x;
            return;
        }
    }
    *inv = -1; //No existe inverso
}

void matrizCofactores(int **matriz, int tamano, int **cofactores){
    for(int i = 0; i < tamano; i++){
        for(int j = 0; j < tamano; j++){
            //Crear submatriz
            int **submatriz = (int **)malloc((tamano-1) * sizeof(int *));
            for(int k = 0; k < tamano-1; k++){
                submatriz[k] = (int *)malloc((tamano-1) * sizeof(int));
            }
            int subi = 0;
            for(int m = 0; m < tamano; m++){
                if(m == i) continue;
                int subj = 0;
                for(int n = 0; n < tamano; n++){
                    if(n == j) continue;
                    submatriz[subi][subj] = matriz[m][n];
                    subj++;
                }
                subi++;
            }
            int subdet;
            determinanteMatriz(submatriz, tamano-1, &subdet);
            cofactores[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * subdet;

            //Liberar memoria de submatriz
            for(int k = 0; k < tamano-1; k++){
                free(submatriz[k]);
            }
            free(submatriz);
        }
    }
}

void transponerMatriz(int **matriz, int tamano, int **transpuesta){
    for(int i = 0; i < tamano; i++){
        for(int j = 0; j < tamano; j++){
            transpuesta[j][i] = matriz[i][j];
        }
    }
}

void aplicarModuloMatriz(int **matriz, int tamano, int modulo){
    for(int i = 0; i < tamano; i++){
        for(int j = 0; j < tamano; j++){
            matriz[i][j] = (matriz[i][j] % modulo + modulo) % modulo;
        }
    }
}

void multiplicarMatrizAdjporInversoMod(int **adjunta, int inversoDet, int tamano, int **inversa){
    for(int i = 0; i < tamano; i++){
        for(int j = 0; j < tamano; j++){
            inversa[i][j] = (adjunta[i][j] * inversoDet) % 26;
            if(inversa[i][j] < 0) inversa[i][j] += 26;
        }
    }
}

void calcularMatrizInversa(int **matriz, int tamano, int **inversa){
    int det;
    determinanteMatriz(matriz, tamano, &det);
    det = (det % 26 + 26) % 26; //Asegurar positivo

    int inversoDet;
    modInverse(det, 26, &inversoDet);
    if(inversoDet == -1){
        printf("La matriz no es invertible modulo 26\n");
        return;
    }
    printf("Inverso modular del determinante: %d\n", inversoDet);

    //Calcular matriz de cofactores
    int **cofactores = (int **)malloc(tamano * sizeof(int *));
    for(int i = 0; i < tamano; i++){
        cofactores[i] = (int *)malloc(tamano * sizeof(int));
    }
    matrizCofactores(matriz, tamano, cofactores);
    printf("Matriz de cofactores:\n");
    imprimirMatriz(cofactores, tamano);

    //Transponer cofactores para obtener adjunta
    int **adjunta = (int **)malloc(tamano * sizeof(int *));
    for(int i = 0; i < tamano; i++){
        adjunta[i] = (int *)malloc(tamano * sizeof(int));
    }
    transponerMatriz(cofactores, tamano, adjunta);
    printf("Matriz adjunta:\n");
    imprimirMatriz(adjunta, tamano);

    //Multiplicar adjunta por inverso del determinante modulo 26
    multiplicarMatrizAdjporInversoMod(adjunta, inversoDet, tamano, inversa);
    printf("Matriz inversa modulo 26:\n");
    imprimirMatriz(inversa, tamano);

    //Liberar memoria
    for(int i = 0; i < tamano; i++){
        free(cofactores[i]);
        free(adjunta[i]);
    }
    free(cofactores);
    free(adjunta);
}

void descifrarMensaje(int **matrizInversa, int *vectorCifrado, int *vectorDescifrado, int tamano){
    for(int i = 0; i < tamano; i++){
        vectorDescifrado[i] = 0;
        for(int j = 0; j < tamano; j++){
            vectorDescifrado[i] += matrizInversa[i][j] * vectorCifrado[j];
        }
        vectorDescifrado[i] = vectorDescifrado[i] % 26; //Modulo 26 para mantener en rango de letras
    }
}

int main(){
    char *mensaje;
    char *llave;

    //Asignar memoria para mensaje y llave
    mensaje = (char *)malloc(256*sizeof(char));
    llave = (char *)malloc(356*sizeof(char));
    if(mensaje == NULL || llave == NULL){
        printf("Error al asignar memoria\n");
        return 1;
    }

    //Lecura de mensaje y llave
    printf("Ingrese el mensaje a cifrar: ");
    fflush(stdin);
    gets(mensaje);

    int tamanoMensaje = strlen(mensaje);

    printf("Ingrese la llave de cifrado, considera que debe tener %d caracteres: ", tamanoMensaje*tamanoMensaje);
    fflush(stdin);
    gets(llave);

    int tamanoLlave = strlen(llave);
    if(tamanoLlave != tamanoMensaje*tamanoMensaje){
        printf("Error: La llave debe tener exactamente %d caracteres.\n", tamanoMensaje*tamanoMensaje);
        free(mensaje);
        free(llave);
        return 1;
    }

    //Crear matriz de la llave
    int **matrizLlave = (int **)malloc(tamanoMensaje * sizeof(int *));
    for(int i = 0; i < tamanoMensaje; i++){
        matrizLlave[i] = (int *)malloc(tamanoMensaje * sizeof(int));
    }

    //Llenar matriz de la llave
    int k = 0;
    for(int i = 0; i < tamanoMensaje; i++){
        for(int j = 0; j < tamanoMensaje; j++){
            matrizLlave[i][j] = charToNum(llave[k]);
            k++;
        }
    }

    //Crear vector del mensaje
    int *vectorMensaje = (int *)malloc(tamanoMensaje * sizeof(int));
    for(int i = 0; i < tamanoMensaje; i++){
        vectorMensaje[i] = charToNum(mensaje[i]);
    }

    //Imprimir Mensaje como Matriz
    printf("Matriz del mensaje:\n");
    for(int i = 0; i < tamanoMensaje; i++){
        printf("%d\n", vectorMensaje[i]);
    }

    //Imprimir Llave como Matriz
    printf("Matriz de la llave:\n");
    imprimirMatriz(matrizLlave, tamanoMensaje);

    //Cifrado Hill
    //Crear vector del mensaje cifrado
    int *vectorCifrado = (int *)malloc(tamanoMensaje * sizeof(int));
    multiplicarMatrices(matrizLlave, vectorMensaje, vectorCifrado, tamanoMensaje);

    //Imprimir vector cifrado
    printf("Mensaje cifrado:\n");
    for(int i = 0; i < tamanoMensaje; i++){
        printf("%d\n", vectorCifrado[i]);
    }

    //Mostar mensaje cifrado
    printf("Mensaje cifrado: ");
    for(int i = 0; i < tamanoMensaje; i++){
        printf("%c", numToChar(vectorCifrado[i]));
    }
    printf("\n");

    //Descifrado Hill
    //Obtener determinante modular de la matriz llave
    int **matrizInversa = (int **)malloc(tamanoMensaje * sizeof(int *));
    for(int i = 0; i < tamanoMensaje; i++){
        matrizInversa[i] = (int *)malloc(tamanoMensaje * sizeof(int));
    }
    calcularMatrizInversa(matrizLlave, tamanoMensaje, matrizInversa);
    printf("Matriz inversa de la llave:\n");
    imprimirMatriz(matrizInversa, tamanoMensaje);

    //Crear vector del mensaje descifrado
    int *vectorDescifrado = (int *)malloc(tamanoMensaje * sizeof(int));
    descifrarMensaje(matrizInversa, vectorCifrado, vectorDescifrado, tamanoMensaje);

    //Imprimir vector descifrado
    printf("Mensaje descifrado:\n");
    for(int i = 0; i < tamanoMensaje; i++){
        printf("%d\n", vectorDescifrado[i]);
    }

    //Mostar mensaje descifrado
    printf("Mensaje descifrado: ");
    for(int i = 0; i < tamanoMensaje; i++){
        printf("%c", numToChar(vectorDescifrado[i]));
    }
    printf("\n");

    //Liberar memoria
    for(int i = 0; i < tamanoMensaje; i++){
        free(matrizLlave[i]);
    }
    free(matrizLlave);
    free(vectorMensaje);
    free(vectorCifrado);
    free(mensaje);
    free(llave);
    for(int i = 0; i < tamanoMensaje; i++){
        free(matrizInversa[i]);
    }
    free(matrizInversa);
    free(vectorDescifrado);



    return 0;


}