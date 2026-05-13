# Práctica 5 — El viaje del héroe

Tu única tarea es implementar la función el_camino_del_heroe en **`camino_heroe.cpp`**.

---

## Archivos

| Archivo | Descripción |
|---|---|
| `camino_heroe.cpp` | **Aquí implementas el algoritmo.** |
| `camino_heroe.h` | Declaraciones. No modificar. |
| `main.cpp` | E/S y formato de salida. No modificar. |
| `visualizacion.h/.cpp` | Dibuja el mapa en SVG. No modificar. |
| `generador.cpp` | Genera mapas aleatorios. |
| `ejemploN.txt` / `ejemploN_salida.txt` | Casos de prueba y salidas esperadas. |

---

## Compilación y ejecución

```bash
make                # compila todo
make run            # ejecuta con ejemplo 1 (salida por pantalla)
make ejemplos       # ejecuta los 3 ejemplos y genera SVGs con ruta destacada
make aleatorio      # mapa aleatorio de 6 nodos
make clean          # elimina archivos generados
```

El SVG se puede abrir directamente con cualquier navegador.

---

## Uso del solver

```bash
./aventura <entrada>
./aventura <entrada> <salida.txt>
./aventura <entrada> <salida.txt> <mapa.svg>
./aventura <entrada> <salida.txt> <mapa.svg> <origen> <destino>
```

---

## Formato de entrada

```
n
C[1][1] C[1][2] ... C[1][n]
...
C[n][1] C[n][2] ... C[n][n]
G[1] G[2] ... G[n]
```

- `-1` indica que no existe camino directo entre dos localizaciones.
- `G[k]` solo se paga cuando `k` es nodo intermedio (nunca como origen o destino).

---

## Generador de mapas

```bash
./generador <n> [salida.txt] [densidad] [semilla]
```

```bash
./generador 6 mapa.txt 0.4 42   # 6 nodos, densidad 0.4, semilla fija
```
