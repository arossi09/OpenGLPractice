CC = g++
CFLAGS = -I./include -I./$(IMGUI_DIR) -I./$(IMGUI_DIR)/backends -std=c++11 -g -Wall
LDFLAGS = -lglfw -ldl -lGL
IMGUI_DIR = src/thirdparty/imgui

SOURCES = src/main.cpp src/camera.cpp src/box.cpp src/shader.cpp src/glad.c
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp \
            $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

OBJ = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

TARGET = my_project

# Default target
all: $(TARGET)

# Linking the final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(TARGET)

# Compile .c files
%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile .cpp files in the src directory
%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile ImGui .cpp files
%.o: $(IMGUI_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile ImGui backend .cpp files
%.o: $(IMGUI_DIR)/backends/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean the build
clean:
	@echo "Cleaning up..."
	rm -f $(OBJ) $(TARGET)

