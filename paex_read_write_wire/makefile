LD_LIBRARY_PATH=/usr/local/lib
export LD_LIBRARY_PATH

all: output_file_name

output_file_name: main.o
	gcc main.o -lportaudio -o output_file_name -lm

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *o output_file_name