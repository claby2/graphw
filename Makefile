OSFLAG :=
ifeq ($(OS), Windows_NT)
	test-executable = test.exe
	benchmark-executable = benchmark.exe
else
	test-executable = test
	benchmark-executable = benchmark
endif

sdl-flags = -lSDL2main -lSDL2

.PHONY: test test-compile benchmark benchmark-compile build-examples

test: ./test/$(test-executable)
	./test/$(test-executable)

./test/$(test-executable): ./test/test_main.o
	g++ -std=c++17 ./test/test_main.o ./test/test_graphw.cpp $(sdl-flags) -o ./test/test

./test/test_main.o:
	g++ ./test/test_main.cpp -c -o ./test/test_main.o

test-compile:
ifeq (,$(wildcard ./test/test_main.o))
	$(info Compiling ./test/test_main.cpp, this only needs to be done once.)
	g++ ./test/test_main.cpp -c -g -o ./test/test_main.o
	g++ -std=c++17 ./test/test_main.o ./test/test_graphw.cpp $(sdl-flags) -g -o ./test/test
else
	g++ -std=c++17 ./test/test_main.o ./test/test_graphw.cpp $(sdl-flags) -g -o ./test/test
endif

benchmark: ./test/$(benchmark-executable)
	./test/$(benchmark-executable)

./test/$(benchmark-executable):
	g++ -std=c++17 ./test/benchmark.cpp $(sdl-flags) -o ./test/benchmark

benchmark-compile:
	g++ -std=c++17 ./test/benchmark.cpp $(sdl-flags) -o ./test/benchmark

benchmark-all: benchmark-compile benchmark
	

build-examples:
	g++ -std=c++17 ./examples/animate.cpp $(sdl-flags)               -o ./examples/animate
	g++ -std=c++17 ./examples/arc_diagram.cpp $(sdl-flags)           -o ./examples/arc_diagram
	g++ -std=c++17 ./examples/circular_layout.cpp $(sdl-flags)       -o ./examples/circular_layout
	g++ -std=c++17 ./examples/force_directed_layout.cpp $(sdl-flags) -o ./examples/force_directed_layout
	g++ -std=c++17 ./examples/random_layout.cpp $(sdl-flags)         -o ./examples/random_layout
	g++ -std=c++17 ./examples/spiral_layout.cpp $(sdl-flags)         -o ./examples/spiral_layout