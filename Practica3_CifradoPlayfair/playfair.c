#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define true 1
#define false 0

char matriz[5][5]; //Matriz generada

void generadorMatriz(char* llave){
    int len_key = strlen(llave);
    int aux[len_key];
    int indice_llave = 0;
    int bandera_limiteLlave = false;
    int alfabeto = 97;

    for(int i=0; i<strlen(aux); i++){
        aux[i]=false;
    }

    for (int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(indice_llave>=len_key){
                bandera_limiteLlave = true;
                break;
            }else{
                if(aux[llave[i]-97] == false){
                    if(llave[i] == 106){
                        matriz[i][j] = 105;        
                    }else{
                         matriz[i][j] = llave[indice_llave];
                    }
                    indice_llave++;
                }    
            }
            if(bandera_limiteLlave){
                if(aux[alfabeto-97] == false){
                    if(alfabeto == 106){
                        matriz[i][jx] = 105;
                    }else{
                        matriz[i][j] = alfabeto;
                    }
                    
                }
            }
        }
        //if(bandera_limiteLlave)
           // break;
    }
}

char* encriptado(char* mensaje, char* llave){
    
    return mensaje;
}

char* descencriptado(char* mensaje, char* llave){
    int i, x, y, j=0;
    int longitudLlave = strlen(llave);
    for (i=0; i<strlen(mensaje); i++){
        if(!isalpha(mensaje[i]))
            continue;
        x = (islower(mensaje[i])) ? 97 : 65;
        y = tolower(llave[j]) - 97;
        
        mensaje[i] = x + (((mensaje[i] - x - y) + 26)) % 26;
        j++;
        if(j == longitudLlave)
            j = 0;
    }
    return mensaje;
}

int main()
{
    char* mensaje;
    char* llave;
    int opcion;
    
    mensaje = (char*) malloc(256 * sizeof(char));//Reserva de memoria dinámica
    if (!mensaje) {
        printf("Error de memoria\n");
        return 1;
    }

    llave = (char*) malloc(256 * sizeof(char));//Reserva de memoria dinámica
    if (!llave) {
        printf("Error de memoria\n");
        return 1;
    }

    /*strcpy(mensaje, "BCD");
    strcpy(llave, "DEF");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, llave));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, llave));
    printf("\n");

    strcpy(mensaje, "PruebaCifrado");
    strcpy(llave, "enbs");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, llave));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, llave));
    printf("\n");

    strcpy(mensaje, "AbcDefGhiJklMnoPqr");
    strcpy(llave, "UNhafna");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, llave));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, llave));
    printf("\n");

    strcpy(mensaje, "esto Es Una Cadena Larga");
    strcpy(llave, "MSKna");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, llave));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, llave));
    printf("\n");

    strcpy(mensaje, "AbcD/eF8Gh*I[jK");
    strcpy(llave, "kanamfdnUSFM");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, llave));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, llave));
    printf("\n");

    strcpy(mensaje, "X");
    strcpy(llave, "AaaA");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, llave));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, llave));
    printf("\n");*/

    
    printf("Ingresa tu mensaje: \n");
    fflush(stdin);
    gets(mensaje);
    
    
    printf("Ingresa tu llave: \n");
    fflush(stdin);
    gets(llave);

    do{
        printf("Ingrese la opción que desee ejecutar:\n");
        printf("1) Encriptar\n");
        printf("2) Descencrpitar\n");
        printf("3) Salir\n");
        scanf("%d", &opcion);
        
        switch(opcion){
            case 1:
                mensaje=encriptado(mensaje, llave);
                printf("Su mensaje se encripto con éxito, es: \n");
                printf("%s\n", mensaje);
                break;
            case 2:
                mensaje=descencriptado(mensaje, llave);
                printf("Su mensaje se descencriptó con éxito, es: \n");
                printf("%s\n", mensaje);
                break; 
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Ingrese otra opción: \n");
                break;
        }
    } while (opcion!=3);
    
    free(mensaje);
    free(llave);
    
    return 0;
}