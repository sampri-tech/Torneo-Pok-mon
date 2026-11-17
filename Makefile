CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -std=c99 -I Libraries

SRCS = main.c \
       Libraries/Entrenador.c \
       Libraries/Pokedex.c \
       Libraries/Pokemon.c \
       Libraries/LIST.c \
       Libraries/Combate.c \
       Libraries/Torneo.c \
       Libraries/Menu.c

TARGET = torneo.exe

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(TARGET) 2>nul
	del /Q main.o 2>nul
	del /Q Libraries\Entrenador.o 2>nul
	del /Q Libraries\Pokedex.o 2>nul
	del /Q Libraries\Pokemon.o 2>nul
	del /Q Libraries\LIST.o 2>nul
	del /Q Libraries\Combate.o 2>nul
	del /Q Libraries\Torneo.o 2>nul
	del /Q Libraries\Menu.o 2>nul
	del /Q resultados.txt 2>nul
	del /Q clasificacion.txt 2>nul
