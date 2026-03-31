CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
SOURCES = main.cpp game.cpp ball.cpp paddle.cpp graphics.cpp sounds.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXEC = pong.exe

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)