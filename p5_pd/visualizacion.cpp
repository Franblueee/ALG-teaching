#include "visualizacion.h"
#include "camino_heroe.h"

#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <set>
#include <utility>

using namespace std;

// =============================================================================
//   MODULO DE VISUALIZACION
// =============================================================================
//   mostrar_mapa_ascii: representacion textual por pantalla o fichero.
//   generar_dot:        archivo Graphviz simple convertible a PNG/SVG.
//   generar_svg:        mapa ilustrado con iconos para cada localizacion
//                       y guardian, exportado como SVG.
// =============================================================================


// -----------------------------------------------------------------------------
//   ASCII
// -----------------------------------------------------------------------------

void mostrar_mapa_ascii(ostream& out,
                        const vector<vector<int>>& C,
                        const vector<int>& G) {

    int n = static_cast<int>(C.size());

    out << "===========================================\n";
    out << "  MAPA DE LA AVENTURA\n";
    out << "===========================================\n";

    out << "  Localizaciones (" << n << "):\n";
    for (int i = 0; i < n; ++i) {
        out << "    [" << (i + 1) << "]  guardian con coste G = " << G[i] << "\n";
    }
    out << "\n";

    out << "  Caminos directos:\n";
    bool alguno = false;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (C[i][j] >= INF) continue;
            out << "    [" << (i + 1) << "] --(" << setw(2) << C[i][j]
                << ")--> [" << (j + 1) << "]\n";
            alguno = true;
        }
    }
    if (!alguno) {
        out << "    (no hay caminos directos)\n";
    }
    out << "\n";
}


// -----------------------------------------------------------------------------
//   SVG ILUSTRADO
// -----------------------------------------------------------------------------

namespace {

constexpr double PI = 3.14159265358979323846;

// Tipos de localizacion (asignados ciclicamente segun indice).
const char* tipo_localizacion(int i) {
    static const char* tipos[] = {
        "castillo", "taberna", "bosque", "cripta", "mazmorra",
        "templo", "pueblo", "montana", "lago", "torre"
    };
    return tipos[i % 10];
}

const char* tipo_guardian(int g) {
    if (g <= 2) return "slime";
    if (g <= 5) return "goblin";
    return "dragon";
}

// Todos los iconos como simbolos SVG reutilizables. Cada uno tiene
// viewBox centrado en (0,0) para facilitar el posicionamiento.
const char* DEFS_ICONOS = R"SVG(
  <!-- Marcador de flecha para las aristas -->
  <marker id="flecha" viewBox="0 0 10 10" refX="9" refY="5"
          markerWidth="7" markerHeight="7" orient="auto">
    <path d="M0,0 L10,5 L0,10 z" fill="#5d4037"/>
  </marker>

  <!-- Marcador de flecha para la ruta destacada -->
  <marker id="flecha-ruta" viewBox="0 0 10 10" refX="9" refY="5"
          markerWidth="8" markerHeight="8" orient="auto">
    <path d="M0,0 L10,5 L0,10 z" fill="#e65100"/>
  </marker>

  <!-- ===== LOCALIZACIONES ===== -->

  <!-- CASTILLO -->
  <symbol id="ic-castillo" viewBox="-30 -30 60 60">
    <rect x="-20" y="-5" width="40" height="28" fill="#a1887f" stroke="#4e342e" stroke-width="1.2"/>
    <rect x="-24" y="-16" width="9" height="39" fill="#8d6e63" stroke="#4e342e" stroke-width="1.2"/>
    <rect x="15" y="-16" width="9" height="39" fill="#8d6e63" stroke="#4e342e" stroke-width="1.2"/>
    <rect x="-24" y="-20" width="3" height="5" fill="#8d6e63"/>
    <rect x="-19" y="-20" width="3" height="5" fill="#8d6e63"/>
    <rect x="15" y="-20" width="3" height="5" fill="#8d6e63"/>
    <rect x="20" y="-20" width="3" height="5" fill="#8d6e63"/>
    <rect x="-4" y="-9" width="3" height="4" fill="#a1887f"/>
    <rect x="2" y="-9" width="3" height="4" fill="#a1887f"/>
    <path d="M-6,23 L-6,10 Q-6,4 0,4 Q6,4 6,10 L6,23 Z" fill="#3e2723"/>
    <line x1="-19" y1="-20" x2="-19" y2="-28" stroke="#4e342e" stroke-width="1"/>
    <polygon points="-19,-28 -11,-25 -19,-22" fill="#c62828"/>
  </symbol>

  <!-- TABERNA -->
  <symbol id="ic-taberna" viewBox="-30 -30 60 60">
    <rect x="-18" y="-3" width="36" height="25" fill="#cd853f" stroke="#4e342e" stroke-width="1.2"/>
    <polygon points="-22,-3 0,-20 22,-3" fill="#8b4513" stroke="#4e342e" stroke-width="1.2"/>
    <rect x="-4" y="9" width="8" height="13" fill="#3e2723"/>
    <circle cx="3" cy="15" r="0.8" fill="#ffc107"/>
    <rect x="-14" y="2" width="7" height="6" fill="#fff59d" stroke="#4e342e" stroke-width="0.8"/>
    <line x1="-14" y1="5" x2="-7" y2="5" stroke="#4e342e" stroke-width="0.5"/>
    <line x1="-10.5" y1="2" x2="-10.5" y2="8" stroke="#4e342e" stroke-width="0.5"/>
    <rect x="7" y="2" width="7" height="6" fill="#fff59d" stroke="#4e342e" stroke-width="0.8"/>
    <line x1="7" y1="5" x2="14" y2="5" stroke="#4e342e" stroke-width="0.5"/>
    <line x1="10.5" y1="2" x2="10.5" y2="8" stroke="#4e342e" stroke-width="0.5"/>
    <line x1="-13" y1="-3" x2="-13" y2="-10" stroke="#4e342e"/>
    <rect x="-17" y="-15" width="8" height="6" fill="#f5deb3" stroke="#4e342e" stroke-width="0.8"/>
  </symbol>

  <!-- BOSQUE -->
  <symbol id="ic-bosque" viewBox="-30 -30 60 60">
    <polygon points="-13,20 -22,-3 -4,-3" fill="#1b5e20" stroke="#0d3311" stroke-width="0.8"/>
    <polygon points="-13,7 -20,-13 -6,-13" fill="#2e7d32" stroke="#0d3311" stroke-width="0.8"/>
    <polygon points="-13,-3 -18,-22 -8,-22" fill="#388e3c" stroke="#0d3311" stroke-width="0.8"/>
    <rect x="-14" y="17" width="2" height="6" fill="#5d4037"/>
    <polygon points="0,23 -10,-6 10,-6" fill="#1b5e20" stroke="#0d3311" stroke-width="0.8"/>
    <polygon points="0,8 -8,-18 8,-18" fill="#2e7d32" stroke="#0d3311" stroke-width="0.8"/>
    <polygon points="0,-6 -6,-25 6,-25" fill="#388e3c" stroke="#0d3311" stroke-width="0.8"/>
    <rect x="-1" y="20" width="2" height="6" fill="#5d4037"/>
    <polygon points="13,20 4,-3 22,-3" fill="#1b5e20" stroke="#0d3311" stroke-width="0.8"/>
    <polygon points="13,7 6,-13 20,-13" fill="#2e7d32" stroke="#0d3311" stroke-width="0.8"/>
    <rect x="12" y="17" width="2" height="6" fill="#5d4037"/>
  </symbol>

  <!-- CRIPTA -->
  <symbol id="ic-cripta" viewBox="-30 -30 60 60">
    <path d="M-20,23 L-20,-5 Q-20,-18 -10,-18 L10,-18 Q20,-18 20,-5 L20,23 Z"
          fill="#9e9e9e" stroke="#424242" stroke-width="1.2"/>
    <rect x="-1.8" y="-12" width="3.6" height="16" fill="#424242"/>
    <rect x="-6" y="-8" width="12" height="3.5" fill="#424242"/>
    <path d="M-7,23 L-7,11 Q-7,5 0,5 Q7,5 7,11 L7,23 Z" fill="#212121"/>
    <circle cx="-12" cy="-3" r="2" fill="#424242"/>
    <circle cx="12" cy="-3" r="2" fill="#424242"/>
  </symbol>

  <!-- MAZMORRA -->
  <symbol id="ic-mazmorra" viewBox="-30 -30 60 60">
    <path d="M-25,23 Q-25,-10 0,-22 Q25,-10 25,23 Z"
          fill="#6d4c41" stroke="#3e2723" stroke-width="1.2"/>
    <path d="M-11,23 Q-11,2 0,-4 Q11,2 11,23 Z" fill="#000"/>
    <polygon points="-13,-2 -10,-13 -7,-2" fill="#fff" opacity="0.85"/>
    <polygon points="-4,-1 -1,-12 2,-1" fill="#fff" opacity="0.85"/>
    <polygon points="5,-2 8,-13 11,-2" fill="#fff" opacity="0.85"/>
    <polygon points="-3,22 0,15 3,22" fill="#fff" opacity="0.85"/>
    <polygon points="-8,22 -6,17 -4,22" fill="#fff" opacity="0.85"/>
    <polygon points="4,22 6,17 8,22" fill="#fff" opacity="0.85"/>
  </symbol>

  <!-- TEMPLO -->
  <symbol id="ic-templo" viewBox="-30 -30 60 60">
    <rect x="-25" y="20" width="50" height="5" fill="#9e9e9e" stroke="#424242" stroke-width="1.2"/>
    <rect x="-22" y="17" width="44" height="3" fill="#bdbdbd" stroke="#424242" stroke-width="0.8"/>
    <rect x="-20" y="-3" width="6" height="20" fill="#eeeeee" stroke="#9e9e9e" stroke-width="0.8"/>
    <rect x="-10" y="-3" width="6" height="20" fill="#eeeeee" stroke="#9e9e9e" stroke-width="0.8"/>
    <rect x="4" y="-3" width="6" height="20" fill="#eeeeee" stroke="#9e9e9e" stroke-width="0.8"/>
    <rect x="14" y="-3" width="6" height="20" fill="#eeeeee" stroke="#9e9e9e" stroke-width="0.8"/>
    <rect x="-22" y="-7" width="44" height="4" fill="#bdbdbd" stroke="#424242" stroke-width="0.8"/>
    <polygon points="-25,-7 0,-22 25,-7" fill="#cfd8dc" stroke="#424242" stroke-width="1.2"/>
    <circle cx="0" cy="-13" r="2.5" fill="#ffd54f" stroke="#f57f17" stroke-width="0.8"/>
  </symbol>

  <!-- PUEBLO -->
  <symbol id="ic-pueblo" viewBox="-30 -30 60 60">
    <rect x="-23" y="5" width="14" height="18" fill="#a0522d" stroke="#4e342e" stroke-width="1"/>
    <polygon points="-24,5 -16,-4 -8,5" fill="#8b4513" stroke="#4e342e" stroke-width="1"/>
    <rect x="-18" y="15" width="4" height="8" fill="#3e2723"/>
    <rect x="-7" y="-2" width="16" height="25" fill="#cd853f" stroke="#4e342e" stroke-width="1"/>
    <polygon points="-8,-2 1,-15 10,-2" fill="#8b4513" stroke="#4e342e" stroke-width="1"/>
    <rect x="-1" y="14" width="5" height="9" fill="#3e2723"/>
    <rect x="-4" y="3" width="4" height="4" fill="#fff59d" stroke="#4e342e" stroke-width="0.5"/>
    <rect x="11" y="8" width="12" height="15" fill="#a0522d" stroke="#4e342e" stroke-width="1"/>
    <polygon points="10,8 17,0 24,8" fill="#8b4513" stroke="#4e342e" stroke-width="1"/>
    <rect x="15" y="15" width="4" height="8" fill="#3e2723"/>
  </symbol>

  <!-- MONTANA -->
  <symbol id="ic-montana" viewBox="-30 -30 60 60">
    <polygon points="-25,23 -8,-15 8,23" fill="#757575" stroke="#424242" stroke-width="1.2"/>
    <polygon points="2,23 6,-22 25,23" fill="#616161" stroke="#424242" stroke-width="1.2"/>
    <polygon points="-8,-15 -3,-4 -14,-4" fill="#fff" opacity="0.92"/>
    <polygon points="6,-22 12,-6 0,-6" fill="#fff" opacity="0.92"/>
  </symbol>

  <!-- LAGO -->
  <symbol id="ic-lago" viewBox="-30 -30 60 60">
    <ellipse cx="0" cy="6" rx="25" ry="15" fill="#90a955" stroke="#558b2f" stroke-width="1"/>
    <ellipse cx="0" cy="6" rx="21" ry="11" fill="#4a90e2"/>
    <ellipse cx="0" cy="3" rx="18" ry="8" fill="#5fa3e8"/>
    <path d="M-15,3 Q-12,1 -9,3 T-3,3" stroke="#fff" fill="none" stroke-width="1.2" opacity="0.9"/>
    <path d="M3,9 Q6,7 9,9 T15,9" stroke="#fff" fill="none" stroke-width="1.2" opacity="0.9"/>
    <line x1="17" y1="-6" x2="15" y2="6" stroke="#558b2f" stroke-width="1.8"/>
    <ellipse cx="16" cy="-6" rx="1.5" ry="4" fill="#33691e"/>
  </symbol>

  <!-- TORRE -->
  <symbol id="ic-torre" viewBox="-30 -30 60 60">
    <rect x="-12" y="10" width="24" height="13" fill="#9e9e9e" stroke="#424242" stroke-width="1.2"/>
    <rect x="-9" y="-14" width="18" height="24" fill="#bdbdbd" stroke="#424242" stroke-width="1.2"/>
    <rect x="-12" y="-17" width="24" height="4" fill="#9e9e9e" stroke="#424242" stroke-width="1.2"/>
    <rect x="-12" y="-20" width="3" height="3" fill="#9e9e9e"/>
    <rect x="-6" y="-20" width="3" height="3" fill="#9e9e9e"/>
    <rect x="0" y="-20" width="3" height="3" fill="#9e9e9e"/>
    <rect x="6" y="-20" width="3" height="3" fill="#9e9e9e"/>
    <rect x="-2.5" y="-7" width="5" height="8" fill="#fff59d" stroke="#4e342e" stroke-width="0.5"/>
    <line x1="-2.5" y1="-3" x2="2.5" y2="-3" stroke="#4e342e" stroke-width="0.5"/>
    <line x1="0" y1="-7" x2="0" y2="1" stroke="#4e342e" stroke-width="0.5"/>
    <line x1="0" y1="-20" x2="0" y2="-28" stroke="#424242" stroke-width="1"/>
    <polygon points="0,-28 9,-25 0,-22" fill="#c62828"/>
  </symbol>

  <!-- ===== GUARDIANES ===== -->

  <!-- SLIME (G bajo) -->
  <symbol id="ic-slime" viewBox="-15 -15 30 30">
    <ellipse cx="0" cy="9" rx="11" ry="2" fill="#000" opacity="0.2"/>
    <path d="M-10,8 Q-12,-2 -8,-6 Q0,-11 8,-6 Q12,-2 10,8 Z"
          fill="#66bb6a" stroke="#1b5e20" stroke-width="1"/>
    <path d="M-8,-3 Q-6,-5 -4,-3" stroke="#fff" fill="none" stroke-width="0.8" opacity="0.7"/>
    <ellipse cx="-3" cy="0" rx="1.5" ry="2" fill="#000"/>
    <ellipse cx="3" cy="0" rx="1.5" ry="2" fill="#000"/>
    <circle cx="-3" cy="-1" r="0.4" fill="#fff"/>
    <circle cx="3" cy="-1" r="0.4" fill="#fff"/>
    <path d="M-3,4 Q0,6 3,4" stroke="#000" fill="none" stroke-width="0.8"/>
  </symbol>

  <!-- GOBLIN (G medio) -->
  <symbol id="ic-goblin" viewBox="-15 -15 30 30">
    <ellipse cx="0" cy="11" rx="9" ry="1.8" fill="#000" opacity="0.2"/>
    <rect x="-5" y="2" width="10" height="9" fill="#ef6c00" stroke="#bf360c" stroke-width="0.8"/>
    <line x1="-5" y1="3" x2="-9" y2="7" stroke="#ef6c00" stroke-width="2.2" stroke-linecap="round"/>
    <line x1="5" y1="3" x2="9" y2="7" stroke="#ef6c00" stroke-width="2.2" stroke-linecap="round"/>
    <circle cx="0" cy="-3" r="6.5" fill="#fb8c00" stroke="#bf360c" stroke-width="0.8"/>
    <polygon points="-6,-5 -10,-2 -5,-1" fill="#fb8c00" stroke="#bf360c" stroke-width="0.8"/>
    <polygon points="6,-5 10,-2 5,-1" fill="#fb8c00" stroke="#bf360c" stroke-width="0.8"/>
    <circle cx="-2.5" cy="-3" r="1.3" fill="#fff"/>
    <circle cx="2.5" cy="-3" r="1.3" fill="#fff"/>
    <circle cx="-2.5" cy="-3" r="0.7" fill="#000"/>
    <circle cx="2.5" cy="-3" r="0.7" fill="#000"/>
    <path d="M-3,1 L-2,3 L0,1 L2,3 L3,1" stroke="#000" fill="none" stroke-width="0.8"/>
  </symbol>

  <!-- DRAGON (G alto) -->
  <symbol id="ic-dragon" viewBox="-15 -15 30 30">
    <ellipse cx="0" cy="11" rx="12" ry="2" fill="#000" opacity="0.2"/>
    <ellipse cx="-2" cy="5" rx="10" ry="5" fill="#c62828" stroke="#7f0000" stroke-width="0.8"/>
    <path d="M-4,3 Q-12,-7 -14,2 Q-9,4 -4,5 Z" fill="#a01a1a" stroke="#7f0000" stroke-width="0.8"/>
    <path d="M0,3 Q-3,-5 6,-7 Q9,-3 5,3 Z" fill="#a01a1a" stroke="#7f0000" stroke-width="0.8"/>
    <path d="M6,5 Q11,1 12,-5 Q9,-9 5,-7 Q3,-3 4,2 Z" fill="#c62828" stroke="#7f0000" stroke-width="0.8"/>
    <circle cx="10" cy="-4" r="1.2" fill="#fff"/>
    <circle cx="10" cy="-4" r="0.6" fill="#000"/>
    <polygon points="13,-3 16,-2 13,-1" fill="#c62828" stroke="#7f0000" stroke-width="0.5"/>
    <path d="M12,-1 Q15,-2 17,1 Q14,0 12,1 Z" fill="#ff9800"/>
    <path d="M-10,5 Q-14,7 -15,4 Q-13,6 -10,7 Z" fill="#c62828" stroke="#7f0000" stroke-width="0.5"/>
    <polygon points="-5,-1 -3,-4 -1,-1" fill="#7f0000"/>
    <polygon points="0,-2 2,-5 4,-2" fill="#7f0000"/>
  </symbol>
)SVG";


// Calcula posiciones de los nodos en circulo.
void calcular_posiciones(int n, double cx, double cy, double r,
                         vector<double>& px, vector<double>& py) {
    px.resize(n);
    py.resize(n);
    for (int i = 0; i < n; ++i) {
        double a = 2.0 * PI * i / n - PI / 2.0;
        px[i] = cx + r * cos(a);
        py[i] = cy + r * sin(a);
    }
}

// Escribe el SVG de las aristas (caminos curvos con flechas y etiquetas).
// Las aristas presentes en `aristas_ruta` se dibujan resaltadas en dorado.
void escribir_aristas(ostream& f,
                      const vector<vector<int>>& C,
                      const vector<double>& px,
                      const vector<double>& py,
                      const set<pair<int,int>>& aristas_ruta) {
    int n = static_cast<int>(C.size());
    constexpr double RADIO_NODO = 32.0;     // Margen para no superponer flecha con icono.
    constexpr double OFFSET_CURVA = 38.0;   // Cuanto se desvia la curva del segmento recto.

    // Primera pasada: aristas normales (debajo).
    // Segunda pasada: aristas destacadas (encima, para que se vean bien).
    for (int pasada = 0; pasada < 2; ++pasada) {
        f << (pasada == 0 ? "  <g id='caminos'>\n" : "  <g id='caminos-ruta'>\n");

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i == j || C[i][j] >= INF) continue;

                bool destacada = aristas_ruta.count({i, j}) > 0;
                if ((pasada == 0 && destacada) || (pasada == 1 && !destacada)) {
                    continue;
                }

                double dx = px[j] - px[i];
                double dy = py[j] - py[i];
                double len = sqrt(dx * dx + dy * dy);
                if (len < 1e-6) continue;
                double ux = dx / len, uy = dy / len;
                double perx = uy, pery = -ux;

                double sx = px[i] + ux * RADIO_NODO;
                double sy = py[i] + uy * RADIO_NODO;
                double ex = px[j] - ux * RADIO_NODO;
                double ey = py[j] - uy * RADIO_NODO;
                double mx = (sx + ex) / 2.0 + OFFSET_CURVA * perx;
                double my = (sy + ey) / 2.0 + OFFSET_CURVA * pery;

                const char* color  = destacada ? "#e65100" : "#5d4037";
                const char* marker = destacada ? "url(#flecha-ruta)" : "url(#flecha)";
                double grosor = destacada ? 4.5 : 2.2;
                double opac   = destacada ? 1.0 : 0.85;

                f << "    <path d='M " << sx << "," << sy
                  << " Q " << mx << "," << my << " " << ex << "," << ey
                  << "' fill='none' stroke='" << color
                  << "' stroke-width='" << grosor
                  << "' stroke-linecap='round' marker-end='" << marker
                  << "' opacity='" << opac << "'/>\n";

                // Etiqueta del coste.
                double lx = (sx + ex) / 2.0 + OFFSET_CURVA * 0.85 * perx;
                double ly = (sy + ey) / 2.0 + OFFSET_CURVA * 0.85 * pery;

                const char* relleno = destacada ? "#ffe082" : "#fff8e1";
                const char* borde   = destacada ? "#e65100" : "#5d4037";
                double bw           = destacada ? 1.8 : 1.0;

                f << "    <g>\n"
                  << "      <rect x='" << lx - 13 << "' y='" << ly - 10
                  << "' width='26' height='18' rx='5' "
                  << "fill='" << relleno << "' stroke='" << borde
                  << "' stroke-width='" << bw << "'/>\n"
                  << "      <text x='" << lx << "' y='" << ly + 4
                  << "' text-anchor='middle' font-family='Georgia, serif' "
                  << "font-size='12' font-weight='bold' fill='#3e2723'>"
                  << C[i][j] << "</text>\n"
                  << "    </g>\n";
            }
        }
        f << "  </g>\n";
    }
}

// Escribe el SVG de los nodos: icono de localizacion + numero + guardian.
// Los nodos en `nodos_ruta` reciben un halo dorado.
void escribir_nodos(ostream& f,
                    const vector<int>& G,
                    const vector<double>& px,
                    const vector<double>& py,
                    const set<int>& nodos_ruta) {
    int n = static_cast<int>(G.size());
    f << "  <g id='localizaciones'>\n";
    for (int i = 0; i < n; ++i) {
        double x = px[i], y = py[i];

        // Halo dorado si el nodo esta en la ruta destacada.
        if (nodos_ruta.count(i)) {
            f << "    <circle cx='" << x << "' cy='" << y
              << "' r='38' fill='#ffe082' opacity='0.55'/>\n"
              << "    <circle cx='" << x << "' cy='" << y
              << "' r='38' fill='none' stroke='#ff8f00' stroke-width='3' "
              << "stroke-dasharray='5,3' opacity='0.85'/>\n";
        }

        // Icono de la localizacion (60x60 centrado).
        f << "    <use href='#ic-" << tipo_localizacion(i)
          << "' x='" << x - 30 << "' y='" << y - 30
          << "' width='60' height='60'/>\n";

        // Etiqueta con el numero del nodo.
        f << "    <circle cx='" << x + 24 << "' cy='" << y - 24
          << "' r='13' fill='#3e2723' stroke='#fff8e1' stroke-width='2.5'/>\n"
          << "    <text x='" << x + 24 << "' y='" << y - 19
          << "' text-anchor='middle' font-family='Georgia, serif' "
          << "font-size='14' font-weight='bold' fill='#fff8e1'>"
          << (i + 1) << "</text>\n";

        // Guardian.
        f << "    <use href='#ic-" << tipo_guardian(G[i])
          << "' x='" << x - 42 << "' y='" << y + 12
          << "' width='30' height='30'/>\n";

        // Etiqueta G=N.
        f << "    <text x='" << x << "' y='" << y + 48
          << "' text-anchor='middle' font-family='Georgia, serif' "
          << "font-size='11' fill='#3e2723' font-style='italic'>"
          << "G = " << G[i] << "</text>\n";
    }
    f << "  </g>\n";
}

// Escribe la leyenda en una esquina del mapa.
void escribir_leyenda(ostream& f, double H) {
    double x0 = 20, y0 = H - 150;

    f << "  <g id='leyenda'>\n"
      << "    <rect x='" << x0 << "' y='" << y0
      << "' width='220' height='130' rx='8' "
      << "fill='#fff8e1' stroke='#5d4037' stroke-width='1.5' opacity='0.95'/>\n"
      << "    <text x='" << x0 + 110 << "' y='" << y0 + 22
      << "' text-anchor='middle' font-family='Georgia, serif' "
      << "font-size='14' font-weight='bold' fill='#3e2723'>Guardianes</text>\n";

    f << "    <use href='#ic-slime' x='" << x0 + 15 << "' y='" << y0 + 32
      << "' width='28' height='28'/>\n"
      << "    <text x='" << x0 + 55 << "' y='" << y0 + 52
      << "' font-family='Georgia, serif' font-size='12' fill='#3e2723'>"
      << "Slime (G &lt;= 2)</text>\n";

    f << "    <use href='#ic-goblin' x='" << x0 + 15 << "' y='" << y0 + 62
      << "' width='28' height='28'/>\n"
      << "    <text x='" << x0 + 55 << "' y='" << y0 + 82
      << "' font-family='Georgia, serif' font-size='12' fill='#3e2723'>"
      << "Goblin (3 &lt;= G &lt;= 5)</text>\n";

    f << "    <use href='#ic-dragon' x='" << x0 + 15 << "' y='" << y0 + 92
      << "' width='28' height='28'/>\n"
      << "    <text x='" << x0 + 55 << "' y='" << y0 + 112
      << "' font-family='Georgia, serif' font-size='12' fill='#3e2723'>"
      << "Dragon (G &gt;= 6)</text>\n"
      << "  </g>\n";
}

// Escribe el panel con el resumen de la ruta destacada (en la esquina
// superior derecha del mapa).
void escribir_panel_ruta(ostream& f, double W,
                         const vector<int>& ruta,
                         int coste_total) {
    if (ruta.empty()) return;

    double x0 = W - 240, y0 = 105;

    f << "  <g id='panel-ruta'>\n"
      << "    <rect x='" << x0 << "' y='" << y0
      << "' width='220' height='95' rx='8' "
      << "fill='#fff8e1' stroke='#e65100' stroke-width='2' opacity='0.97'/>\n";

    int origen = ruta.front() + 1;
    int destino = ruta.back() + 1;

    f << "    <text x='" << x0 + 110 << "' y='" << y0 + 22
      << "' text-anchor='middle' font-family='Georgia, serif' "
      << "font-size='14' font-weight='bold' fill='#bf360c'>"
      << "Ruta " << origen << " &#8594; " << destino << "</text>\n";

    // Lista de nodos.
    string camino;
    for (size_t k = 0; k < ruta.size(); ++k) {
        camino += to_string(ruta[k] + 1);
        if (k + 1 < ruta.size()) camino += " > ";
    }
    f << "    <text x='" << x0 + 110 << "' y='" << y0 + 48
      << "' text-anchor='middle' font-family='Georgia, serif' "
      << "font-size='13' fill='#3e2723'>"
      << camino << "</text>\n";

    if (coste_total >= 0) {
        f << "    <text x='" << x0 + 110 << "' y='" << y0 + 76
          << "' text-anchor='middle' font-family='Georgia, serif' "
          << "font-size='13' font-style='italic' fill='#3e2723'>"
          << "Coste total: " << coste_total << "</text>\n";
    }
    f << "  </g>\n";
}

} // namespace anonimo


bool generar_svg(const string& fichero,
                 const vector<vector<int>>& C,
                 const vector<int>& G,
                 const vector<int>& ruta,
                 int coste_total) {

    ofstream f(fichero);
    if (!f.is_open()) return false;

    int n = static_cast<int>(C.size());

    // Conjuntos auxiliares para identificar aristas y nodos de la ruta.
    set<pair<int,int>> aristas_ruta;
    set<int> nodos_ruta;
    for (size_t k = 0; k < ruta.size(); ++k) {
        nodos_ruta.insert(ruta[k]);
        if (k + 1 < ruta.size()) {
            aristas_ruta.insert({ruta[k], ruta[k + 1]});
        }
    }

    // Dimensiones del lienzo y centro del mapa.
    constexpr double W = 1000.0;
    constexpr double H = 750.0;
    const double cx = W / 2.0;
    const double cy = H / 2.0 + 30.0;
    const double radio = (n <= 5) ? 240.0 : 285.0;

    vector<double> px, py;
    calcular_posiciones(n, cx, cy, radio, px, py);

    // Cabecera SVG.
    f << "<?xml version='1.0' encoding='UTF-8'?>\n"
      << "<svg xmlns='http://www.w3.org/2000/svg' "
      << "viewBox='0 0 " << W << " " << H << "' "
      << "preserveAspectRatio='xMidYMid meet' "
      << "font-family='Georgia, serif'>\n";

    // Definiciones (iconos y marcadores).
    f << "<defs>\n" << DEFS_ICONOS << "\n</defs>\n";

    // Fondo tipo pergamino.
    f << "  <defs>\n"
      << "    <radialGradient id='pergamino' cx='50%' cy='50%' r='75%'>\n"
      << "      <stop offset='0%' stop-color='#ffffff'/>\n"
      << "      <stop offset='100%' stop-color='#ffffff'/>\n"
      << "    </radialGradient>\n"
      << "  </defs>\n";
    f << "  <rect width='" << W << "' height='" << H << "' fill='url(#pergamino)'/>\n";


    // Marco decorativo.
    f << "  <rect x='15' y='15' width='" << (W - 30) << "' height='" << (H - 30)
      << "' rx='12' fill='none' stroke='#6d4c41' stroke-width='3' opacity='0.5'/>\n";
    f << "  <rect x='22' y='22' width='" << (W - 44) << "' height='" << (H - 44)
      << "' rx='8' fill='none' stroke='#6d4c41' stroke-width='1' opacity='0.4'/>\n";

    // Titulo.
    f << "  <text x='" << W / 2 << "' y='55' text-anchor='middle' "
      << "font-size='32' font-weight='bold' fill='#3e2723' "
      << "style='letter-spacing: 2px'>~ Mapa de la Aventura ~</text>\n";
    f << "  <text x='" << W / 2 << "' y='80' text-anchor='middle' "
      << "font-size='13' fill='#5d4037' font-style='italic'>"
      << "</text>\n";

    // Capas.
    escribir_aristas(f, C, px, py, aristas_ruta);
    escribir_nodos(f, G, px, py, nodos_ruta);
    escribir_leyenda(f, H);
    escribir_panel_ruta(f, W, ruta, coste_total);

    f << "</svg>\n";
    return true;
}
