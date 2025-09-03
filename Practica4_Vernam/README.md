# Práctica 04: Cifrado Vernam  

**Integrantes:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Descripción  

En esta práctica realizamos un porgrama en C para implementar el **cifrado Vernam**, un método de criptografía que hace uso de una clave aleatoria de un solo uso para cifrar mensajes mediante operaciones XOR a nivel binario.

Para encriptar aplicamos las siguientes operaciones:
1. Convertimos cada letra del mensaje y de la clave generada de forma aleatoria a su valor numérico (0-25)
2. Transformamos cada valor a su representación binaria en 5 bits
3. Aplicamos la operación XOR bit a bit entre bits del mensaje y los de la clave
4. Convertimos el resultado binario de vuelta a letra para formar el mensaje cifrado

Para descifrar se sigue el mismo proceso, aplicando nuevamente la operación XOR al mensaje cifrado con la misma clave para recuperar el mensaje original.
---

## Implementación  

La implementación se diseñó utilizando **manejo de archivos** (para leer el mensaje, generar la llave y posteriormente eliminar la clave), **conversión binaria** y **operaciones bit a bit** para procesar el mensaje y la clave.

La función `valor_a_binario` se encarga de convertir un valor numérico (0-25) a una cadena binaria de 5 bits, por su parte, la función `binario_a_valor` se usa para realizar la conversión inversa de binario a valor numérico 

Una vez transformados los valores del mensaje y de la clave a binario, las funciones `cifrar_archivo` y `descifrar_archivo` trabajan procesando cada caracter del mensaje junto con la clave correspondiente, aplicando la operación XOR a nivel binario para ya sea cifrar o descifrar el mensaje. La función `descifrar_archivo` además elimina automáticamente el archivo de clave después de usarlo para implementar el principio de clave de un solo uso.

El programa principal (`main`) presenta un menú interactivo que permite al usuario elegir entre cifrar un mensaje (leyendo de M.txt y generando K.txt -- el archivo en donde se guarda la llave y Cm.txt -- el archivo en donde se guarda el mensaje cifrado) o descifrar un mensaje (leyendo de Cm.txt y K.txt, mostrando el resultado en terminal y eliminando la clave). El programa funciona hasta que el usuario decide salir.

---

# Practice 04: Vernam Cipher

**Team Members:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Description

This C program implements the **Vernam cipher**, a criptography method that consists in the use of a one-time use key that is, in this implementation, done using a random key that afterwards ciphers a message using bit-level XOR operations.

To encript our message the following operations are made:
1. Each letter from the message and the randomly generated key are converted to their numeric value (0-25).
2. Every value is then transformed into its binary representation using 5 bits.
3. The XOR operation is done bit to bit between the message and the key.
4. The resulting binary string is then converted back to letters to form the ciphered message.

To decipher the same process is done, this time applying the XOR operation to the ciphered message with the same key to retrieve the original message.

---

## Implementation

This implementation was designed using **file management** (to get the message, generate the key and also to get rid of the key), **binary conversion** and **bit to bit operations** to process both the message and the key.

The `valor_a_binario` function turns a numeric value (0-25) to a 5 bit string. The `binario_a_valor` function gets a numeric value from a binary value.

Once the values from the message and the key are transformed into binary, the functions `cifrar_archivo` and `descifrar_archivo` process each character present in both elements and then do the XOR operation that results in either ciphering or deciphering the message. The `descifrar_archivo` function also erases the file in where the key is generated after using it in order to implement the one-use key principle. 

The main program (`main`) prompts the user to choose between ciphering a message (reading said message from M.txt and generating K.txt -- the file where the key is stored and Cm.txt -- the file where the resulting ciphered message is stored) or deciphering a message (reading Cm.txt and K.txt, showing the result in the terminal and deleting the key). The programs keeps working until the user decides to close it.
