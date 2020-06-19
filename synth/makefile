all: output_file_name

output_file_name: main.o
	gcc main.o -o output_file_name

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *o output_file_name