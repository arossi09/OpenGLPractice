CC = g++
CFLAGS = -I./include      # This tells the compiler to look in include/
LDFLAGS = -lglfw -ldl

SRC = src/main.cpp src/glad.c

all:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o my_project
