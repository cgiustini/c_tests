/*
** Copyright (C) 2008-2016 Erik de Castro Lopo <erikd@mega-nerd.com>
**
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in
**       the documentation and/or other materials provided with the
**       distribution.
**     * Neither the author nor the names of any contributors may be used
**       to endorse or promote products derived from this software without
**       specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
** PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
** OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
** WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
** OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
** ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

#include <sndfile.h>

#define	BLOCK_SIZE 4096

#ifdef DBL_DECIMAL_DIG
	#define OP_DBL_Digs (DBL_DECIMAL_DIG)
#else
	#ifdef DECIMAL_DIG
		#define OP_DBL_Digs (DECIMAL_DIG)
	#else
		#define OP_DBL_Digs (DBL_DIG + 3)
	#endif
#endif

static void
print_usage (char *progname)
{	printf ("\nUsage : %s [--full-precision] <input file> <output file>\n", progname) ;
	puts ("\n"
		"    Where the output file will contain a line for each frame\n"
		"    and a column for each channel.\n"
		) ;

} /* print_usage */

static void
convert_to_text(SNDFILE * infile, FILE * outfile, int channels, int full_precision)
{	int buf [BLOCK_SIZE] ;
	sf_count_t frames ;
	int k, m, readcount ;

	frames = BLOCK_SIZE / channels ;

	while ((readcount = sf_readf_int (infile, buf, frames)) > 0)
	{	for (k = 0 ; k < readcount ; k++)
		{	for (m = 0 ; m < channels ; m++)
				if (full_precision)
					fprintf (outfile, " %d", buf [k * channels + m]) ;
				else
					fprintf (outfile, " % d", buf [k * channels + m]) ;
			fprintf (outfile, "\n") ;
			} ;
		} ;

	return ;
} /* convert_to_text */


int
write_to_buffer(SNDFILE * infile, int * buffer, int channels)
{	int buf [BLOCK_SIZE] ;
	sf_count_t frames ;
	int k, m, readcount, i;
	i = 0;

	frames = BLOCK_SIZE / channels ;

	while ((readcount = sf_read_int (infile, buf, frames)) > 0)
	{	
		// readcount should always be even if items arg (frames) is even. handle case if it is odd
		if ((readcount % 1) == 0)
		{
			readcount = readcount - 1;
		}
		for (k = 0 ; k < readcount ; k++)
		{
			buffer[i] = buf[k];
			i++;
		};
		// printf("%d\n", i);
		// i = i + readcount;
	} ;

	return i;
} /* convert_to_text */


int write_to_wav(char *filename, int * buffer, int sample_count, int samplerate)
{
	SNDFILE		*outfile = NULL ;
	SF_INFO		outsfinfo ;
	memset (&outsfinfo, 0, sizeof (outsfinfo)) ;
	outsfinfo.samplerate	= samplerate ;
	outsfinfo.frames		= sample_count;
	outsfinfo.channels		= 2;
	outsfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24);
	int samples_written = 0;

	if (! (outfile = sf_open (filename, SFM_WRITE, &outsfinfo)))
	{	printf ("Error : Not able to open output file.\n") ;
		return 1 ;
	} ;

	samples_written = sf_write_int (outfile, buffer, sample_count);
	sf_close(outfile);
	printf  ("%d %d", sample_count, samples_written);		
	return samples_written;
};


int
main (int argc, char * argv [])
{	char 		*infilename, *outfilename ;
	SNDFILE		*infile = NULL ;
	SF_INFO		insfinfo ;

	memset (&insfinfo, 0, sizeof (insfinfo));

	infilename = argv [1] ;
	outfilename = argv [2] ;


	// buffer is just huge and will be filledwith data.
	int	buffer [400000] ;
	int sample_count = 0;

	// write data from input wav file into buffer.
	if ((infile = sf_open (infilename, SFM_READ, &insfinfo)) == NULL)
	{	printf ("Not able to open input file %s.\n", infilename) ;
		puts (sf_strerror (NULL)) ;
		return 1 ;
		} ;
	sample_count = write_to_buffer (infile, buffer, insfinfo.channels);
	sf_close (infile) ;
	printf("Total samples read: %d Total number of frames in file: %d\n",  sample_count, (int) insfinfo.frames);
	printf("Total samples read should be twice number of frames (since a frame represents samples from all channels at one index in time and there are two channels\n");

	// Write data from buffer into wav
	write_to_wav(outfilename, buffer, sample_count, 44100);

	return 0 ;
} /* main */

