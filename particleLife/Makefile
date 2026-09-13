SOURCES = \
	src/main.cpp \
	src/particle.cpp \
	src/simulation.cpp \
	src/controls.cpp \
	src/graphics.cpp \
	src/VulkanCompute.cpp \
	src/imguiWindow.cpp \
    include/imgui/imgui.cpp \
    include/imgui/imgui_draw.cpp \
    include/imgui/imgui_tables.cpp \
    include/imgui/imgui_widgets.cpp \
    include/rlImGui/rlImGui.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXEC    = ex

CXXFLAGS = -I/opt/homebrew/include -Iinclude -Iinclude/imgui -Iinclude/rlImGui -Wall -Wextra -std=c++17
LDFLAGS  = -L/opt/homebrew/lib -lraylib -lvulkan -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SHADER = shaders/particle.comp.spv

$(EXEC): $(OBJECTS) $(SHADER)
	g++ $(OBJECTS) -o $@ $(LDFLAGS)

$(SHADER): shaders/particle.comp
	glslc --target-env=vulkan1.2 $< -o $@

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS) $(SHADER)
	