#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <string>

// =============================================================================
//   GENERADOR DE MAPAS ALEATORIOS
// =============================================================================
// Produce un fichero con el formato de entrada esperado por `aventura`:
//   - n localizaciones
//   - matriz n x n de costes directos (con -1 para "no hay camino")
//   - vector G de n costes de los guardianes
//
// El grafo generado siempre es fuertemente conectado: existe al menos un
// camino entre cualquier par de localizaciones. Para garantizarlo, el
// generador anade primero un ciclo dirigido 1->2->...->n->1 y despues
// rellena con aristas extra segun la densidad pedida.
//
// Uso:
//   ./generador <n> [<salida.txt>] [<densidad>] [<semilla>]
//
//   n         : numero de localizaciones (>= 2).
//   salida.txt: fichero donde escribir el mapa (default: stdout).
//   densidad  : probabilidad de arista extra (0..1, default 0.4).
//   semilla   : entero para reproducibilidad (default: aleatoria).
// =============================================================================


int main(int argc, char* argv[]) {

    if (argc < 2 || argc > 5) {
        std::cerr << "Uso: " << argv[0]
                  << " <n> [<salida.txt>] [<densidad 0..1>] [<semilla>]\n";
        return 1;
    }

    int n;
    try {
        n = std::stoi(argv[1]);
    } catch (...) {
        std::cerr << "Error: n debe ser un entero.\n";
        return 1;
    }
    if (n < 2) {
        std::cerr << "Error: n debe ser al menos 2.\n";
        return 1;
    }

    std::string fichero = (argc >= 3) ? argv[2] : "";

    double densidad = 0.4;
    if (argc >= 4) {
        try {
            densidad = std::stod(argv[3]);
        } catch (...) {
            std::cerr << "Error: densidad debe ser un numero.\n";
            return 1;
        }
        if (densidad < 0.0 || densidad > 1.0) {
            std::cerr << "Error: densidad debe estar entre 0 y 1.\n";
            return 1;
        }
    }

    unsigned semilla;
    if (argc >= 5) {
        try {
            semilla = static_cast<unsigned>(std::stoul(argv[4]));
        } catch (...) {
            std::cerr << "Error: semilla debe ser un entero.\n";
            return 1;
        }
    } else {
        semilla = std::random_device{}();
    }

    std::mt19937 rng(semilla);
    std::uniform_int_distribution<int> dist_coste(1, 15);
    std::uniform_int_distribution<int> dist_g(1, 8);
    std::uniform_real_distribution<double> prob(0.0, 1.0);

    // Matriz C (inicializada a -1 = "no hay camino").
    std::vector<std::vector<int>> C(n, std::vector<int>(n, -1));
    for (int i = 0; i < n; ++i) C[i][i] = 0;

    // Garantizar conectividad fuerte con un ciclo dirigido 1 -> 2 -> ... -> n -> 1.
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        C[i][j] = dist_coste(rng);
    }

    // Aristas extra aleatorias segun densidad.
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (C[i][j] != -1) continue;     // ya tiene arista.
            if (prob(rng) < densidad) {
                C[i][j] = dist_coste(rng);
            }
        }
    }

    // Costes G (entre 1 y 8 para que aparezcan los tres tipos de guardian).
    std::vector<int> G(n);
    for (int i = 0; i < n; ++i) G[i] = dist_g(rng);

    // Escribir resultado.
    std::ostream* out = &std::cout;
    std::ofstream fout;
    if (!fichero.empty()) {
        fout.open(fichero);
        if (!fout) {
            std::cerr << "Error: no se puede escribir en '" << fichero << "'.\n";
            return 1;
        }
        out = &fout;
    }

    *out << n << "\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            *out << ((j == 0) ? "" : " ") << C[i][j];
        }
        *out << "\n";
    }
    for (int i = 0; i < n; ++i) {
        *out << ((i == 0) ? "" : " ") << G[i];
    }
    *out << "\n";

    if (!fichero.empty()) {
        std::cerr << "Mapa generado en '" << fichero << "'"
                  << " (n=" << n
                  << ", densidad=" << densidad
                  << ", semilla=" << semilla << ").\n";
    }

    return 0;
}
