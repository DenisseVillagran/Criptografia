// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <utility>

using namespace std;

char matriz[5][5]; //Matriz generada

void generadorMatriz(string llave) {
    vector<bool> aux(26, false); // Mejor uso para un arreglo de "banderas"
    int indice_llave = 0;
    int alfabeto_actual = 'a';

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            // Rellenar con la llave
            if (indice_llave < llave.length()) {
                char caracter = llave[indice_llave];
                if (caracter == 'j') {
                    caracter = 'i';
                }
                if (!aux[caracter - 'a']) {
                    matriz[i][j] = caracter;
                    aux[caracter - 'a'] = true;
                } else {
                    // Si el carácter ya está en la matriz, no lo usamos
                    // Debemos continuar rellenando la matriz, sin avanzar el índice
                    --j; 
                }
                ++indice_llave;
            } 
            // Rellenar con el resto del alfabeto
            else {
                if (alfabeto_actual == 'j') {
                    alfabeto_actual++; // Saltar 'j'
                }
                while (aux[alfabeto_actual - 'a']) {
                    alfabeto_actual++;
                    if (alfabeto_actual == 'j') {
                        alfabeto_actual++;
                    }
                }
                matriz[i][j] = alfabeto_actual;
                aux[alfabeto_actual - 'a'] = true;
            }
        }
    }
}


string limpiarMensaje(string mensaje){
    int sizeM = mensaje.size();
    string mensajeLimpio;

    for(int i=0; i<sizeM; i++){
        if(mensaje[i] != 32){
             mensajeLimpio = mensajeLimpio + mensaje[i];
        }
    }
    return mensajeLimpio;
}

void prepararPares(string& mensaje){
    int sizeM = mensaje.size();

    for(int i=0; i<sizeM; i+=2){
        // la ultima letra quedó solita, se agrega x al final
        if(i == sizeM - 1){
            mensaje += "x";
            break;
        }

        // Ambas letras son iguales, se inserta una x entre ellas
        if(mensaje[i] == mensaje[i+1]){
            mensaje.insert(i+1, 1, 'x');
            sizeM++;
        }
    }
}

// Funcion para devolver la fila, columna de la letra que estemos buscando
pair<int, int> buscarLetra(char letra){
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            if(letra == matriz[i][j])
                return {i, j};  
        }
    }
    return {-1,-1}; // la letra no fue encontrada, error
}

string encriptado(string mensaje, string llave){
    int sizeM = mensaje.size();
    char letra1, letra2;
    pair<int,int> pos1, pos2;

    // Recorremos cada par
    for(int i=0; i<sizeM; i+=2){
        letra1 = mensaje[i];
        letra2 = mensaje[i+1];

        // Aqui ya tenemos los pares de [fila, columna]
        pos1 = buscarLetra(letra1);
        pos2 = buscarLetra(letra2);

        // Si estan en la misma fila, caemos en el caso 1
        if(pos1.first == pos2.first){
            // Nos movemos a partir de la columna, tiene que ser % 5
            // para que sea circular y no nos salgamos de rango.
            letra1 = matriz[pos1.first][ (pos1.second + 1) % 5 ];
            letra2 = matriz[pos2.first][ (pos2.second + 1) % 5 ];
        }

        // Si estan en la misma columna, caemos en el caso 2
        else if(pos1.second == pos2.second){
            letra1 = matriz[ (pos1.first + 1) % 5 ][pos1.second];
            letra2 = matriz[ (pos2.first + 1) % 5 ][pos2.second];
        }

        // Diferente fila y columna, caemos en el caso 3 (rectangulo)
        else{
            // Misma columna, fila de la otra letra

            // letra1 queda en su misma columna
            // pero se intercambia con la letra de
            // la fila que tiene la letra 2
            letra1 = matriz[pos2.first][pos1.second];

            // letra2 queda en su misma columna
            // pero se intercambia con la letra de
            // la fila que tiene la letra 1
            letra2 = matriz[pos1.first][pos2.second];
        }
        mensaje[i] = letra1;
        mensaje[i+1] = letra2;
    }

    return mensaje;
}

/*
string descencriptado(string mensaje, string llave){
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
}*/

int main() {
    string mensaje, llave;
    int opcion;

    cout << "Ingresa tu mensaje: " << endl;
    getline(cin, mensaje);
    for(int i = 0; i < mensaje.length(); i++) {
        mensaje[i] = tolower(mensaje[i]);
    }
    

    cout << "Ingresa tu llave: " << endl;
    getline(cin, llave);
    for(int i = 0; i < llave.length(); i++) {
        llave[i] = tolower(llave[i]);
    }

    // Se limpian la llave y el mensaje

    mensaje = limpiarMensaje(mensaje);
    prepararPares(mensaje);
    cout << "Mensaje: " << mensaje << endl;

    llave = limpiarMensaje(llave);
    cout << "llave: " << llave << endl; 

    do {
        cout << "Ingrese la opción que desee ejecutar:" << endl;
        cout << "1) Encriptar" << endl;
        cout << "2) Descencriptar" << endl;
        cout << "3) Salir" << endl;
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                generadorMatriz(llave);
                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 5; ++j) {
                        cout << matriz[i][j] << ' ';
                    }
                    cout << endl;
                }
                mensaje = encriptado(mensaje, llave);
                cout << "Su mensaje se encriptó con éxito, es: " << endl;
                cout << mensaje << endl;
                break;
            case 2:
                //mensaje = descencriptado(mensaje, llave);
                cout << "Su mensaje se descencriptó con éxito, es: " << endl;
                cout << mensaje << endl;
                break;
            case 3:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Ingrese otra opción: " << endl;
                break;
        }
    } while (opcion != 3);

    return 0;
}