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

    //Convertir en matriz básica
   vector<bool> basicas(filas, true);

    // Recorrer cada fila i
    for (int i = 0; i < filas; i++) {

        //si ya fue eliminada, no la comparamos
        if (!basicas[i]) continue;

        // la comparamos con las filas que vienen después
        for (int k = 0; k < filas; k++) {
            if (!basicas[k]||k==i) continue;

            bool hayDeterminante = false;

    // Revisamos columnas de 2 en 2 (c y c+1)
            for (int c = 0; c < columnas - 1; c++) {
                int a1 = matriz[i][c];
                int b1 = matriz[k][c];
                int a2 = matriz[i][c+1];
                int b2 = matriz[k][c+1];

                // patrón 0,1 seguido de 1,0
                bool pat1 = (a1 == 0 && b1 == 1) && (a2 == 1 && b2 == 0);
                // patrón 1,0 seguido de 0,1
                bool pat2 = (a1 == 1 && b1 == 0) && (a2 == 0 && b2 == 1);

                if (pat1 || pat2) {
                    hayDeterminante = true;
                    break;  
                }
            }

            if (hayDeterminante) {
                // no se elimina ninguna de las dos, pero seguimos comparando i con la que sigue
                continue;
            }
           
            // si NO hay determinante, entonces una contiene a la otra.
            bool iContieneK = true;
            bool kContieneI = true;
            bool iguales = true;

            for (int c = 0; c < columnas; c++) {
                if (matriz[i][c] < matriz[k][c]) {
                    iContieneK = false;
                }
                if (matriz[k][c] < matriz[i][c]) {
                    kContieneI = false;
                }
                if (matriz[i][c] != matriz[k][c]) {
                    iguales = false;
                }
            }

            if (iContieneK && !kContieneI) {
                basicas[i] = false;
                break;      
            } else if ((kContieneI && !iContieneK)||(iguales)) {
                basicas[k] = false;
            } 
        }
    }

    // Mostrar la matriz básica y calcular densidad
    float filas_den=0;
    float unos=0;
    cout << "\nMatriz basica:\n";
    for (int i = 0; i < filas; i++) {
        if (!basicas[i]) continue;
        filas_den++;
        for (int j = 0; j < columnas; j++) {
            if(matriz[i][j]==1){
                unos++;
            }
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    float densidad;
    densidad=unos/(filas_den*columnas);
    cout<<"Densidad: "<<densidad<<endl;

    return 0;
}
