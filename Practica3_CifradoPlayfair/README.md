# Práctica 03: Cifrado Playfair  

**Integrantes:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Descripción  

En esta práctica realizamos un porgrama en C++ para implementar el **cifrado Playfair**, un método de criptografía que hace uso de una matriz generada a partir de una llave dada para cifrará el mensaje por pares de letras.

Para encriptar, se deben aplicar las siguientes tres reglas sobre la matriz generada:
1. Si las letras se encuentran en el mismo renglón, la letra se transforma en la letra de la derecha.
2. Si están en distintas columas/renglones, se hace un cuadrado usando las letras elegidas como esquinas. Las letras se convierten en esquinas opuestas.
3. Si las letras se encuentran en la misma columna, la letra se transforma en la letra de abajo.

---

## Implementación  

La implementación se diseñó utilizando una **matriz de caracteres, vectores auxiliares** y **variables string** para almacenar el mensaje y la llave, lo que nos permitió hacer ajustes a las entradas ingresadas por el usuario como obtener el largo de la cadena y pasar la cadena a minúsculas.  

La funciones `limpiarMensaje` y `prepararPares` se usan antes del proceso de encriptado para preparar el texto plano. 
- En `limpiarMensaje` se eliminan los espacios en blanco del mensaje ingresado.
- En `prpararPares` se separa el mensaje en pares de letras para cumplir con las especificaciones del cifrado. Además, cuando se tiene un par con letras iguales, se agrega una 'x' entre ambas y si el mensaje tiene una cantidad impar de letras, al último par se le agrega una 'x'.

Posterormente, las funciones `encriptado` y `desencriptado` trabajan recorriendo la matriz generada siguiendo las reglas antes mencionadas basándose en la posición de cada par de letras en la matriz. Las reglas para el proceso de descrifrado son las mismas, solo que se aplican a la inversa junto con la función `eliminarX` para recuperar el mensaje original.  

El programa principal (`main`) solicita al usuario el mensaje y la llave, presenta un menú para elegir acción y, según la opción seleccionada, llama a la función adecuada, mostrando el resultado hasta que el usuario decida salir.  

---

# Practice 03: Playfair Cipher  

**Team Members:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Description

This program implements the **Playfair cipher**, a polyalphabetic substitution cipher that uses a 5x5 key square to encrypt messages two letters at a time. 

To encrypt a message, we apply one of the following three rules based on the letters' positions in the key square:
1. If the letters are on the same row, they're replaced by the letters immediately to their right (wrapping around to the start of the row if necessary).
2. If the letters form a rectangle, they're replaced by the letters on the opposite corners of that rectangle.
3. If the letters are on the same column, they're replaced by the letters immediately below them (wrapping around to the top of the column if necessary).

---

## Implementation

The program is built using a **character matrix, auxiliary vectors**, and **string variables** to store the message and key. This design allows for easy manipulation of user input, such as obtaining string length and converting the text to lowercase.

The `limpiarMensaje` and `prepararPares` functions are used to prepare the plaintext before encryption.
- The `limpiarMensaje` function removes all spaces from the user's input.
- The `prepararPares` function splits the message into pairs of letters to meet the cipher's specifications. It also inserts an 'x' between identical letters in a pair and appends an 'x' to the last letter if the message has an odd number of characters.

Following this, the `encriptado` and `desencriptado` functions process the message by traversing the matrix according to the rules mentioned above, based on each pair's position. The decryption rules are the inverse of the encryption rules and are used in conjunction with the `eliminaX` function to recover the original plaintext.

The main program (`main`) prompts the user for a message and a key, displays a menu to choose an action, and then calls the appropriate function to show the result until the user decides to exit.
