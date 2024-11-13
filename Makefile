CC = g++
CFLAGS = -I./include -g -Wall     # This tells the compiler to look in include
LDFLAGS = -lglfw -ldl

SRC = src/main.cpp src/glad.c 
OBJ = src/shader.o src/camera.o

TARGET = my_project

all: $(TARGET)


$(TARGET): $(SRC) $(OBJ)
	$(CC) $(CFLAGS) $(SRC) $(OBJ) $(LDFLAGS) -o $(TARGET)


src/shader.o: src/shader.cpp
	$(CC) $(CFLAGS) -c -o src/shader.o src/shader.cpp 

src/camera.o: src/camera.cpp
	$(CC) $(CFLAGS) -c -o src/camera.o src/camera.cpp 



clean:
	@echo "Cleaning things..."
	@rm -f $(TARGET) src/*.o
