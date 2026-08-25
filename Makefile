build:
	mkdir -p build && cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && make && cd ..

.PHONY: rebuild clean

rebuild: clean build

clean:
	rm -rf build/

run:
test:
