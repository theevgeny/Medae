all: build

build2:
	mkdir -p build && cd build && cmake .. && make -j 2 && cd ..

build:
	mkdir -p build && cd build && cmake .. && make -j 8 && cd ..

.PHONY: rebuild clean build

rebuild: clean build

clean:
	rm -rf build/

run:
test:
