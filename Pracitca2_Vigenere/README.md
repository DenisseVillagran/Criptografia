# Práctica 02: Cifrado Vigenère y Criptoanálisis Kasiski

**Integrantes:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Descripción

**Cifrado Vigenère**

aaaa

**Criptoanálisis Kasiski**

El método de Kasiski (F. Kasiski, 1863) es una técnica clásica para atacar el cifrado de Vigenère. Se basa en que secuencias 
de letras repetidas del texto cifrado suelen originarse en la misma palabra del texto plano que quedó alineada con la misma 
porción de la clave. Midiendo la distancia entre esas repeticiones y calculando su MCD, se estima la longitud de la clave. 
Con esa longitud, el criptograma se divide en subcriptogramas (uno por cada posición de la clave) y cada uno se analiza como 
un César independiente usando frecuencias del idioma. Es una técnica simple y efectiva con textos de tamaño razonable.

---

## Implementación

**Cifrado Vigenère**

aaa

**Criptoanálisis Kasiski**

La implementación se diseñó utilizando un arreglo dinámico con apuntadores `char* con malloc` para almacenar el mensaje
cifrado, lo que permite manejar cadenas de longitud variable sin depender de un arreglo estático, la cadena introduccida no 
debe tener espacios. Posteriormente se convierte todo a mayúsculas `toupper` para trabajar solo con el alfabeto `A..Z`, esto 
evita tratar casos de minúsculas o caracteres fuera del rango.

**1) Buscar repeticiones de tamaño 3 y guardar distancias**

Se recorre el texto con dos índices (`i`, `j`) y compara trigramas `strncmp(&mensaje[i], &mensaje[j], 3)`. En Kasiski, 
repeticiones de 3 letras suelen indicar que una misma palabra del texto plano coincidió con la misma parte de la clave.

Cada vez que hay coincidencia, se calcula la distancia `d = j - i` y se guarda en un vector dinámico 
`int *distancias`. Se obtiene una lista de distancias entre repeticiones.

**2) Calcular MCD de todas las distancias → longitud de la llave**

Toma todas las distancias y calcula su Máximo Común Divisor usando Euclides (`mcd`). El MCD de esas distancias suele ser la 
longitud de la clave (cuántas letras tiene). Se imprime el vector de distancias y el MCD, guardado en `int llave`.

**3) Construir subcriptogramas según la llave**

Divide el criptograma en llave subcriptogramas, uno por cada posición de la clave:
- Sub 0: posiciones 0, L, 2L, ...
- Sub 1: 1, 1+L, 1+2L, ...
- … hasta L-1 (donde L es llave).

Se crea un arreglo de punteros `char **subs`, y cada subcriptograma se llena con los caracteres que le corresponden. Se
muestra cada subcriptograma (C_A, C_B, C_C, …) para que puedas ver su contenido. A nivel teórico, cada subcriptograma está 
cifrado por un César distinto (una letra de la clave).

**4) Ataque estadístico simple**

En inglés, las letras _A_, _E_, _O_, _T_ son muy frecuentes. Si un subcriptograma fue cifrado con un César de desplazamiento `K`, 
al “descifrar” mentalmente con `K` deberían aparecer muchas _A/E/O/T_ en el resultado.

Para cada desplazamiento `K (0..25)` se suma cuántas letras hay en las posiciones `K + {0, 4, 14, 19} (mod 26)`, que corresponden 
a _A_, _E_, _O_, _T_. Los `K` con mayor suma son candidatos a la letra de la clave en esa posición (la letra es `'A' + K`).

Se imprimen los TOP `TOP_CAND` candidatos por cada subcriptograma y una clave sugerida tomando el mejor de cada posición.
Ojo: no se descifra el texto aquí; solo se proponen posibles llaves.




