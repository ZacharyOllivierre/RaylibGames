SOURCES = main.cpp tile.cpp tileManager.cpp graphics.cpp helperFunctions.cpp entity.cpp entityManager.cpp structure.cpp house.cpp structureManager.cpp 
OBJECTS = $(SOURCES:.cpp=.o)
EXEC    = ex.exe

CXXFLAGS = -I/opt/homebrew/include -Wall -Wextra -std=c++17
LDFLAGS  = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

$(EXEC): $(OBJECTS)
	g++ $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

run: $(EXEC)
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJECTS)