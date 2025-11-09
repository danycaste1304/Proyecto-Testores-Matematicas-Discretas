#include <iostream>
#include <vector>
#include <cstdlib>   // Para rand() y srand()
#include <ctime>     // Para time()
using namespace std;

int main() {
    int filas, columnas;

    // Pedir dimensiones al usuario
    cout << "Ingrese el numero de filas (maximo 100): ";
    cin >> filas;
    cout << "Ingrese el numero de columnas (maximo 10): ";
    cin >> columnas;

    // Validar límites
    if (filas <= 0 or filas > 100 or columnas <= 0 or columnas > 10) {
        throw out_of_range("Error: dimensiones fuera de rango.");
    }

    // Semilla aleatoria para que los resultados cambien cada vez
    srand(time(nullptr));

    // Crear y llenar la matriz booleana
    vector<vector<int>> matriz(filas, vector<int>(columnas));

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = rand() % 2; // Genera 0 o 1
        }
    }

    // Mostrar la matriz
    cout << "\nMatriz booleana generada aleatoriamente:\n";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}