#include<stdio.h>
#include<stdlib.h>
#include<time.h>


//LEEMOS EL MENSAJE
int contar_caracteres_archivo(const char *nombre_archivo) {
    FILE *file = fopen(nombre_archivo, "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre_archivo);
        return -1;
    }
    int contador = 0;
    int c;
    while ((c = fgetc(file)) != EOF) {
        contador++;
    }
    fclose(file);
    return contador;
}


//GENERAMOS LA LLAVE
void generar_archivo_K(const char *nombre_archivo, long longitud) {
    FILE *file = fopen(nombre_archivo, "w");
    if (file == NULL) {
        printf("No se pudo crear el archivo %s\n", nombre_archivo);
        return;
    }
    srand(time(NULL));
    for (int i = 0; i < longitud; i++) {
        char letra = 'A' + (rand() % 26); // Letra aleatoria entre 'A' y 'Z'
        fputc(letra, file);
    }
    fclose(file);
}

//VALOR A BINARIO
void valor_a_binario(int valor, char *bin) {
    for (int i = 4; i >= 0; i--) {
        bin[i] = (valor & 1) ? '1' : '0';
        valor >>= 1;
    }
    bin[5] = '\0';
}

// Convierte una cadena binaria de 5 bits a valor (0-25)
int binario_a_valor(const char *bin) {
    int valor = 0;
    for (int i = 0; i < 5; i++) {
        valor = (valor << 1) | (bin[i] - '0');
    }
    return valor;
}

// Cifra el mensaje usando bloques binarios de 5 bits y XOR
void cifrar_archivo(const char *archivo_m, const char *archivo_k, const char *archivo_cm, int longitud) {
    FILE *fm = fopen(archivo_m, "r");
    FILE *fk = fopen(archivo_k, "r");
    FILE *fcm = fopen(archivo_cm, "w");
    if (fm == NULL || fk == NULL || fcm == NULL) {
        printf("Error al abrir uno de los archivos para cifrado\n");
        if (fm) fclose(fm);
        if (fk) fclose(fk);
        if (fcm) fclose(fcm);
        return;
    }

    for (int i = 0; i < longitud; i++) {
        char letra_m = fgetc(fm);
        char letra_k = fgetc(fk);
        if (letra_m == EOF || letra_k == EOF) break;

        int val_m = letra_m - 'A';
        int val_k = letra_k - 'A';

        char bin_m[6], bin_k[6], bin_c[6];
        valor_a_binario(val_m, bin_m);
        valor_a_binario(val_k, bin_k);

        // XOR bit a bit
        for (int j = 0; j < 5; j++) {
            bin_c[j] = (bin_m[j] == bin_k[j]) ? '0' : '1';
        }
        bin_c[5] = '\0';

        int val_c = binario_a_valor(bin_c);
        char letra_c = 'A' + (val_c % 26);

        fputc(letra_c, fcm);
    }

    fclose(fm);
    fclose(fk);
    fclose(fcm);
}

// Descifra el mensaje usando bloques binarios de 5 bits y XOR, imprime en terminal y elimina K.txt
void descifrar_archivo(const char *archivo_cm, const char *archivo_k, int longitud) {
    FILE *fcm = fopen(archivo_cm, "r");
    FILE *fk = fopen(archivo_k, "r");
    if (fcm == NULL || fk == NULL) {
        printf("Error al abrir Cm.txt o K.txt para descifrado\n");
        if (fcm) fclose(fcm);
        if (fk) fclose(fk);
        return;
    }

    printf("Mensaje desencriptado:\n");
    for (int i = 0; i < longitud; i++) {
        char letra_c = fgetc(fcm);
        char letra_k = fgetc(fk);
        if (letra_c == EOF || letra_k == EOF) break;

        int val_c = letra_c - 'A';
        int val_k = letra_k - 'A';

        char bin_c[6], bin_k[6], bin_m[6];
        valor_a_binario(val_c, bin_c);
        valor_a_binario(val_k, bin_k);

        // XOR bit a bit
        for (int j = 0; j < 5; j++) {
            bin_m[j] = (bin_c[j] == bin_k[j]) ? '0' : '1';
        }
        bin_m[5] = '\0';

        int val_m = binario_a_valor(bin_m);
        char letra_m = 'A' + (val_m % 26);

        printf("%c", letra_m);
    }
    printf("\n");

    fclose(fcm);
    fclose(fk);

    // Elimina el archivo K.txt
    if (remove(archivo_k) == 0) {
        printf("Archivo %s eliminado correctamente.\n", archivo_k);
    } else {
        printf("No se pudo eliminar el archivo %s.\n", archivo_k);
    }
}

int main() {
    int opcion;
    do {
        printf("\n--- MENU ---\n");
        printf("1) Cifrar con Vernam\n");
        printf("2) Desencriptar\n");
        printf("0) Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1: {
                int caracteres = contar_caracteres_archivo("M.txt");
                if (caracteres != -1) {
                    printf("El número de caracteres en M.txt es: %d\n", caracteres);
                    generar_archivo_K("K.txt", caracteres);
                    printf("Archivo K.txt generado con %d letras aleatorias.\n", caracteres);
                    cifrar_archivo("M.txt", "K.txt", "Cm.txt", caracteres);
                    printf("Archivo Cm.txt generado con el mensaje cifrado.\n");
                }
                break;
            }
            case 2: {
                int caracteres = contar_caracteres_archivo("Cm.txt");
                if (caracteres != -1) {
                    descifrar_archivo("Cm.txt", "K.txt", caracteres);
                }
                break;
            }
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida\n");
        }
    } while(opcion != 0);

    return 0;
}
