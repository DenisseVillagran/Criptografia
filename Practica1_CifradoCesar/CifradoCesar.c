#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* encriptado(char* mensaje, int llave){
    int i;
    for (i=0; i<strlen(mensaje); i++){
        if (mensaje[i]== 32)
            continue;
        else if (mensaje[i] >= 65 && mensaje[i] <= 90)
            mensaje[i] = 65 + (mensaje[i] - 65 + llave) % 26;
        else if (mensaje[i] >= 97 && mensaje[i] <= 122) 
            mensaje[i] = 97 + (mensaje[i] - 97 + llave) % 26;
    }
    return mensaje;
}

char* descencriptado(char* mensaje, int llave){
    int i;
    for (i=0; i<strlen(mensaje); i++){
        if (mensaje[i]== 32)
            continue;
        else if (mensaje[i] >= 65 && mensaje[i] <= 90)
            mensaje[i] = 65 + (mensaje[i] - 65 - llave) % 26;
        else if (mensaje[i] >= 97 && mensaje[i] <= 122) 
            mensaje[i] = 97 + (mensaje[i] - 97 - llave) % 26;
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
