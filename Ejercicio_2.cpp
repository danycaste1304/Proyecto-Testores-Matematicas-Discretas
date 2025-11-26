#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

using Matrix = vector<vector<int>>;

vector<vector<int>> ingresoMatrizBasica() {

    int filas, columnas;
    
    // Pedir dimensiones al usuario
    cout << "-------- Generacion de Matriz Booleana --------" << endl;
    cout << "Ingrese el numero de filas (maximo 100): ";
    cin >> filas;
    cout << "Ingrese el numero de columnas (maximo 10): ";
    cin >> columnas;

    // Validar límites
    if (filas <= 0 || filas > 100 || columnas <= 0 || columnas > 10) {
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

    // Mostrar la matriz booleana generada
    cout << "\nMatriz booleana generada aleatoriamente:\n";
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    // Convertir en matriz básica
    vector<bool> basicas(filas, true);

    // Recorrer cada fila i
    for (int i = 0; i < filas; i++) {

        // Si ya fue eliminada, no la comparamos
        if (!basicas[i]) continue;

        // La comparamos con todas las demás filas
        for (int k = 0; k < filas; k++) {
            if (!basicas[k] || k == i) continue;

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

            for (int c = 0; c < columnas; c++) {
                if (matriz[i][c] < matriz[k][c]) {
                    iContieneK = false;
                }
                if (matriz[k][c] < matriz[i][c]) {
                    kContieneI = false;
                }
            }

            if (iContieneK && !kContieneI) {
                basicas[i] = false;
                break;
            } else if (kContieneI && !iContieneK) {
                basicas[k] = false;
            }
        }
    }

    // Mostrar la matriz básica
    vector<vector<int>> matrizBasica;

    cout << "\nMatriz basica:\n";
    for (int i = 0; i < filas; i++) {
        if (!basicas[i]) continue;
        matrizBasica.push_back(matriz[i]);
        for (int j = 0; j < columnas; j++) {
            if (matriz[i][j] == 1) {
            }
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    return matrizBasica;
}


//Función suma de filas
vector<int> sumarFilas(const Matrix &A, const vector<int> &cols, int lastRow) {
    vector<int> sumas(lastRow + 1, 0);

    for (int i = 0; i <= lastRow; i++) {
        int suma = 0;
        for (int j = 0; j < (int)cols.size(); j++) {
            int c = cols[j];    // índice de columna (0,1,2,...)
            suma += A[i][c];    // sumo el valor A[i][c]
        }
        sumas[i] = suma;
    }

    return sumas;
}


// Condición YYC
bool condicionYYC(const Matrix &A, const vector<int> &cols, int lastRow) {
    if (cols.empty()) return false;

    vector<int> sumas = sumarFilas(A, cols, lastRow);

    // Encontrar filas donde la suma es 1
    vector<int> filasConUno;
    for (int i = 0; i <= lastRow; i++) {
        if (sumas[i] == 1) {
            filasConUno.push_back(i);
        }
    }

    int numColumnas = cols.size();

    // Condición 1
    if (numColumnas < (int)filasConUno.size()) {
        return false;
    }

    // Condición 2: cada columna debe tener al menos un 1 en esas filas
    for (int k = 0; k < numColumnas; k++) {
        int columna = cols[k];
        bool tieneUno = false;

        for (int f = 0; f < (int)filasConUno.size(); f++) {
            int fila = filasConUno[f];
            if (A[fila][columna] == 1) {
                tieneUno = true;
                break;
            }
        }

        if (!tieneUno) {
            return false;
        }
    }

    return true;
}

// Función auxiliar: ordenar un vector de forma sencilla
void ordenarVector(vector<int> &v) {
    int n = v.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[i]) {
                int temp = v[i];
                v[i] = v[j];
                v[j] = temp;
            }
        }
    }
}

//Verificar si un testor contiene una columna
bool testorContieneColumna(const vector<int> &t, int col) {
    for (int i = 0; i < (int)t.size(); i++) {
        if (t[i] == col) return true;
    }
    return false;
}

// Comparar dos testores
bool sonIguales(const vector<int> &a, const vector<int> &b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Verificar si un testor ya está en la lista
bool yaExisteTestor(const vector<vector<int>> &lista, const vector<int> &t) {
    for (int i = 0; i < (int)lista.size(); i++) {
        if (sonIguales(lista[i], t)) {
            return true;
        }
    }
    return false;
}

// Imprimir testores
void imprimirTestores(const vector<vector<int>> &testores) {
    if (testores.empty()) {
        cout << "(ninguno)";
        return;
    }

    for (int i = 0; i < (int)testores.size(); i++) {
        cout << "{";
        for (int j = 0; j < (int)testores[i].size(); j++) {
            cout << testores[i][j]; 
            if (j < (int)testores[i].size() - 1) cout << ",";
        }
        cout << "} ";
    }
}

//Main
int main() {

     
    Matrix A = {
        {1,1,1,0,0,0},
        {0,0,0,0,0,1},
        {1,0,0,1,0,0},
        {0,1,1,1,0,0},
        {1,0,0,0,1,0},
        {0,1,0,1,1,0},
    };
    /*
    Matrix A = ingresoMatrizBasica();
    */

    int filas = A.size();
    if (filas == 0) return 0;
    int columnas = A[0].size();

    // Tiempo total desde el inicio
    auto inicioGlobal = high_resolution_clock::now();

    vector<vector<int>> testores;

    // PRIMERA FILA
    for (int j = 0; j < columnas; j++) {
        if (A[0][j] == 1) {
            vector<int> t;
            t.push_back(j);
            testores.push_back(t);
        }
    }
    
    cout << "\nYYC\n";
    cout << "Fila considerada: 1\n";
    cout << "Testores tipicos (solo primera fila): ";
    imprimirTestores(testores);
    cout << "\n";
    
    auto t1 = high_resolution_clock::now();
    duration<double> total1 = t1 - inicioGlobal;
    cout << "Tiempo acumulado: " << total1.count() << " s\n\n";


    // RESTO DE FILAS
    for (int i = 1; i < filas; i++) {

        auto t0 = high_resolution_clock::now(); // tiempo parcial

        vector<vector<int>> nuevosTestores;

        for (int ti = 0; ti < (int)testores.size(); ti++) {
            vector<int> t = testores[ti];
            bool seMantiene = false;

            for (int k = 0; k < (int)t.size(); k++) {
                int col = t[k];
                if (A[i][col] == 1) {
                    seMantiene = true;
                    break;
                }
            }

            if (seMantiene) {
                if (!yaExisteTestor(nuevosTestores, t)) {
                    nuevosTestores.push_back(t);
                }
            } else {
                for (int j = 0; j < columnas; j++) {
                    if (A[i][j] == 1) {
                        vector<int> combinado = t;

                        if (!testorContieneColumna(combinado, j)) {
                            combinado.push_back(j);
                        }

                        ordenarVector(combinado);

                        if (condicionYYC(A, combinado, i)) {
                            if (!yaExisteTestor(nuevosTestores, combinado)) {
                                nuevosTestores.push_back(combinado);
                            }
                        }
                    }
                }
            }
        }

        testores = nuevosTestores;

        auto t1 = high_resolution_clock::now();
        duration<double> tiempoParcial = t1 - t0;
        duration<double> tiempoTotal   = t1 - inicioGlobal;

        cout << "Fila considerada: " << (i + 1) << "\n";
        cout << "Testores tipicos hasta esta fila: ";
        imprimirTestores(testores);
        cout << "\n";

        cout << "Tiempo parcial: " << tiempoParcial.count() << " s\n";
        cout << "Tiempo acumulado: " << tiempoTotal.count() << " s\n\n";
    }

    // RESULTADO FINAL
    cout << "==== Testores tipicos finales ====\n";
    imprimirTestores(testores);
    cout << "\n";

    return 0;
}
