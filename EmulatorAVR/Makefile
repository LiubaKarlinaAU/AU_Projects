.PHONY: all clean

CXX=g++
CXXFLAGS=-std=c++11 -Wall -g -pedantic

all: emulator

bin:
	mkdir -p bin

emulator: src/main.cpp  bin/emulator.o bin/microcontroller.o bin/function_defenition.o bin/state_register.o bin/function_execution.o 
	$(CXX) -g $(CXXFLAGS) -o $@ -Iinclude $< bin/*

bin/%.o: src/%.cpp include/*.h bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf bin emulator

run:
	./emulator
