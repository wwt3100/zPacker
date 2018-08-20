
#include "stdafx.h"
#include <Windows.h>
#include "zPacklib.h"
#include "lib_qlz\quicklz.h"

size_t zPack_compress(const void *source, char *destination, size_t size)
{
	qlz_state_compress sd = { 0 };
	return qlz_compress(source, destination, size, &sd);
}

size_t zPack_Decompress(const char *source, void *destination)
{
	qlz_state_decompress sd = { 0 };
	return qlz_decompress(source, destination, &sd);
}

size_t zPcak_GetSize_decompressed(const void *source)
{
	return qlz_size_decompressed(source);
}

size_t zPack_GetSize_compressed(const char *source)
{
	return qlz_size_compressed(source);
}