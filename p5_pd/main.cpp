#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "camino_heroe.h"
#include "visualizacion.h"

using namespace std;

// =============================================================================
//   ARCHIVO COMPLETO - NO HACE FALTA MODIFICAR
// =============================================================================
// Se encarga de leer la entrada, llamar a las funciones 
// en camino_heroe.cpp, y mostrar el resultado.
//
// Uso:
//   ./aventura <entrada>
//   ./aventura <entrada> <salida>
//   ./aventura <entrada> <salida> <mapa.svg>
//   ./aventura <entrada> <salida> <mapa.svg> <origen> <destino>
//
//   Si se indican origen y destino, la ruta optima entre ellos se resalta
//   en dorado sobre el mapa SVG.
// =============================================================================


bool leer_entrada(const string& fichero,
                  int& n,
                  vector<vector<int>>& C,
                  vector<int>& G) {

    ifstream f(fichero);
    if (!f.is_open()) {
        cerr << "Error: no se puede abrir el fichero '" << fichero << "'\n";
        return false;
    }

    f >> n;
    if (n <= 0) {
        cerr << "Error: el numero de localizaciones debe ser positivo.\n";
        return false;
    }

    C.assign(n, vector<int>(n, INF));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            int valor;
            f >> valor;
            if (!f) {
                cerr << "Error: matriz de costes incompleta o mal formada.\n";
                return false;
            }
            C[i][j] = (valor < 0) ? INF : valor;
        }
    }

    G.assign(n, 0);
    for (int k = 0; k < n; ++k) {
        f >> G[k];
        if (!f) {
            cerr << "Error: vector de costes de nodos incompleto.\n";
            return false;
        }
    }

    return true;
}


string formato_coste(int v) {
    return (v >= INF) ? "INF" : to_string(v);
}


bool termina_en(const string& s, const string& suf) {
    if (s.size() < suf.size()) return false;
    for (size_t i = 0; i < suf.size(); ++i) {
        char a = tolower(s[s.size() - suf.size() + i]);
        char b = tolower(suf[i]);
        if (a != b) return false;
    }
    return true;
}


void imprimir_resultado(ostream& out,
                        int n,
                        const vector<vector<int>>& C,
                        const vector<int>& G,
                        const vector<vector<int>>& D,
                        const vector<vector<int>>& P) {

    mostrar_mapa_ascii(out, C, G);

    out << "===========================================\n";
    out << "  MATRIZ DE COSTES MINIMOS\n";
    out << "===========================================\n";
    out << "      ";
    for (int j = 0; j < n; ++j) out << setw(6) << (j + 1);
    out << "\n";
    for (int i = 0; i < n; ++i) {
        out << setw(4) << (i + 1) << ": ";
        for (int j = 0; j < n; ++j) out << setw(6) << formato_coste(D[i][j]);
        out << "\n";
    }
    out << "\n";

    out << "===========================================\n";
    out << "  RUTAS OPTIMAS\n";
    out << "===========================================\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            out << "  " << (i + 1) << " -> " << (j + 1) << ": ";
            if (D[i][j] >= INF) { out << "no alcanzable\n"; continue; }
            vector<int> ruta = recuperar_ruta(P, i, j);
            if (ruta.empty()) { out << "no alcanzable\n"; continue; }
            for (size_t k = 0; k < ruta.size(); ++k) {
                out << (ruta[k] + 1);
                if (k + 1 < ruta.size()) out << " -> ";
            }
            out << "   [";
            for (size_t k = 0; k + 1 < ruta.size(); ++k) {
                int a = ruta[k], b = ruta[k + 1];
                if (k > 0) out << " + ";
                out << "C(" << (a + 1) << "->" << (b + 1) << ")=" << C[a][b];
                if (k + 2 < ruta.size())
                    out << " + G(" << (b + 1) << ")=" << G[b];
            }
            out << "]   (coste " << D[i][j] << ")\n";
        }
    }
}


void uso(const char* prog) {
    cerr << "Uso:\n"
              << "  " << prog << " <entrada>\n"
              << "  " << prog << " <entrada> <salida>\n"
              << "  " << prog << " <entrada> <salida> <mapa.svg>\n"
              << "  " << prog << " <entrada> <salida> <mapa.svg> <origen> <destino>\n";
}


int main(int argc, char* argv[]) {

    if (argc < 2 || argc == 5 || argc > 6) {
        uso(argv[0]);
        return 1;
    }

    int n;
    vector<vector<int>> C;
    vector<int> G;
    if (!leer_entrada(argv[1], n, C, G)) return 1;

    // Parsear origen y destino si se proporcionan (argv[4] y argv[5]).
    int origen = -1, destino = -1;
    if (argc == 6) {
        try {
            origen  = stoi(argv[4]) - 1;
            destino = stoi(argv[5]) - 1;
        } catch (...) {
            cerr << "Error: origen y destino deben ser enteros.\n";
            return 1;
        }
        if (origen < 0 || origen >= n || destino < 0 || destino >= n) {
            cerr << "Error: origen y destino deben estar entre 1 y "
                      << n << ".\n";
            return 1;
        }
        if (!termina_en(argv[3], ".svg")) {
            cerr << "Error: la ruta destacada solo se soporta con "
                      << "mapas .svg.\n";
            return 1;
        }
    }

    vector<vector<int>> D, P;
    el_camino_del_heroe(C, G, D, P);

    // Resultado textual.
    if (argc >= 3) {
        ofstream out(argv[2]);
        if (!out.is_open()) {
            cerr << "Error: no se puede escribir en '" << argv[2] << "'\n";
            return 1;
        }
        imprimir_resultado(out, n, C, G, D, P);
        cout << "Resultados escritos en '" << argv[2] << "'.\n";
    } else {
        imprimir_resultado(cout, n, C, G, D, P);
    }

    // Visualizacion SVG opcional.
    if (argc >= 4) {
        string archivo = argv[3];
        vector<int> ruta;
        int coste_total = -1;
        if (origen >= 0) {
            if (D[origen][destino] >= INF) {
                cerr << "Advertencia: no existe ruta de "
                          << (origen + 1) << " a " << (destino + 1)
                          << "; se genera el mapa sin destacarla.\n";
            } else {
                ruta = recuperar_ruta(P, origen, destino);
                coste_total = D[origen][destino];
            }
        }
        bool ok = generar_svg(archivo, C, G, ruta, coste_total);
        if (ok) {
            cout << "Mapa ilustrado generado en '" << archivo << "'";
            if (!ruta.empty()) {
                cout << " (con ruta " << (origen + 1)
                          << " -> " << (destino + 1)
                          << " destacada, coste " << coste_total << ")";
            }
            cout << ".\n  Abrelo directamente con un navegador.\n";
        } else {
            cerr << "Error: no se pudo escribir '" << archivo << "'.\n";
            return 1;
        }
    }

    return 0;
}
