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

rc:
	mkdir -p client && cd client && ./client && cd ..

rs:
	mkdir -p server && cd server && ./server && cd ..

mb:
	mkdir -p client
	mkdir -p server
	rm -f ./client/client
	rm -f ./server/server
	mv ./build/client ./client/client 2>/dev/null || true
	mv ./server/server ./server/server 2>/dev/null || true