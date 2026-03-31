SOURCES = main.cpp ball.cpp ballManager.cpp graphics.cpp basicFunctions.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC    = b.exe

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