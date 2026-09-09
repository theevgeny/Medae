all: build

build:
	mkdir -p build && cd build && cmake .. && make && cd ..

buildmt:
	mkdir -p build && cd build && cmake .. && make -j ${THREADS} && cd ..

.PHONY: rebuild clean build

rebuild: clean build

clean:
	rm -rf build/

run:
test:
