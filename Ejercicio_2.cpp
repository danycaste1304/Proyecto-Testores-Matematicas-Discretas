#include <iostream>
#include <vector>
using namespace std;

using Matrix = vector<vector<int>>;

// ---------------------------------------------------------------------
// Suma de filas desde fila 0 hasta lastRow, considerando solo columnas "cols"
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
// Condición YYC:
// 1) #columnas >= #filas con suma == 1
// 2) cada columna tiene al menos un 1 en esas filas
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
// Función auxiliar: ordenar un vector de forma sencilla (burbuja)
// ---------------------------------------------------------------------
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

// ---------------------------------------------------------------------
// Función auxiliar: verificar si un testor contiene una columna
// ---------------------------------------------------------------------
bool testorContieneColumna(const vector<int> &t, int col) {
    for (int i = 0; i < (int)t.size(); i++) {
        if (t[i] == col) return true;
    }
    return false;
}

// ---------------------------------------------------------------------
// Función auxiliar: comparar dos testores (mismo tamaño y mismos elementos)
// ---------------------------------------------------------------------
bool sonIguales(const vector<int> &a, const vector<int> &b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// ---------------------------------------------------------------------
// Verificar si un testor ya está en la lista (para no duplicar)
// ---------------------------------------------------------------------
bool yaExisteTestor(const vector<vector<int>> &lista, const vector<int> &t) {
    for (int i = 0; i < (int)lista.size(); i++) {
        if (sonIguales(lista[i], t)) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------
// Imprimir todos los testores con notación de conjuntos {1,2,3}
// (Aquí sumamos +1 para que las columnas se vean desde 1 en vez de 0)
// ---------------------------------------------------------------------
void imprimirTestores(const vector<vector<int>> &testores) {
    if (testores.empty()) {
        cout << "(ninguno)";
        return;
    }

    for (int i = 0; i < (int)testores.size(); i++) {
        cout << "{";
        for (int j = 0; j < (int)testores[i].size(); j++) {
            cout << testores[i][j] + 1;  // +1 para notación humana
            if (j < (int)testores[i].size() - 1) cout << ",";
        }
        cout << "} ";
    }
}

// ---------------------------------------------------------------------
// MAIN: implementación simple del algoritmo YYC como lo describiste
// ---------------------------------------------------------------------
int main() {
    // EJEMPLO de matriz A (puedes reemplazarla por tu matriz)
    Matrix A = {
        {1,0,1,0},
        {0,1,1,0},
        {1,1,0,1}
    };

    int filas = A.size();
    if (filas == 0) return 0;
    int columnas = A[0].size();

    vector<vector<int>> testores; // cada testor es un vector de columnas (0-based)

    // =========================
    // 1) PRIMERA FILA
    // =========================
    for (int j = 0; j < columnas; j++) {
        if (A[0][j] == 1) {
            vector<int> t;
            t.push_back(j);  // guardamos el índice de la columna (0-based)
            testores.push_back(t);
        }
    }

    cout << "Fila considerada: 1" << endl;
    cout << "Testores tipicos (solo primera fila): ";
    imprimirTestores(testores);
    cout << endl << endl;

    // =========================
    // 2) RESTO DE FILAS (YYC)
    // =========================
    for (int i = 1; i < filas; i++) {  // i = 1 corresponde a la fila 2
        vector<vector<int>> nuevosTestores;

        // Para cada testor actual
        for (int tIndex = 0; tIndex < (int)testores.size(); tIndex++) {
            vector<int> t = testores[tIndex];
            bool seMantiene = false;

            // Verificamos si hay al menos un 1 debajo de alguna columna del testor
            for (int k = 0; k < (int)t.size(); k++) {
                int col = t[k];
                if (A[i][col] == 1) {
                    seMantiene = true;
                    break;
                }
            }

            if (seMantiene) {
                // El testor se mantiene tal cual
                if (!yaExisteTestor(nuevosTestores, t)) {
                    nuevosTestores.push_back(t);
                }
            } else {
                // No hay 1 en esta fila para las columnas del testor
                // combinamos con cada columna que tenga 1 en la fila i
                for (int j = 0; j < columnas; j++) {
                    if (A[i][j] == 1) {
                        vector<int> combinado = t;

                        if (!testorContieneColumna(combinado, j)) {
                            combinado.push_back(j);
                        }

                        // Ordenamos el testor para tener un orden consistente
                        ordenarVector(combinado);

                        // Aplicamos la condición YYC usando filas desde 0 hasta i
                        if (condicionYYC(A, combinado, i)) {
                            if (!yaExisteTestor(nuevosTestores, combinado)) {
                                nuevosTestores.push_back(combinado);
                            }
                        }
                    }
                }
            }
        }

        // Actualizamos la lista de testores típicos con los nuevos
        testores = nuevosTestores;

        cout << "Fila considerada: " << (i + 1) << endl;
        cout << "Testores tipicos hasta esta fila: ";
        imprimirTestores(testores);
        cout << endl << endl;
    }

    // =========================
    // 3) RESULTADO FINAL
    // =========================
    cout << "==== Testores tipicos finales (despues de la ultima fila) ====" << endl;
    imprimirTestores(testores);
    cout << endl;

    return 0;
}
