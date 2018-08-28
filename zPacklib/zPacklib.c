#include "zPacklib.h"
#include "lib_qlz\quicklz.h"

FRESULT ZPack_open(ZPACK_FIL * fp, const CHAR * path, BYTE mode)
{
	return FR_OK;
}

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


UINT zPack_Compress_File(IN_FILEINFO *in, OUT_FILEINFO *out, UINT *n)
{
	static BYTE *pBUF;
	if (in->pFil == 0)
	{
		if(ZPack_OpenFile(in->pFil, in->FilePath, FA_OPEN_EXISTING| FA_READ)!=0)
			return ZPACK_FILE_NOT_OPEN;
	}
	if (out->pFil == 0)
	{
		if (ZPack_OpenFile(out->pFil, out->FilePath, FA_CREATE_ALWAYS| FA_WRITE) != 0)
			return ZPACK_FILE_NOT_OPEN;
		pBUF = malloc(OUT_BEFFER_LEN);
		ZPack_lseek(out->pFil, sizeof(ZPACKFILEHEADER));
	}
	
	size_t r;
	ui32 compressed;
	size_t base;
	ZPACKINFOHEADER ih = { 0 };
	ih.frUncompressedSize = ZPack_GetFileSize(in->pFil);

	const unsigned char *last_byte = source + size - 1;
	const unsigned char *src = source;
	unsigned char *cword_ptr = destination;
	unsigned char *dst = destination + 4;
	ui32 cword_val = 1U << 31;
	const unsigned char *last_matchstart = last_byte - 6 - 4;
	ui32 fetch = 0;
	unsigned int lits = 0;

	(void)lits;

	if (src <= last_matchstart)
		fetch = fast_read(src, 3);

	while (src <= last_matchstart)
	{
		if ((cword_val & 1) == 1)
		{
			// store uncompressed if compression ratio is too low
			if (src > source + (size >> 1) && dst - destination > src - source - ((src - source) >> 5))
				return 0;

			fast_write((cword_val >> 1) | (1U << 31), cword_ptr, 4);

			cword_ptr = dst;
			dst += 4;
			cword_val = 1U << 31;
			fetch = fast_read(src, 3);
		}
	}
	while (src <= last_byte)
	{
		if ((cword_val & 1) == 1)
		{
			fast_write((cword_val >> 1) | (1U << 31), cword_ptr, 4);
			cword_ptr = dst;
			dst += 4;
			cword_val = 1U << 31;
		}
		*dst = *src;
		src++;
		dst++;
		cword_val = (cword_val >> 1);
	}

	while ((cword_val & 1) != 1)
		cword_val = (cword_val >> 1);

	fast_write((cword_val >> 1) | (1U << 31), cword_ptr, 4);

	// min. size must be 9 bytes so that the qlz_size functions can take 9 bytes as argument
	return dst - destination < 9 ? 9 : dst - destination;


}
UINT zPack_Compress_End(OUT_FILEINFO *out)
{
	return 0;
}


