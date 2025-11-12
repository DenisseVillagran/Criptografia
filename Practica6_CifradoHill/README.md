# Práctica 06: Cifrado Hill  

**Integrantes:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Descripción  

En esta práctica se implementó el **cifrado Hill**, un algoritmo clásico de cifrado por bloques que combina álgebra lineal y aritmética modular.  
Su principio se basa en representar grupos de letras como vectores numéricos y multiplicarlos por una **matriz llave** invertible módulo 26.  
El resultado se convierte nuevamente en texto cifrado, lo que genera sustituciones dependientes de múltiples letras simultáneamente.  

Este cifrado es relevante porque muestra cómo las operaciones matriciales pueden aplicarse en criptografía, sirviendo de base conceptual para los cifrados modernos basados en transformaciones lineales.  

---

## Implementación  

El programa se desarrolló en C utilizando **memoria dinámica con `malloc`** para manejar el mensaje, la llave y las matrices de tamaño variable.  
La estructura general del código consta de las siguientes partes:

### 1. Conversión de caracteres  
Cada letra del mensaje y la llave se transforma en un número entre 0 y 25 con la función `charToNum()`.  
El proceso inverso se realiza con `numToChar()` para reconstruir el texto cifrado o descifrado.

### 2. Construcción de la matriz llave  
La llave introducida por el usuario se organiza en una **matriz cuadrada** del mismo tamaño que el mensaje.  
Cada posición contiene el valor numérico correspondiente a una letra, lo que forma la base de la operación de cifrado.

### 3. Cifrado del mensaje  
El vector del mensaje se multiplica por la matriz llave mediante la función `multiplicarMatrices()`, y el resultado se reduce módulo 26.  
Así se obtiene un nuevo vector con los valores cifrados, que luego se convierten a letras para mostrar el texto encriptado.

### 4. Cálculo de la matriz inversa  
Para descifrar el mensaje, el programa calcula la **inversa modular de la matriz llave** usando:  
- Determinante (`determinanteMatriz()`)  
- Cofactores (`matrizCofactores()`)  
- Transposición (matriz adjunta)  
- Inverso modular del determinante (`modInverse()`)  

La multiplicación de la adjunta por el inverso modular del determinante produce la **matriz inversa módulo 26**, necesaria para recuperar el texto original.

### 5. Descifrado  
El texto cifrado se multiplica por la matriz inversa, obteniendo los valores originales del mensaje, que se convierten nuevamente en letras.  
Finalmente, se muestra el mensaje descifrado, validando que el proceso sea reversible.  

---

# Practice 06: Hill Cipher  

**Team Members:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Description  

In this practice, the **Hill cipher** was implemented, a classical block cipher that combines linear algebra and modular arithmetic.  
Its principle is to represent groups of letters as numeric vectors and multiply them by an **invertible key matrix** modulo 26.  
The resulting vector is then converted back into text, producing substitutions that depend on multiple letters at once.  

This cipher is significant because it demonstrates how matrix operations can be applied in cryptography and forms a conceptual foundation for modern ciphers based on linear transformations.  

---

## Implementation  

The program was developed in C using **dynamic memory allocation (`malloc`)** to handle variable-length messages, keys, and matrices.  
The overall structure of the code consists of the following parts:

### 1. Character conversion  
Each letter in the message and key is converted to a number between 0 and 25 using `charToNum()`.  
The reverse process is performed with `numToChar()` to rebuild the encrypted or decrypted text.

### 2. Key matrix construction  
The user-provided key is organized into a **square matrix** of the same size as the message.  
Each position stores the numeric value of a letter, forming the basis for the encryption operation.

### 3. Encryption process  
The message vector is multiplied by the key matrix using `multiplicarMatrices()`, and the result is reduced modulo 26.  
This produces a new numeric vector representing the encrypted message, which is then converted into text.

### 4. Inverse matrix computation  
To decrypt the message, the program computes the **modular inverse of the key matrix** using:  
- Determinant (`determinanteMatriz()`)  
- Cofactors (`matrizCofactores()`)  
- Transposition (adjugate matrix)  
- Modular inverse of the determinant (`modInverse()`)  

Multiplying the adjugate matrix by the modular inverse of the determinant yields the **inverse key matrix mod 26**, used to decrypt the ciphertext.

### 5. Decryption  
The ciphertext vector is multiplied by the inverse matrix, obtaining the original message values, which are then converted back into letters.  
Finally, the decrypted message is displayed, confirming that the process is fully reversible.
