CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
SOURCES = main.cpp game.cpp graphics.cpp controls.cpp neighborhood.cpp button.cpp rules.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = ex.exe


all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)
	