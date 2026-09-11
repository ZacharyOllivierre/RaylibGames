SOURCES = \
	src/main.cpp \
	src/particle.cpp \
	src/simulation.cpp \
	src/controls.cpp \
	src/graphics.cpp \
    include/imgui/imgui.cpp \
    include/imgui/imgui_draw.cpp \
    include/imgui/imgui_tables.cpp \
    include/imgui/imgui_widgets.cpp \
    include/rlImGui/rlImGui.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXEC    = ex

CXXFLAGS = -I/opt/homebrew/include -Iinclude/imgui -Iinclude/rlImGui -Wall -Wextra -std=c++17
LDFLAGS  = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

$(EXEC): $(OBJECTS)
	g++ $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)