#include <stdio.h>

void square_wave(int * data, int length, int period){

	int on = 1;
	int flip = (int)(period / 2);
	for(int i = 0; i < 128; ++i)
	{
		if(i % flip == 0)
		{
			on = (on == 0) ? 1 : 0;
		}
		data[i] = on;
	}
}

void multiply_vectors(int * in1, int * in2, int * out, int length){
	for(int i = 0; i < length; ++i)
	{
		out[i] = in1[i] * in2[i];
	}
}

int main() {
	printf("Hello World\n");

	int length = 128;

	int in [length];
	int in_period = 4;
	square_wave(in, length, in_period);

	int square [length];
	int square_period = 4;
	square_wave(square, length, square_period);

	int out [length];

	multiply_vectors(in, square, out, length);

	for(int i = 0; i < length; ++i)
	{
		printf("%d\n", out[i]);
	}

	return 0;
}

