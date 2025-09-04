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

El cifrado Vigenère es un método clásico de criptografía que emplea una clave de varias letras para desplazar cada carácter del mensaje de acuerdo con la posición correspondiente de esa clave. A diferencia del cifrado César, en el que el desplazamiento es fijo, en Vigenère el desplazamiento cambia en función de la letra de la clave, repitiéndose la clave a lo largo del texto. Esto lo hace más resistente que César frente a ataques simples de frecuencia.

**Criptoanálisis Kasiski**

El método de Kasiski (F. Kasiski, 1863) es una técnica clásica para atacar el cifrado de Vigenère. Se basa en que secuencias 
de letras repetidas del texto cifrado suelen originarse en la misma palabra del texto plano que quedó alineada con la misma 
porción de la clave. Midiendo la distancia entre esas repeticiones y calculando su MCD, se estima la longitud de la clave. 
Con esa longitud, el criptograma se divide en subcriptogramas (uno por cada posición de la clave) y cada uno se analiza como 
un César independiente usando frecuencias del idioma. Es una técnica simple y efectiva con textos de tamaño razonable.

---

## Implementación

**Cifrado Vigenère**

La implementación del cifrado Vigenère se realizó en C utilizando arreglos dinámicos con `malloc` para almacenar tanto el mensaje como la llave, permitiendo manejar cadenas de longitud variable sin depender de arreglos estáticos. El programa convierte a minúscula la llave (`tolower`) y decide el desplazamiento para cada carácter en función de la letra de la clave.  

- En la función `encriptado`, se recorre el mensaje carácter por carácter, se verifica si es una letra (`isalpha`) y se calcula la base ASCII (`'A'` o `'a'`) para aplicar el desplazamiento correspondiente: `(mensaje[i] - base + desplazamiento) % 26`.
- En la función `descencriptado`, se realiza el proceso inverso: `(mensaje[i] - base - desplazamiento + 26) % 26` para recuperar el texto original.

El `main` solicita al usuario el mensaje y la llave, y presenta un menú con tres opciones: encriptar, descencriptar y salir. Dependiendo de la opción, llama a la función adecuada y muestra el resultado. Al finalizar, se libera la memoria reservada para evitar fugas.


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

**2) Encontramos divisores comunes para determinar la longitud de la clave**

Toma todas las distancias y calcula su Máximo Común Divisor usando Euclides (`mcd`). El MCD de esas distancias suele ser la 
longitud de la clave (cuántas letras tiene). Se imprime el vector de distancias y el MCD, guardado en `int llave`.

Para un mejor funcionamiento, también dejamos los divisores de las distancias y seleccionamos los más frecuentes para mejorar la detección de la longitud real de la clave, en especial cuando hay múltiples repeticiones con diferentes distancias.

**3) Construir subcriptogramas según la llave**

Divide el criptograma en llave subcriptogramas, uno por cada posición de la clave:
- Sub 0: posiciones 0, L, 2L, ...
- Sub 1: 1, 1+L, 1+2L, ...
- … hasta L-1 (donde L es llave).

Se crea un arreglo de punteros `char **subs`, y cada subcriptograma se llena con los caracteres que le corresponden. Se
muestra cada subcriptograma (C_A, C_B, C_C, …) para que puedas ver su contenido. A nivel teórico, cada subcriptograma está 
cifrado por un César distinto (una letra de la clave).

**4) Ataque estadístico con chi-cuadrado**

En cada sucriptograma probamos cada posible desplazamiento (0-25) y calculamos el estadístico chi-cuadrado, el cual compara la distribución de frecuencias del terxto descifrado con las frecuencias esperadas del español. El desplazamiento que produce el valor chi-cuadrado más bajo se usa como letra de clave para la posición.

**5) Descifrado de los posibles mensajes**

Una vez estimada la clave completa, se procede a descifrar el mensaje completo utilizando el cifrado Vigenère inverso, mostrando el texto plano recuperado de cada una de las posibles claves.


---

# Practice 02: Vigenère Cipher and Kasiski Cryptanalysis

**Team Members:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Description

**Vigenère Cipher**

The Vigenère cipher is a classical encryption method that uses a multi-letter key to shift each character of the message according to the corresponding position of that key. Unlike the Caesar cipher, where the shift is fixed, Vigenère’s shift changes based on the key letter and repeats throughout the text. This makes it more resistant to simple frequency analysis attacks.

**Kasiski Cryptanalysis**

The Kasiski method (F. Kasiski, 1863) is a classical technique to attack the Vigenère cipher. It relies on the fact that repeated sequences of letters in the ciphertext often come from the same plaintext word aligned with the same portion of the key. By measuring the distance between those repetitions and calculating their GCD, one can estimate the key length. With that length, the ciphertext is divided into subciphertexts (one per key position) and each is analyzed as an independent Caesar cipher using language frequencies. It is a simple and effective technique for reasonably sized texts.

---

## Implementation

**Vigenère Cipher**

The Vigenère cipher was implemented in C using dynamic arrays with `malloc` to store both the message and the key, allowing variable-length strings without depending on static arrays. The program converts the key to lowercase (`tolower`) and computes the shift for each character based on the key letter.  

- In the `encriptado` function, the message is iterated character by character, checked if it’s a letter (`isalpha`), and the ASCII base (`'A'` or `'a'`) is computed to apply the appropriate shift: `(mensaje[i] - base + shift) % 26`.
- In the `descencriptado` function, the inverse process is performed: `(mensaje[i] - base - desplazamiento + 26) % 26` to recover the original text.

The `main` function asks the user for the message and the key, and presents a menu with three options: encrypt, decrypt, and exit. Depending on the option selected, it calls the corresponding function and displays the result. At the end, the allocated memory is freed to avoid leaks.

**Kasiski Cryptanalysis**

The implementation uses a dynamic array with `malloc` to store the ciphertext, allowing variable-length strings without relying on a static array. The input string should not contain spaces. Then it converts everything to uppercase (`toupper`) to work only with the alphabet `A..Z`, which avoids handling lowercase or out-of-range characters.

**1) Search for 3-letter repetitions and store distances**

The text is traversed with two indices (`i`, `j`) comparing trigrams `strncmp(&mensaje[i], &mensaje[j], 3)`. In Kasiski, repetitions of 3 letters usually indicate that the same plaintext word coincided with the same part of the key.

Each time a match is found, the distance `d = j - i` is calculated and stored in a dynamic vector `int *distancias`. A list of distances between repetitions is thus obtained.

**2) Find common divisors to determine the key length**

All distances are taken and their Greatest Common Divisor is calculated using Euclid’s algorithm (`mcd`). The GCD of those distances is usually the key length (how many letters it has). The vector of distances and the GCD are printed, stored in `int llave`.

For better accuracy, divisors of the distances are also considered and the most frequent ones are selected to improve detection of the actual key length, especially when there are multiple repetitions with different distances.

**3) Build subciphertexts according to the key length**

The ciphertext is divided into key-length subciphertexts, one for each key position:
- Sub 0: positions 0, L, 2L, ...
- Sub 1: 1, 1+L, 1+2L, ...
- … up to L-1 (where L is the key length).

An array of pointers `char **subs` is created, and each subciphertext is filled with its corresponding characters. Each subciphertext (C_A, C_B, C_C, …) is displayed so you can see its content. Theoretically, each subciphertext is encrypted with a different Caesar shift (one key letter).

**4) Statistical attack with chi-square**

In each subciphertext, every possible shift (0-25) is tried and the chi-square statistic is computed, which compares the frequency distribution of the decrypted text with the expected frequencies in Spanish. The shift producing the lowest chi-square value is used as the key letter for that position.

**5) Decryption of possible messages**

Once the full key is estimated, the entire message is decrypted using the inverse Vigenère cipher, showing the plaintext recovered for each possible key.
