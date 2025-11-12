# Práctica 07: Algoritmo de Euclides Extendido 

**Integrantes:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Descripción  

En esta práctica realizamos un porgrama en C para implementar el **algoritmo de Eucludes Extendido**, es una versión mejorada del algoritmo de Euclides que, además de encontrar el máximo común divisor (MCD) de dos números, también los expresa como una combinación lineal de esos números. 

Para encriptar aplicamos las siguientes operaciones:
1. *Realizar el algoritmo de Euclides normal*: Se comienza con las divisiones sucesivas para encontrar el MCD de los números \(a\) y \(b\).
2. *Despejar los residuos*: En cada paso de la división, se despeja el residuo. Por ejemplo, si se tiene la ecuación \(a=bq+r\), el residuo es \(r=a-bq\).
3. *Sustituir los residuos*: Se sustituyen los residuos en los pasos anteriores de forma iterativa. Esto se hace comenzando desde el penúltimo residuo y sustituyendo hacia arriba hasta que se pueda expresar el MCD como una combinación lineal de los números originales \(a\) y \(b\). 

---

## Implementación  

El programa implementa el **algoritmo de Euclides extendido** para calcular el *máximo común divisor (MCD)* entre dos números enteros `a` y `b`, además de obtener los coeficientes enteros x y y que satisfacen la ecuación `a·x + b·y = MCD(a, b)`. En la función `main`, el usuario introduce los valores de `a` y `b`, que se envían a la función recursiva `gcdExtended`. Esta función aplica el algoritmo de Euclides clásico al dividir repetidamente `b` entre `a` hasta que a sea cero (caso base). En ese punto, el MCD es b y los valores iniciales son `x = 0` y `y = 1`. A medida que la recursión retrocede, los valores de `x` y `y` se actualizan con las fórmulas `x = y1 - (b / a) * x1` y `y = x1`, obteniendo así los coeficientes que expresan el MCD como una combinación lineal de los números originales. Finalmente, el programa muestra el resultado del MCD, demostrando cómo el algoritmo no solo encuentra el divisor común máximo, sino también los valores de `x` y `y` asociados.

---

# Practice 07: Extended Euclidean Algorithm  

**Team Members:**  
- Bueno Hernández Héctor Daniel  
- Cruz Buenavista Lesliee Sarahí  
- Fernández Cano Iván Antonio  
- Flores Rivera Brenda Lucrecia  
- Miranda Becerril Francisco Balam  
- Villagrán Segura Denisse Abril  

---

## Description  

In this practice, we developed a C program to implement the **Extended Euclidean Algorithm**, which is an improved version of the classical Euclidean algorithm. In addition to finding the **greatest common divisor (GCD)** of two numbers, it also expresses it as a linear combination of those numbers.  

The general procedure is as follows:  
1. **Perform the standard Euclidean algorithm:** Successive divisions are made to find the GCD of the numbers \(a\) and \(b\).  
2. **Isolate the remainders:** At each step, the remainder is expressed from the division equation \(a = bq + r\), giving \(r = a - bq\).  
3. **Substitute the remainders:** The remainders are substituted back into the previous steps iteratively, starting from the second-to-last remainder, until the GCD can be expressed as a linear combination of the original numbers \(a\) and \(b\).  


---

## Implementation  

The program implements the **Extended Euclidean Algorithm** to calculate the *greatest common divisor (GCD)* between two integers `a` and `b`, as well as the integer coefficients `x` and `y` that satisfy the equation `a·x + b·y = GCD(a, b)`.  

In the `main` function, the user inputs the values of `a` and `b`, which are sent to the recursive function `gcdExtended`. This function applies the classical Euclidean algorithm by repeatedly dividing `b` by `a` until `a` becomes zero (base case). At that point, the GCD is `b` and the initial values are `x = 0` and `y = 1`.  

As the recursion unwinds, the values of `x` and `y` are updated using the formulas `x = y1 - (b / a) * x1` and `y = x1`, obtaining the coefficients that express the GCD as a linear combination of the original numbers. Finally, the program displays the GCD result along with the values of `x` and `y`, demonstrating that the algorithm not only finds the greatest common divisor but also the corresponding coefficients.  

---


