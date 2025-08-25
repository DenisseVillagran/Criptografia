#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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

// Función para descifrar Vigenère
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

int main(void) {
    char *mensaje = NULL;
    size_t cap = 1024;
    mensaje = (char*)malloc(cap);
    if (!mensaje) { puts("Error de memoria."); return 1; }

    printf("Ingrese el texto cifrado (sin espacios): ");
    if (scanf("%1023s", mensaje) != 1) { puts("Entrada invalida."); free(mensaje); return 1; }

    int len = (int)strlen(mensaje);
    for (int i = 0; i < len; ++i) mensaje[i] = (char)toupper((unsigned char)mensaje[i]);

    // 1) Buscar repeticiones
    int *distancias = NULL;
    int nd = 0;

    for (int i = 0; i <= len - TAM_SUB; i++) {
        for (int j = i + 1; j <= len - TAM_SUB; j++) {
            if (strncmp(&mensaje[i], &mensaje[j], TAM_SUB) == 0) {
                int d = j - i;
                int *tmp = (int*)realloc(distancias, (nd + 1) * sizeof(int));
                if (!tmp) { puts("Error de memoria."); free(mensaje); free(distancias); return 1; }
                distancias = tmp;
                distancias[nd++] = d;
            }
        }
    }

    if (nd == 0) {
        puts("\nNo se hallaron secuencias repetidas de tamaño 3.");
        free(mensaje);
        free(distancias);
        return 0;
    }

    // 2) Encontrar longitudes candidatas (como en Python)
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

    free(mensaje);
    free(distancias);
    return 0;
}