#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   // <-- agregado

#define TAM_SUB 3   // tamaño de la secuencia a buscar

// ====== Agregado: parámetros/ayudas del ataque ======
#define TOP_CAND 3  // cuántos candidatos por posición de la llave mostrar
// patrón de offsets para letras frecuentes en inglés: A,E,O,T -> 0,+4,+10,+5 mod 26
static const int PAT[4] = {0, 4, 14, 19};

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

int main(void) {
    char *mensaje = NULL;
    size_t cap = 1024;
    mensaje = (char*)malloc(cap);
    if (!mensaje) { puts("Error de memoria."); return 1; }

    printf("Ingrese el texto cifrado (sin espacios): ");
    if (scanf("%1023s", mensaje) != 1) { puts("Entrada invalida."); free(mensaje); return 1; }

    int len = (int)strlen(mensaje);

    // Normalizamos a MAYÚSCULAS (agregado, no modifica lo anterior)
    for (int i = 0; i < len; ++i) mensaje[i] = (char)toupper((unsigned char)mensaje[i]);

    // -------- 1) Buscar repeticiones de tamaño 3 y guardar distancias ----------
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

                printf("Secuencia \"%.*s\" en %d y %d (distancia %d)\n",
                       TAM_SUB, &mensaje[i], i, j, d);
            }
        }
    }

    if (nd == 0) {
        puts("\nNo se hallaron secuencias repetidas de tamaño 3.");
        free(mensaje);
        free(distancias);
        return 0;
    }

    // -------- 2) Calcular MCD de todas las distancias -> llave ---------------
    int llave = distancias[0];
    for (int i = 1; i < nd; i++) llave = mcd(llave, distancias[i]);
    if (llave <= 0) llave = 1;   // resguardo

    printf("\nVector de distancias: ");
    for (int i = 0; i < nd; i++) printf("%d ", distancias[i]);
    printf("\nMCD (llave) = %d\n", llave);

    // -------- 3) Construir subcriptogramas según la llave --------------------
    char **subs = (char**)calloc(llave, sizeof(char*));
    if (!subs) { puts("Error de memoria."); free(mensaje); free(distancias); return 1; }

    for (int r = 0; r < llave; r++) {
        // capacidad exacta = ceil((len - r) / llave) + 1 para '\0'
        int cap_r = ( (len - r + llave - 1) / llave ) + 1;
        subs[r] = (char*)malloc(cap_r);
        if (!subs[r]) { puts("Error de memoria."); free(mensaje); free(distancias);
                        for (int k = 0; k < r; k++) free(subs[k]); free(subs); return 1; }

        int w = 0;
        for (int pos = r; pos < len; pos += llave) subs[r][w++] = mensaje[pos];
        subs[r][w] = '\0';
    }

    // -------- 4) Imprimir subcriptogramas ------------------------------------
    puts("\nSubcriptogramas (C_A, C_B, C_C, ...):");
    for (int r = 0; r < llave; r++) {
        char etiqueta = (char)('A' + (r % 26)); // A, B, C, ...
        printf("C%c = %s\n", etiqueta, subs[r]);
    }

    // ================== AGREGADO: Ataque estadístico simple ==================
    // Método del enlace: buscar, para cada posible K (0..25),
    // la suma de frecuencias en posiciones K + {0,4,14,19} (A,E,O,T).
    // Los K con mayor suma son candidatos de letra de clave para esa posición.
    printf("\n--- Ataque estadistico (patron A,E,O,T -> 0,+4,+10,+5 mod 26) ---\n");

    // Para construir una clave sugerida tomamos el mejor candidato de cada posición
    char *clave_sugerida = (char*)malloc(llave + 1);
    if (!clave_sugerida) { puts("Error de memoria."); goto limpieza; }

    for (int r = 0; r < llave; ++r) {
        int counts[26] = {0};
        int n = 0;
        for (char *p = subs[r]; *p; ++p) {
            if (*p >= 'A' && *p <= 'Z') { counts[*p - 'A']++; n++; }
        }

        // top candidatos (k y su puntaje)
        int topK[TOP_CAND];     for (int i = 0; i < TOP_CAND; ++i) topK[i] = -1;
        int topScore[TOP_CAND]; for (int i = 0; i < TOP_CAND; ++i) topScore[i] = -1;

        for (int k = 0; k < 26; ++k) {
            int score = 0;
            for (int t = 0; t < 4; ++t) {
                score += counts[(k + PAT[t]) % 26];
            }
            // insert sort para top-k máximos
            for (int pos = 0; pos < TOP_CAND; ++pos) {
                if (score > topScore[pos]) {
                    for (int q = TOP_CAND - 1; q > pos; --q) {
                        topScore[q] = topScore[q - 1];
                        topK[q]     = topK[q - 1];
                    }
                    topScore[pos] = score;
                    topK[pos]     = k;
                    break;
                }
            }
        }

        // imprimir candidatos y fijar el mejor para la clave sugerida
        printf("Posicion %d (C%c): candidatos ", r + 1, 'A' + (r % 26));
        for (int i = 0; i < TOP_CAND; ++i) {
            if (topK[i] >= 0) printf("[%c (puntaje=%d)] ", 'A' + topK[i], topScore[i]);
        }
        printf("\n");

        clave_sugerida[r] = (topK[0] >= 0) ? (char)('A' + topK[0]) : 'A';
    }
    clave_sugerida[llave] = '\0';

    printf("\nClave sugerida (mas probable segun el patron): %s\n", clave_sugerida);
    printf("(Puedes probar tambien combinaciones usando los candidatos por posicion.)\n");

    free(clave_sugerida);
    // ================== FIN ataque estadístico simple ==================

limpieza:
    // Limpieza
    for (int r = 0; r < llave; r++) free(subs[r]);
    free(subs);
    free(mensaje);
    free(distancias);
    return 0;
}
