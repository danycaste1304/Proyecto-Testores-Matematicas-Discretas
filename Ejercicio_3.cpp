#include <iostream>
#include <vector>
#include <cstdint> //Permite agregar uint64_t
#include <chrono> //Ayuda a calcular el tiempo de ejecución del algoritmo
#include <algorithm> 
#include <utility>
using namespace std;

// --------------------------- Tipos y alias ----------------------------------
// Representación: vector<int> de longitud n con valores 0/1
// - Una fila de la matriz (MB) es vector<int> fila(n)
// - Un candidato α es vector<int> alpha(n)
// - Conjunto de filas MB es vector<vector<int>>
using Row = vector<int>;
using Matrix = vector<Row>;

// ----------------------- Utilidad: imprimir vector --------------------------
void print_row(const Row &r) {
    for (int x : r) cout << x << " ";
    cout << endl;
}

// Convierte un testor típico en conjunto {i,j,k}
vector<int> row_to_set(const Row &r) {
    vector<int> S;
    for (int j = 0; j < r.size(); j++) {
        if (r[j] == 1)
            S.push_back(j + 1);   // posición humana (1-based)
    }
    return S;
}

// Imprime el conjunto {i,j,k}
void print_set(const vector<int> &S) {
    cout << "{";
    for (int i = 0; i < S.size(); i++) {
        cout << S[i];
        if (i < S.size() - 1) cout << ",";
    }
    cout << "}";
}

// ---------------------------------------------------------------------------
// FUnciones auxiliares para la Proposición 1.4 

//---------------------------------------------------------
// Convierte un row en máscara binaria (LSB = columna 1)
//---------------------------------------------------------
uint64_t row_to_mask(const Row &r) {
    int n = r.size();
    uint64_t m = 0;

    for (int j = 0; j < n; ++j) {
        if (r[j] == 1) {
            int bit = n - 1 - j;       // col j  -> bit (n-1-j)
            m |= (1ULL << bit);
        }
    }
    return m;
}


//---------------------------------------------------------
// Convierte máscara → row (n columnas)
//---------------------------------------------------------
Row mask_to_row(uint64_t m, int n) {
    Row r(n, 0);
    for (int j = 0; j < n; ++j) {
        int bit = n - 1 - j;           // bit (n-1-j) -> col j
        r[j] = ((m >> bit) & 1ULL) ? 1 : 0;
    }
    return r;
}

//---------------------------------------------------------
// Encuentra el último 1 de alpha (índice 0-based) (Ya repito esto)
//---------------------------------------------------------
int last_one_index(const Row &r) {
    // Devuelve la posición del 1 más a la derecha en la fila r
    // Convención: columnas como j=0..n-1, col1 es j=0
    for (int j = (int)r.size() - 1; j >= 0; --j)
        if (r[j] == 1)
            return j;
    return -1; // No debería ocurrir si alpha es testor
}
//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//PARTE A — Evaluación de TIPICIDAD (Paso previo a 1.4 y 1.5)
// -------------------------------------------------------
// Verifica si un vector "sub" está contenido en "sup"
// -------------------------------------------------------
bool contiene(const Row &sup, const Row &sub) {
    for (int j = 0; j < sub.size(); j++) {
        // si sub requiere un 1 en una posición donde sup tiene 0,
        // entonces sub NO está contenido en sup
        if (sub[j] == 1 && sup[j] == 0)
            return false;
    }
    return true;
}

// -------------------------------------------------------
// Verifica si alpha es típico respecto a la lista T
// -------------------------------------------------------
bool esTipico(const Row &alpha, const vector<Row> &T) {
    for (const Row &t : T) {
        // si alpha contiene a un testor típico previo,
        // entonces alpha NO es típico
        if (contiene(alpha, t))
            return false;
    }
    return true;  // ningún t está contenido → sí es típico
}

//---------------------------------------------------------
// PROPOSICIÓN 1.4 (versión operativa de BT)
// Salto cuando α es testor TÍPICO
//
// Si k es el subíndice (1-based) del último 1 en α,
// entonces los próximos (2^(n-k) - 1) n-úplos serán testores
// pero NO típicos. Por tanto el salto debe avanzar
// 2^(n-k) posiciones.
//
// Implementación:
//   1. last = índice (0-based) del último 1 en α
//   2. k = last + 1         (posición humana)
//   3. step = 2^(n - k)     (si es 0, usar step = 1)
//   4. convertir α → mask
//   5. mask = mask + step
//   6. volver a vector
//---------------------------------------------------------
Row salto_1_4(const Row &alpha) {
    int n = alpha.size();
    int last = last_one_index(alpha);   // índice 0-based

    int k = last + 1;                   // índice 1-based
    int exp = n - k;                    // exponente en la fórmula

    uint64_t step;
    if (exp <= 0) {
        // entonces 2^0 = 1 → salto mínimo de una posición
        step = 1ULL;
    } else {
        step = (1ULL << exp);          // 2^(n-k)
    }

    // Conversión α → mask
    uint64_t mask = row_to_mask(alpha);

    // Sumar el salto
    uint64_t next_mask = mask + step;

    // Si se sale del rango n bits → devolver vector nulo
    uint64_t limit = (1ULL << n);
    if (next_mask >= limit)
        return Row(n, 1);   // fin de búsqueda

    // Convertimos de vuelta a vector
    return mask_to_row(next_mask, n);
}


// ---------------------------------------------------------------------------
// Proposición 1.5 (caso NO-testor)
// Entrada:
//   alpha     = vector<int> tamaño n, con 0/1
//   MB        = matriz básica
//   offenders = índices de filas de MB que alfa NO cubre (ya calculado con Prop. 1.3)
//
// Salida:
//   new_alpha = alpha después de aplicar el salto de la Proposición 1.5
//
// Procedimiento operativo:
//
//   1. Para cada fila fallada r_i, encontrar b_i = índice del 1 más a la derecha
//   2. K = mínimo de todos los b_i  (el más a la izquierda entre los rightmost)
//   3. Construir α' como:
//
//          α'_j = α_j      si j < K
//          α'_j = 1        si j = K
//          α'_j = 0        si j > K
// ---------------------------------------------------------------------------

Row salto_1_5(const Row &alpha, const Matrix &MB, const vector<int> &offenders) {
    int n = alpha.size();
    
    // Paso 1: obtener b_i para cada fila fallada
    vector<int> b_positions;  
    for (int idx : offenders) {
        int b = last_one_index(MB[idx]);
        if (b != -1) b_positions.push_back(b);
    }

    // Paso 2: K = el más a la izquierda de esos rightmost (mínimo índice)
    int k = b_positions[0];
    for (int pos : b_positions)
        if (pos < k) k = pos;

    // Paso 3: construir α' de acuerdo a la regla de 1.5
    Row new_alpha(n);
    for (int j = 0; j < n; j++) {
        if (j < k)       new_alpha[j] = alpha[j];
        else if (j == k) new_alpha[j] = 1;
        else             new_alpha[j] = 0;
    }
    return new_alpha;
}

// ------------------ PROPOSICIÓN 1.3 (vectorial) ----------------------------
// Comprobar si alpha es testor respecto a MB
// Entrada:
//  - alpha: vector<int> tamaño n (0/1) que representa la lista α
//  - MB: matriz básica como vector de filas
//
// Salida (par):
//  - bool isTestor: true si α cubre todas las filas de MB (es testor)
//  - vector<int> offending_indices: lista de índices de filas de MB que NO son cubiertas por α
//
// Definición vectorial de "cubrir una fila r":
//   hay cobertura si existe j tal que alpha[j] == 1 && r[j] == 1

pair<bool, vector<int>> isTestor_and_offenders(const Row &alpha, const Matrix &MB) {
    int n = (int)alpha.size();
    vector<int> offenders;
    // recorrer cada fila r de MB y comprobar si alpha la cubre
    for (int i = 0; i < (int)MB.size(); ++i) {
        const Row &r = MB[i];
        bool covers = false;
        for (int j = 0; j < n; ++j) {
            if (alpha[j] == 1 && r[j] == 1) { // existe columna j que ambos tienen 1
                covers = true;
                break;
            }
        }
        if (!covers) {
            offenders.push_back(i); // guardamos índice (0-based) de fila no cubierta
        }
    }
    bool isTestor = offenders.empty();
    return {isTestor, offenders};
}

// ---------------------------------------------------------------------------
// PARTE C — CICLO PRINCIPAL DEL ALGORITMO BT
// Devuelve el conjunto de testores típicos de MB
// ---------------------------------------------------------------------------

Matrix BT(const Matrix &MB) {
    int n = MB[0].size();          // número de columnas

    // α = 00...01 (primer n-úplo que probamos)
    Row alpha(n, 0);
    alpha[n - 1] = 1;

    // Último n-úplo = 11...1
    Row ultimo(n, 1);

    // Conjunto de testores típicos
    Matrix tipicos;

    // Mientras no lleguemos a 11...1
    while (true) {

        // -----------------------------
        // Proposición 1.3
        // ¿alpha es testor?
        // -----------------------------
        pair<bool, vector<int>> resultado = isTestor_and_offenders(alpha, MB);
        bool esTestor = resultado.first;
        vector<int> offenders = resultado.second;

        if (!esTestor) {
            // -----------------------------------------------
            // Caso NO TESTOR → aplicar Proposición 1.5
            // -----------------------------------------------
            if (!offenders.empty()) {
                alpha = salto_1_5(alpha, MB, offenders);
            } else {
                // Caso raro: no cubre nada, avanzamos 1
                uint64_t mask = row_to_mask(alpha);
                mask += 1ULL;
                alpha = mask_to_row(mask, n);
            }

        } else {
            // -----------------------------------------------
            // Caso SÍ ES TESTOR → evaluar tipicidad
            // -----------------------------------------------
            if (esTipico(alpha, tipicos)) {
                // α ES TÍPICO → guardarlo
                tipicos.push_back(alpha);

                // ---------------------------------------
                // Aplicar salto 1.4 (salto por típicos)
                // ---------------------------------------
                alpha = salto_1_4(alpha);

            } else {
                // α es testor PERO NO típico → siguiente n-úplo
                uint64_t mask = row_to_mask(alpha);
                mask += 1ULL;
                alpha = mask_to_row(mask, n);
            }
        }

        // -----------------------------------------------------------
        // Condición de salida: si llegamos al último n-úplo (111...1)
        // -----------------------------------------------------------
        if (alpha == ultimo) break;
    }

    return tipicos;
}


int main(){

    Matrix A = {
        {1,0,1,0,1,0},
        {1,1,0,0,1,0},
        {0,1,1,0,1,1},
        {1,0,0,1,0,1},
        {0,1,0,1,1,0},
        {1,0,1,0,0,1},
        {0,1,1,1,0,0}
    };

    cout << "Matriz A: " << endl;

    for (auto &row : A) {
        print_row(row);
    }

    cout << "\nEjecutando algoritmo...\n\n";

    //Inicio de tiempo de ejecución
    auto inicio = chrono::high_resolution_clock::now();
    
    // Ejecutar BT
    Matrix tipicos = BT(A);

    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double> tiempo = fin - inicio;

    cout << "TESTORES TIPICOS ENCONTRADOS\n";
    for (const Row &t : tipicos) {
        print_row(t);                    // forma vectorial 0/1
    }

    cout << "\nConjuntos de testores tipicos:" << endl;
    for (const Row &t : tipicos) {
        Row S = row_to_set(t);
        print_set(S);
        cout << "\n";
    }

    // TIEMPO
    cout << "\nTiempo de ejecucion: " 
         << tiempo.count() << " segundos.\n";

    return 0;
}