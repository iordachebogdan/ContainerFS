CC= gcc
CFLAGS= -Wall `pkg-config fuse3 --cflags` -g
LDFLAGS= -Wall `pkg-config fuse3 --libs` -lzip -g

all: build/ build/zipfs

build/:
	@mkdir -p build

build/zipfs: build/main.o build/fzip.o build/data.o build/utility.o build/readdir.o
	$(CC) -o build/zipfs build/main.o build/fzip.o build/data.o build/utility.o build/readdir.o $(LDFLAGS)

build/main.o: src/main.c src/utility.h src/data.h src/fzip.h
	$(CC) -c -o build/main.o src/main.c $(CFLAGS)

build/fzip.o: src/fzip.c src/data.h src/fzip.h
	$(CC) -c -o build/fzip.o src/fzip.c $(CFLAGS)

build/data.o: src/data.c src/data.h
	$(CC) -c -o build/data.o src/data.c $(CFLAGS)

build/utility.o: src/utility.c src/utility.h
	$(CC) -c -o build/utility.o src/utility.c $(CFLAGS)

build/readdir.o: src/readdir.c src/fzip.h
	$(CC) -c -o build/readdir.o src/readdir.c $(CFLAGS)

clean:
	@rm -f build/*.o
