#include "zPacklib.h"
#include "lib_qlz\quicklz.h"

#if (defined(__X86__) || defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(__386__) || defined(__x86_64__) || defined(_M_X64))
#define X86X64
#endif

#define MINOFFSET 2
#define UNCONDITIONAL_MATCHLEN 6
#define UNCOMPRESSED_END 4
#define CWORD_LEN 4

#define FILE_BLOCK 10*1024

FRESULT zf_open(ZPACK_FIL * fp, const CHAR * path, BYTE mode)
{
	return FR_OK;
}
static void *zf_malloc(size_t size)
{
	return malloc(size);
}
static void zf_free(void *pointer)
{
	free(pointer);
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

static ZPACKFILEHEADER zfhead = { 0 };
UINT zPack_Compress_File(IN_FILEINFO *in, OUT_FILEINFO *out, UINT *n)
{
	if (out->pFil == 0)
	{
		if (zf_open(out->pFil, out->FilePath, FA_CREATE_ALWAYS| FA_WRITE) != FR_OK)
		{
			return -1;
		}
		if (out->Key != 0)
		{
			sm3(out->Key, strnlen_s(out->Key, 512), out->KeyHash);
		}
		memset(&zfhead, 0, sizeof(ZPACKFILEHEADER));
		//zfhead.zfType = 0x5A50414B;
		zfhead.zfType = 0x4B41505A;
		zfhead.zfBlockSize = FILE_BLOCK / 1024;
		zf_lseek(out->pFil, sizeof(ZPACKFILEHEADER));
	}
	if (in->pFil != 0)
	{
		ZPACKINFOHEADER zihead = { 0 };
		zfhead.zfFileNumber += 1;
		UINT filenamelen = strlen(in->FileName);
		zihead.frFileNameLength = filenamelen;
		filenamelen = (16 * (filenamelen + 15) / 16);
		UINT nextfile = zf_tell(in->pFil) + 4;
		zf_lseek(out->pFil, sizeof(ZPACKINFOHEADER)+ filenamelen);
		size_t buffsize = zf_size(in->pFil);
		size_t blocksize = buffsize > FILE_BLOCK ? FILE_BLOCK : buffsize;
		BYTE *inbuf = zf_malloc(blocksize);
		BYTE *outbuf = zf_malloc(blocksize + 10);
		UINT uncomp_crc = -1;
		UINT outsize = 0;
		for (UINT i = 0; i < (buffsize / FILE_BLOCK + (buffsize % FILE_BLOCK == 0) ? 0 : 1); i++)
		{
			if (i < buffsize / FILE_BLOCK || buffsize % FILE_BLOCK == 0)
			{
				blocksize = FILE_BLOCK;
			}
			else
			{
				blocksize = buffsize % FILE_BLOCK;
			}
			BYTE fh;
			UINT br;
			qlz_state_compress sd = { 0 };
			zf_read(in->pFil, inbuf, blocksize, &br);
			uncomp_crc = GetCRC32Ex(uncomp_crc, inbuf, blocksize);
			memset(outbuf, 0, blocksize + 10);
			outsize = outsize + qlz_compress(inbuf, outbuf + 1, blocksize, &sd) + 1;
			if (i == (buffsize / FILE_BLOCK + (buffsize % FILE_BLOCK == 0) ? 0 : 1) - 1)	//last block
			{
				*outbuf = 0xFF;
			}
			else
			{
				*outbuf = 0xCC;
			}
			if (out->Key != 0)
			{
				for (UINT c = 0; c < c / 16; c++)
				{
					sm4_context ctx;
					sm4_setkey_enc(&ctx, out->KeyHash + c % 2 ? 0 : 16);
					sm4_crypt_ecb(&ctx, 1, 16, outbuf + 16 * c, outbuf + 16 * c);
				}
			}
		}
	}
}
UINT zPack_Compress_End(OUT_FILEINFO *out)
{

}

