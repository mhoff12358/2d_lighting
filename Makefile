
SDL_INCLUDE = -IC:/MinGW/include
SDL_LIB = -LC:/MinGW/lib -lSDL2main -lSDL2 -lSDL2_image
# GL_INCLUDE = 
GL_LIB = -lglew32 -lopengl32 -lglu32

COMPILER = g++
COMPILER_FLAGS = -Wall -c -g -std=c++11 $(SDL_INCLUDE)
LD_FLAGS = -lmingw32 -mwindows -mconsole -std=c++11 $(GL_LIB) $(SDL_LIB)

EXE = main.exe

all: $(EXE)

$(EXE): main.o view_state.o view_drawer.o io_controller.o game_logic.o shader.o my_math.o texture_handler.o actor.o
	$(COMPILER) main.o view_state.o view_drawer.o io_controller.o game_logic.o shader.o my_math.o texture_handler.o actor.o $(LD_FLAGS) -o $@

main.o: main.cpp main.h
	$(COMPILER) $(COMPILER_FLAGS) main.cpp -o $@

view_state.o: view_state.cpp view_state.h
	$(COMPILER) $(COMPILER_FLAGS) view_state.cpp -o $@

texture_handler.o: texture_handler.cpp texture_handler.h
	$(COMPILER) $(COMPILER_FLAGS) texture_handler.cpp -o $@

shader.o: shader.cpp shader.h
	$(COMPILER) $(COMPILER_FLAGS) shader.cpp -o $@

actor.o: actor.cpp actor.h
	$(COMPILER) $(COMPILER_FLAGS) actor.cpp -o $@

view_drawer.o: view_drawer.cpp view_drawer.h
	$(COMPILER) $(COMPILER_FLAGS) view_drawer.cpp -o $@

io_controller.o: io_controller.cpp io_controller.h
	$(COMPILER) $(COMPILER_FLAGS) io_controller.cpp -o $@

game_logic.o: game_logic.cpp game_logic.h
	$(COMPILER) $(COMPILER_FLAGS) game_logic.cpp -o $@

my_math.o: my_math.cpp my_math.h
	$(COMPILER) $(COMPILER_FLAGS) my_math.cpp -o $@

clean:
	rm *.o