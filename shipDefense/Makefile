SOURCES = \
	src/main.cpp \
	src/ships/ship.cpp \
	src/ships/playerShip.cpp \
	src/ships/enemyShip.cpp \
	src/graphics.cpp \
	src/world.cpp \
	src/game.cpp

OBJECTS = $(SOURCES:.cpp=.o)
EXEC    = ex

CXXFLAGS = -I/opt/homebrew/include -Iinclude -Iinclude/imgui -Iinclude/rlImGui -Wall -Wextra -std=c++17
LDFLAGS  = -L/opt/homebrew/lib -lraylib -lvulkan -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

$(EXEC): $(OBJECTS)
	g++ $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)
	