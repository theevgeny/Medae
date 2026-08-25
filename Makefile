build:
	mkdir -p build && cd build && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DALSOFT_EXAMPLES=OFF -DALSOFT_UTILS=OFF && make -j 8 && cd ..

.PHONY: rebuild clean build

rebuild: clean build

clean:
	rm -rf build/

run:
test:
