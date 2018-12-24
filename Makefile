CC= gcc
CFLAGS= -Wall `pkg-config fuse3 --cflags` -g
LDFLAGS= -Wall `pkg-config fuse3 --libs` -lzip -g

all: build/ build/zipfs

build/:
	@mkdir -p build

build/zipfs: build/main.o
	$(CC) -o build/zipfs build/main.o $(LDFLAGS)

build/main.o: src/main.c
	$(CC) -c -o build/main.o src/main.c $(CFLAGS)

clean:
	@rm -f build/*.o

