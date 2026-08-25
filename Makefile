build:
	mkdir -p build && cd build && cmake -j .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_POLICY_VERSION_MINIMUM=3.5 && make && cd ..

.PHONY: rebuild clean build

rebuild: clean build

clean:
	rm -rf build/

run:
test:
