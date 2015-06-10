CX=g++
LD=g++
CXFLAGS=-Wall -g -Iinc -std=gnu++11
LDFLAGS=
PRODUCT=bin/gpse

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(patsubst src/%.cpp,build/%.o,$(SOURCES))

all: dirs $(OBJECTS)
	$(LD) -o $(PRODUCT) $(OBJECTS) $(LDFLAGS)

.PHONY: dirs
dirs:
	mkdir -p build
	mkdir -p bin

build/%.o: src/%.cpp
	$(CX) $(CXFLAGS) -c $< -o $@

clean:
	rm -rf build

clean_all: clean
	rm -rf bin