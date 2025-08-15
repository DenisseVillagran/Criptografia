#include <stdio.h> 

#include <stdlib.h> 

#include <string.h> 

 

Char* encriptado(char* mensaje, int llave){ 

    Int i; 

    For (i=0; i<strlen(mensaje); i++){ 

        If (mensaje[i]== 32) 

            Continue; 

        Else if (mensaje[i] >= 65 && mensaje[i] <= 90) 

            Mensaje[i] = 65 + (mensaje[i] – 65 + llave) % 26; 

        Else if (mensaje[i] >= 97 && mensaje[i] <= 122)  

            Mensaje[i] = 97 + (mensaje[i] – 97 + llave) % 26; 

    } 

    Return mensaje; 

} 

 

Char* descencriptado(char* mensaje, int llave){ 

    Int i; 

    For (i=0; i<strlen(mensaje); i++){ 

        If (mensaje[i]== 32) 

            Continue; 

        Else if (mensaje[i] >= 65 && mensaje[i] <= 90) 

            Mensaje[i] = 65 + (mensaje[i] – 65 – llave) % 26; 

        Else if (mensaje[i] >= 97 && mensaje[i] <= 122)  

            Mensaje[i] = 97 + (mensaje[i] – 97 – llave) % 26; 

    } 

    Return mensaje; 

} 

 

Int main() 

{ 

    Char* mensaje; 

    Int llave, opcion; 

     

    Mensaje = (char*) malloc(256 * sizeof(char));//Reserva de memoria dinámica 

    If (¡mensaje) { 

        Printf(“Error de memoria\n”); 

        Return 1; 

    } 

     

    Printf(“Ingresa tu mensaje: \n”); 

    Fflush(stdin); 

    Gets(mensaje); 

     

    Printf(“Ingresa el número de llave\n”); 

    Scanf(“%i”, &llave); 

    Do{ 

        Printf(“Ingrese la opción que desee ejecutar:\n”); 

        Printf(“1) Encriptar\n”); 

        Printf(“2) Descencrpitar\n”); 

        Printf(“3) Salir\n”); 

        Scanf(“%d”, &opcion); 

         

        Switch(opcion){ 

            Case 1: 

                Mensaje=encriptado(mensaje, llave); 

                Printf(“Su mensaje se encripto con éxito, es: \n”); 

                Printf(“%s\n”, mensaje); 

                Break; 

            Case 2: 

                Mensaje=descencriptado(mensaje, llave); 

                Printf(“Su mensaje se descencriptó con éxito, es: \n”); 

                Printf(“%s\n”, mensaje); 

                Break;  

            Case 3: 

                Printf(“Saliendo…\n”); 

                Break; 

            Default: 

                Printf(“Ingrese otra opción: \n”); 

                Break; 

        } 

    } while (opcion!=3); 

     

    Free(mensaje); 

     

    Return 0; 

} 