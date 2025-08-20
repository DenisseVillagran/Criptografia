#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define true 1
#define false 0

char* encriptado(char* mensaje, int llave){
    int i;
    for (i=0; i<strlen(mensaje); i++){
        if (isupper(mensaje[i])) 
            mensaje[i] = 65 + ((mensaje[i] - 65 + llave)) % 26;

        else if (islower(mensaje[i]))
            mensaje[i] = 97 + ((mensaje[i] - 97 + llave)) % 26;
    }
    return mensaje;
}

char* descencriptado(char* mensaje, int llave){
    int i;
    for (i=0; i<strlen(mensaje); i++){
        if (isupper(mensaje[i]))
            mensaje[i] = 65 + (((mensaje[i] - 65 - llave) + 26)) % 26;
        
        else if (islower(mensaje[i])) 
            mensaje[i] = 97 + (((mensaje[i] - 97 - llave) + 26)) % 26;
        
    }
    return mensaje;
}

int main()
{
    char* mensaje;
    int llave, opcion;
    
    mensaje = (char*) malloc(256 * sizeof(char));//Reserva de memoria dinámica
    if (!mensaje) {
        printf("Error de memoria\n");
        return 1;
    }

    strcpy(mensaje, "Hola");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, 6));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, 6));
    printf("\n");

    strcpy(mensaje, "PruebaCifrado");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, 3));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, 3));
    printf("\n");

    strcpy(mensaje, "AbcDefGhiJklMnoPqr");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, 10));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, 10));
    printf("\n");

    strcpy(mensaje, "esto Es Una Cadena Larga");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, 24));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, 24));
    printf("\n");

    strcpy(mensaje, "AbcD/eF8Gh*I[jK");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, 15));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, 15));
    printf("\n");

    strcpy(mensaje, "X");
    printf("\nCadena original: %s", mensaje);
    printf("\nCadena encriptada: %s", encriptado(mensaje, 17));
    printf("\nCadena descencriptada: %s", descencriptado(mensaje, 17));
    printf("\n");

    
    printf("Ingresa tu mensaje: \n");
    fflush(stdin);
    gets(mensaje);
    
    printf("Ingresa el número de llave\n");
    scanf("%i", &llave);
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
    
    return 0;
}
