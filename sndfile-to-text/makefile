all: output_file_name

output_file_name: sndfile-to-text.o
	gcc sndfile-to-text.o -lsndfile -o output_file_name

sndfile-to-text.o: sndfile-to-text.c
	gcc -c sndfile-to-text.c

clean:
	rm -rf *o output_file_name