#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

int main() {

    int m, n;
    cout << "Matriz booleana A\n";
    cout << "Ingresa numero de filas (m): ";
    while (!(cin >> m) || m <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Valor invalido. Ingresa m > 0: ";
    }

    cout << "Ingresa numero de columnas (n): ";
    while (!(cin >> n) || n <= 0) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Valor invalido. Ingresa n > 0: ";
    }

    vector<vector<int>> A(m, vector<int>(n, 0));

    cout << "\nIngresa A fila por fila (solo 0 o 1):\n";
    for (int i = 0; i < m; ++i) {
        cout << "Fila " << (i + 1) << " (" << n << " valores): ";
        for (int j = 0; j < n; ++j) {
            int x;
            while (!(cin >> x) || (x != 0 && x != 1)) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "  Valor invalido en (" << (i+1) << "," << (j+1)
                     << "). Solo 0/1: ";
            }
            A[i][j] = x;
        }
    }

    cout << "\nA cargada (" << m << "x" << n << "):\n";
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << A[i][j] << (j + 1 == n ? '\n' : ' ');
        }
    }

    return 0;
}