# Práctica 01: Cifrado César  

**Integrantes:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Descripción  

En esta práctica realizamos un programa en C para implementar el **cifrado César**, un método de criptografía clásica que desplaza cada letra del mensaje un número fijo de posiciones en el alfabeto, definido por una llave.  

Para encriptar, se recorre el texto carácter por carácter, ignorando los espacios. Si el carácter es una letra mayúscula o minúscula, se ajusta su valor ASCII para aplicar el desplazamiento circular dentro del rango de letras. El proceso de desencriptado es el inverso: se resta la llave para recuperar el texto original.  

---

## Implementación  

La implementación se diseñó utilizando un **arreglo dinámico con apuntadores** (`char*` con `malloc`) para almacenar el mensaje, lo que permite manejar cadenas de longitud variable sin depender de un arreglo estático. Al finalizar la ejecución, la memoria se libera con `free` para evitar fugas.  

Las funciones `encriptado` y `descencriptado` trabajan recorriendo el arreglo de caracteres índice por índice con un ciclo `for`.  
- En `encriptado`, cada letra se convierte a un índice de 0–25 restando su código ASCII base (`'A'` o `'a'`), se suma la llave y se aplica el módulo 26 para mantener el rango de letras, volviendo luego a ASCII.  
- En `descencriptado`, el procedimiento es el mismo, pero restando la llave para deshacer el desplazamiento.  

El programa principal (`main`) solicita al usuario el mensaje y la llave, presenta un menú para elegir acción y, según la opción seleccionada, llama a la función adecuada, mostrando el resultado hasta que el usuario decida salir.  

La implementación del cifrado César se basó en su **simplicidad y eficiencia**, permitiendo transformar el mensaje directamente sobre el mismo arreglo de caracteres sin estructuras adicionales, lo que optimiza el uso de memoria.  

El empleo de operaciones aritméticas sobre los valores ASCII facilita distinguir entre mayúsculas y minúsculas, mientras que el uso del operador módulo asegura el comportamiento circular del cifrado. Esto garantiza que el mensaje pueda ser recuperado de manera exacta al aplicar el proceso inverso. La solución es clara, directa y suficiente para comprender el principio fundamental del algoritmo.  

---

# Practice 01: Caesar Cipher  

**Team Members:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Description  

In this practice, we developed a program in C to implement the **Caesar cipher**, a classical cryptography method that shifts each letter of the message a fixed number of positions in the alphabet, defined by a key.  

To encrypt, the text is processed character by character, ignoring spaces. If the character is an uppercase or lowercase letter, its ASCII value is adjusted to apply the circular shift within the range of letters. The decryption process is the inverse: subtracting the key to recover the original text.  

---

## Implementation  

The implementation was designed using a **dynamic array with pointers** (`char*` with `malloc`) to store the message, allowing variable-length strings without depending on a static array. At the end of the execution, the memory is released with `free` to avoid leaks.  

The functions `encriptado` and `descencriptado` work by iterating through the character array index by index using a `for` loop.  
- In `encriptado`, each letter is converted to an index from 0–25 by subtracting its ASCII base (`'A'` or `'a'`), the key is added, and the result is taken modulo 26 to remain within the letter range, then converted back to ASCII.  
- In `descencriptado`, the procedure is the same, but subtracting the key to undo the shift.  

The main program (`main`) requests the message and key from the user, displays a menu to choose an action, and, depending on the option selected, calls the corresponding function, showing the result until the user decides to exit.  

The implementation of the Caesar cipher was based on its **simplicity and efficiency**, allowing the message to be transformed directly within the same character array without additional structures, thus optimizing memory usage.  

The use of arithmetic operations on ASCII values makes it easy to distinguish between uppercase and lowercase letters, while the modulo operator ensures the circular behavior of the cipher. This guarantees that the message can be recovered exactly by applying the inverse process. The solution is clear, straightforward, and sufficient to understand the fundamental principle of the algorithm.  
