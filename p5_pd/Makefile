# Makefile para la practica de Programacion Dinamica
#
# Targets principales:
#   make              -> compila aventura y generador
#   make aventura     -> compila solo el solver
#   make generador    -> compila solo el generador de mapas
#
# Targets de ejecucion:
#   make run          -> ejecuta aventura con ejemplo 1
#   make svg          -> genera mapa1.svg con el ejemplo 1
#   make ruta         -> genera mapa1.svg destacando la ruta 1 -> 3
#   make aleatorio    -> genera un mapa aleatorio de 6 nodos y lo resuelve
#   make ejemplos     -> ejecuta los 3 ejemplos generando SVG con ruta destacada
#
#   make clean        -> elimina archivos generados

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Solver
AVENTURA      = aventura
SRCS_AVENTURA = main.cpp camino_heroe.cpp visualizacion.cpp
OBJS_AVENTURA = $(SRCS_AVENTURA:.cpp=.o)
HEADERS       = camino_heroe.h visualizacion.h

# Generador
GENERADOR     = generador
SRCS_GEN      = generador.cpp

all: $(AVENTURA) $(GENERADOR)

$(AVENTURA): $(OBJS_AVENTURA)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(GENERADOR): $(SRCS_GEN)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(AVENTURA)
	./$(AVENTURA) ejemplo1.txt

svg: $(AVENTURA)
	./$(AVENTURA) ejemplo1.txt salida1.txt mapa1.svg
	@echo "Mapa generado: mapa1.svg (abrir con un navegador)"

ruta: $(AVENTURA)
	./$(AVENTURA) ejemplo1.txt salida1.txt mapa1.svg 1 3
	@echo "Mapa con ruta destacada generado: mapa1.svg"

ejemplos: $(AVENTURA)
	./$(AVENTURA) ejemplo1.txt ejemplo1_salida.txt ejemplo1_mapa.svg 1 3
	./$(AVENTURA) ejemplo2.txt ejemplo2_salida.txt ejemplo2_mapa.svg 1 5
	./$(AVENTURA) ejemplo3.txt ejemplo3_salida.txt ejemplo3_mapa.svg 1 7
	@echo "SVGs generados: ejemplo1_mapa.svg  ejemplo2_mapa.svg  ejemplo3_mapa.svg"

aleatorio: $(AVENTURA) $(GENERADOR)
	./$(GENERADOR) 6 aleatorio.txt 0.4
	./$(AVENTURA) aleatorio.txt salida_aleatorio.txt mapa_aleatorio.svg
	@echo "Mapa aleatorio generado: mapa_aleatorio.svg"

clean:
	rm -f $(OBJS_AVENTURA) $(AVENTURA) $(GENERADOR) \
	      salida*.txt aleatorio.txt mapa*.svg

.PHONY: all run svg ruta ejemplos aleatorio clean
