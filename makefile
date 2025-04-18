CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -pedantic -O0
all: 
	$(CC) $(CFLAGS) -c -o segmentation_guard.o src/segmentation_guard.c 
	mkdir -p build
	mv segmentation_guard.o build/segmentation_guard.o
	cp lib/* build/
	cd build;ar -x libxed.a; ar rcs segmentation_guard.a *.o
	mkdir -p out
	mkdir -p out/include
	mkdir -p out/lib
	cp build/segmentation_guard.a out/lib/
	cp src/segmentation_guard.h out/include/

examples: all
	# Copy the example files to the out directory
	cp examples/* out/
	#individually copmile the examples
	cd out/; for f in *.c; do \
		$(CC) $(CFLAGS) -o `basename $$f .c` $$f -Llib lib/*.a; \
	done

clean:
	rm -fr build out
