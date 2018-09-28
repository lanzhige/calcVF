#ifndef  COMPRESS_H_
#define COMPRESS_H_

#include<fstream>
#include<iostream>
#include<string>
#include<assert.h>
#include "zlib-1.2.11/zlib.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384

using std::string;
using std::ifstream;
using namespace std;

/* report a zlib or i/o error */
void zerr(int ret)
{
	fputs("zpipe: ", stderr);
	switch (ret) {
	case Z_ERRNO:
		if (ferror(stdin))
			fputs("error reading stdin\n", stderr);
		if (ferror(stdout))
			fputs("error writing stdout\n", stderr);
		break;
	case Z_STREAM_ERROR:
		fputs("invalid compression level\n", stderr);
		break;
	case Z_DATA_ERROR:
		fputs("invalid or incomplete deflate data\n", stderr);
		break;
	case Z_MEM_ERROR:
		fputs("out of memory\n", stderr);
		break;
	case Z_VERSION_ERROR:
		fputs("zlib version mismatch!\n", stderr);
	}
}

int compress(unsigned char *source, const string &outpath, int size, int level = Z_DEFAULT_COMPRESSION)
{
	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char *out = new unsigned char[size];
	std::FILE *dest;
	fopen_s(&dest, &outpath[0], "wb");

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, level);
	if (ret != Z_OK)
		return ret;

	strm.avail_in = size;
	flush = Z_FINISH;
	strm.next_in = source;

	do {
		strm.avail_out = CHUNK;
		strm.next_out = out;
		ret = deflate(&strm, flush);    /* no bad return value */
		assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
		have = CHUNK - strm.avail_out;
		if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
			(void)deflateEnd(&strm);
			return Z_ERRNO;
		}
	} while (strm.avail_out == 0);
	delete[] out;
	fclose(dest);
	(void)deflateEnd(&strm);
	return Z_OK;
}

int decompress(const string& inpath, unsigned char *out, int size) {
	int ret;
	unsigned have;
	z_stream strm;
    unsigned char *in;
	std::FILE *source;
	fopen_s(&source, &inpath[0], "rb");
	if (source == nullptr) {
		std::cerr << "error in reading compressed binary: " << inpath << std::endl;
		return -1;
	}

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;
	fseek(source, 0, SEEK_END);
	long lSize = ftell(source);
	rewind(source);

	// allocate memory to contain the whole file:
	in = (unsigned char*)malloc(sizeof(unsigned char)*lSize);
	if (in == NULL) { 
		std::cerr << "can't alloc memory for data!" << std::endl;
		return -2;
	}

	/* decompress until deflate stream ends or end of file */
	do {
		if ((strm.avail_in = fread(in, 1, lSize, source)) != lSize) {
			std::cout << "lSize: " << lSize << std::endl;
			std::cout << "strm available in: " << strm.avail_in << std::endl;
			std::cout << "dir: " << inpath << std::endl;
			//std::cerr << "reading error" << std::endl;
			//return -3;
		}
		if (ferror(source)) {
			(void)inflateEnd(&strm);
			return Z_ERRNO;
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = size;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			have = size - strm.avail_out;
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	delete[] in;
	fclose(source);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

/* Decompress from file source to file dest until stream ends or EOF.
inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
allocated for processing, Z_DATA_ERROR if the deflate data is
invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
the version of the library linked do not match, or Z_ERRNO if there
is an error reading or writing the files. */
int inf(FILE *source, FILE *dest) {
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char *in = new unsigned char[CHUNK];
	unsigned char *out = new unsigned char[CHUNK];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit(&strm);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = fread(in, 1, CHUNK, source);
		if (ferror(source)) {
			(void)inflateEnd(&strm);
			return Z_ERRNO;
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
				(void)inflateEnd(&strm);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


#endif // ! COMPRESS_H_