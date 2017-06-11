.PHONY: all clean

CXX=g++
CXXFLAGS=-std=c++11 -Wall -g -pedantic

all: emulator

bin:
	mkdir -p bin

emulator: src/main.cpp  bin/emulator.o bin/microcontroller.o bin/function_defenition.o bin/state_register.o bin/function_execution.o 
	$(CXX) -g $(CXXFLAGS) -o $@ -Iinclude $< bin/*

emulatortest: test/test.cpp  bin/emulator.o bin/microcontroller.o bin/function_defenition.o bin/state_register.o bin/function_execution.o bin/autotest.o bin/emulator_test.o
	$(CXX) -g $(CXXFLAGS) -o $@ -Iinclude $< bin/*

bin/autotest.o: test/autotest.cpp include/*.h bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

bin/emulator_test.o: test/emulator_test.cpp include/*.h bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

bin/%.o: src/%.cpp include/*.h bin
	$(CXX) $(CXXFLAGS) -c -o $@ -Iinclude $<

clean:
	rm -rf bin emulator emulatortest

run:
	./emulator
