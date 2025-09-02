#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define true 1
#define false 0
#define TAM_SUB 3
#define TOP_CAND 3
#define MAX_LONGITUDES 10

// Frecuencias del español (para chi-cuadrado)
const float frecuencia_es[26] = {
    0.115, 0.022, 0.040, 0.050, 0.125, 0.007, // A-F
    0.010, 0.007, 0.062, 0.004, 0.001, 0.049, // G-L
    0.031, 0.067, 0.086, 0.025, 0.009, 0.068, // M-R
    0.079, 0.046, 0.029, 0.011, 0.001, 0.002, // S-X
    0.010, 0.005                              // Y-Z
};

// Función para limpiar el buffer de entrada
void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ==================== FUNCIONES DE CIFRADO/DESCIFRADO ====================
char* encriptado(char* mensaje, char* llave){
    int i, x, y, j=0;
    int longitudLlave = strlen(llave);
    for (i=0; i<strlen(mensaje); i++){
        if(!isalpha(mensaje[i]))
            continue;
        x = (islower(mensaje[i])) ? 97 : 65;
        y = tolower(llave[j]) - 97;
        
        mensaje[i] = x + ((mensaje[i] - x + y)) % 26;
        j++;
        if(j == longitudLlave)
            j = 0;
    }
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

// ==================== FUNCIONES DE ATAQUE KASISKI ====================

// MCD entre dos enteros (Euclides)
static int mcd(int a, int b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Función para encontrar longitudes candidatas 
void encontrar_longitudes_candidatas(int distancias[], int nd, int longitudes[], int *nlongitudes) {
    int contador[101] = {0}; // Contador para divisores hasta 100
    
    // Contar divisores de cada distancia
    for (int i = 0; i < nd; i++) {
        for (int d = 2; d <= 20 && d <= distancias[i]; d++) {
            if (distancias[i] % d == 0) {
                contador[d]++;
            }
        }
    }
    
    // Encontrar los divisores más frecuentes
    int max_count = 0;
    for (int d = 2; d <= 20; d++) {
        if (contador[d] > max_count) {
            max_count = contador[d];
        }
    }
    
    // Recoger los candidatos (al menos 2 apariciones)
    *nlongitudes = 0;
    for (int d = 2; d <= 20; d++) {
        if (contador[d] >= 2 && contador[d] >= max_count / 2) {
            longitudes[(*nlongitudes)++] = d;
        }
    }
    
    // Si no hay suficientes, añadir algunos comunes
    if (*nlongitudes < 3) {
        for (int d = 2; d <= 20; d++) {
            if (contador[d] > 0 && *nlongitudes < MAX_LONGITUDES) {
                longitudes[(*nlongitudes)++] = d;
            }
        }
    }
}

// Función chi-cuadrado
float chi_cuadrado(const char *texto) {
    int conteo[26] = {0};
    int n = 0;
    
    // Contar letras
    for (int i = 0; texto[i]; i++) {
        if (isalpha(texto[i])) {
            conteo[toupper(texto[i]) - 'A']++;
            n++;
        }
    }
    
    // Calcular chi-cuadrado
    float chi = 0;
    for (int i = 0; i < 26; i++) {
        float esperado = n * frecuencia_es[i];
        if (esperado > 0) {
            chi += pow(conteo[i] - esperado, 2) / esperado;
        }
    }
    return chi;
}

// Función para adivinar clave con chi-cuadrado
void adivinar_clave(const char *texto, int longitud, char *clave) {
    int len = strlen(texto);
    
    for (int i = 0; i < longitud; i++) {
        // Extraer subcriptograma
        char *segmento = malloc(len / longitud + 2);
        int idx = 0;
        
        for (int j = i; j < len; j += longitud) {
            segmento[idx++] = texto[j];
        }
        segmento[idx] = '\0';
        
        // Probar cada posible desplazamiento
        float mejor_chi = 1e9;
        int mejor_k = 0;
        
        for (int k = 0; k < 26; k++) {
            // Descifrar el segmento
            char *descifrado = malloc(idx + 1);
            for (int m = 0; m < idx; m++) {
                int c = segmento[m] - 'A';
                int p = (c - k + 26) % 26;
                descifrado[m] = 'A' + p;
            }
            descifrado[idx] = '\0';
            
            // Calcular chi-cuadrado
            float chi = chi_cuadrado(descifrado);
            if (chi < mejor_chi) {
                mejor_chi = chi;
                mejor_k = k;
            }
            
            free(descifrado);
        }
        
        clave[i] = 'A' + mejor_k;
        free(segmento);
    }
    clave[longitud] = '\0';
}

// Función para descifrar Vigenère (versión para ataque)
void descifrar_vigenere(const char *cifrado, const char *clave, char *descifrado) {
    int len = strlen(cifrado);
    int len_clave = strlen(clave);
    
    for (int i = 0; i < len; i++) {
        if (isalpha(cifrado[i])) {
            int c = toupper(cifrado[i]) - 'A';
            int k = toupper(clave[i % len_clave]) - 'A';
            int p = (c - k + 26) % 26;
            descifrado[i] = 'A' + p;
        } else {
            descifrado[i] = cifrado[i];
        }
    }
    descifrado[len] = '\0';
}

// Función para realizar el ataque Kasiski
void ataque_kasiski() {
    char mensaje[1024];
    
    printf("\n=== ATAQUE KASISKI ===\n");
    printf("Ingrese el texto cifrado (sin espacios): ");
    gets(mensaje);

    int len = strlen(mensaje);
    for (int i = 0; i < len; ++i) {
        mensaje[i] = toupper(mensaje[i]);
    }

    // 1) Buscar repeticiones
    int *distancias = NULL;
    int nd = 0;

    for (int i = 0; i <= len - TAM_SUB; i++) {
        for (int j = i + 1; j <= len - TAM_SUB; j++) {
            if (strncmp(&mensaje[i], &mensaje[j], TAM_SUB) == 0) {
                int d = j - i;
                int *tmp = realloc(distancias, (nd + 1) * sizeof(int));
                if (!tmp) { 
                    printf("Error de memoria.\n"); 
                    free(distancias); 
                    return; 
                }
                distancias = tmp;
                distancias[nd++] = d;
            }
        }
    }

    if (nd == 0) {
        printf("\nNo se hallaron secuencias repetidas de tamaño %d.\n", TAM_SUB);
        free(distancias);
        return;
    }

    // 2) Encontrar longitudes candidatas
    int longitudes[MAX_LONGITUDES];
    int nlongitudes;
    encontrar_longitudes_candidatas(distancias, nd, longitudes, &nlongitudes);

    printf("\nLongitudes de clave candidatas: [");
    for (int i = 0; i < nlongitudes; i++) {
        printf("%d", longitudes[i]);
        if (i < nlongitudes - 1) printf(", ");
    }
    printf("]\n");

    // 3) Probar cada longitud candidata
    for (int l = 0; l < nlongitudes; l++) {
        int longitud = longitudes[l];
        printf("\n--- Probando longitud %d ---\n", longitud);
        
        char *clave_estimada = malloc(longitud + 1);
        adivinar_clave(mensaje, longitud, clave_estimada);
        printf("Clave estimada: %s\n", clave_estimada);
        
        char *descifrado = malloc(len + 1);
        descifrar_vigenere(mensaje, clave_estimada, descifrado);
        printf("Texto descifrado (inicio): %.200s...\n", descifrado);
        
        free(clave_estimada);
        free(descifrado);
    }

    free(distancias);
}

// ==================== FUNCIÓN PRINCIPAL ====================
int main()
{
    char* mensaje;
    char* llave;
    int opcion;
    
    mensaje = (char*) malloc(256 * sizeof(char));
    if (!mensaje) {
        printf("Error de memoria\n");
        return 1;
    }

    llave = (char*) malloc(256 * sizeof(char));
    if (!llave) {
        printf("Error de memoria\n");
        return 1;
    }

    // Ejemplos de prueba
    strcpy(mensaje, "BCD");
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
    printf("\n");

    // Menú principal
    do{
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1) Cifrar/Descifrar mensaje\n");
        printf("2) Ataque Kasiski (romper cifrado)\n");
        printf("3) Salir\n");
        printf("Ingrese la opcion que desee ejecutar: ");
        scanf("%d", &opcion);
        limpiar_buffer();
        
        switch(opcion){
            case 1:
                printf("Ingresa tu mensaje: \n");
                gets(mensaje);
                
                printf("Ingresa tu llave: \n");
                gets(llave);

                int subopcion;
                printf("1) Encriptar\n");
                printf("2) Descencrpitar\n");
                printf("Ingrese opcion: ");
                scanf("%d", &subopcion);
                limpiar_buffer();
                
                if(subopcion == 1){
                    mensaje = encriptado(mensaje, llave);
                    printf("Su mensaje se encripto con éxito, es: \n");
                    printf("%s\n", mensaje);
                } else if(subopcion == 2){
                    mensaje = descencriptado(mensaje, llave);
                    printf("Su mensaje se descencriptó con éxito, es: \n");
                    printf("%s\n", mensaje);
                } else {
                    printf("Opcion invalida.\n");
                }
                break;
            case 2:
                ataque_kasiski();
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Ingrese otra opcion: \n");
                break;
        }
    } while (opcion != 3);
    
    free(mensaje);
    free(llave);
    
    return 0;
}