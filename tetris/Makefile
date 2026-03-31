SOURCES = main.cpp graphics.cpp board.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC    = t.exe

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