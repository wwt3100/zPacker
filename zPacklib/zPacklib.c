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

size_t zPack_GetSize_decompressed(const void *source)
{
	return qlz_size_decompressed(source);
}

size_t zPack_GetSize_compressed(const char *source)
{
	return qlz_size_compressed(source);
}

static ZPACKFILEHEADER zfhead = { 0 };
//static FATFS fs = { 0 };
//UINT zPack_Init(void)
//{
//	FILE *fpImg;
//	f_mount(0, "0:", 1);
//	errno_t err = fopen_s(&fpImg, "Sim.img", "rb+"); //open file for read/write  
//	switch(err)
//	{
//	case 0:
//		break;
//	case 2:
//		fopen_s(&fpImg, "Sim.img", "wb");
//		fseek(fpImg, 128 * 1024 * 1024 + 1, 0);
//		fputc(EOF, fpImg);
//		break;
//	default:
//		return err;
//	}
//	fclose(fpImg);
//	UINT res = f_mount(&fs, "0:", 1);
//	BYTE work[FF_MAX_SS];
//	if (res == FR_NO_FILESYSTEM)
//	{
//		res = f_mkfs("0:", FM_FAT32, 0, work, sizeof(work));
//		res += f_mount(&fs, "0:", 1);
//	}
//		
//	return res;
//}
UINT zPack_Compress_File(IN_FILEINFO *in, OUT_FILEINFO *out, UINT *n)
{
	if (out->pFil == 0)
	{
		if (zf_open(&(out->pFil), out->FilePath, FA_CREATE_ALWAYS| FA_WRITE |FA_READ) != FR_OK)
		{
			return -1;
		}
		if (out->Key != 0)
		{
			sm3(out->Key, strnlen_s(out->Key, 512), out->KeyHash);
		}
		//memset(&zfhead, 0, sizeof(ZPACKFILEHEADER));
		//zfhead.zfType = 0x5A50414B;	ZPAK
		//zfhead.zfType = 0x4B41505A;
		//zfhead.zfBlockSize = FILE_BLOCK / 1024;
		zf_lseek(out->pFil, sizeof(ZPACKFILEHEADER));
	}
	if (in->pFil != 0)
	{
		ZPACKINFOHEADER zihead = { 0 };
		out->zfFileNumber += 1;
		UINT filenamelen = strlen(in->FileName);
		zihead.Signage = 0x214C465A;
		zihead.frFileNameLength = filenamelen;
		filenamelen = (16 * ((filenamelen + 15) / 16));
		UINT thisfile = zf_tell(out->pFil);
		zf_lseek(out->pFil, thisfile + sizeof(ZPACKINFOHEADER) + filenamelen);
		UINT thisfile2 = zf_tell(out->pFil);		//for test
		size_t buffsize = zf_size(in->pFil);
		size_t blocksize = buffsize > FILE_BLOCK ? FILE_BLOCK : buffsize;
		BYTE *inbuf = zf_malloc(blocksize);
		BYTE *outbuf = zf_malloc(blocksize + 400);
		UINT uncomp_crc = -1;
		UINT outsize = 0;
		for (UINT i = 0;; i++)		//文件分块
		{
			UINT br,bw;
			UINT c = 0;
			qlz_state_compress sd = { 0 };
			zf_read(in->pFil, inbuf, FILE_BLOCK, &br);
			uncomp_crc = GetCRC32Ex(uncomp_crc, inbuf, br);
			memset(outbuf, 0, br + 400);
			outsize = qlz_compress(inbuf, outbuf + 1, br, &sd) + 1;
			if (br < FILE_BLOCK || zf_eof(in->pFil) != 0)	//last block
			{
				*outbuf = 0xFF;
			}
			else
			{
				*outbuf = 0xCC;
			}
			if (out->Key != 0)
			{
				for (c = 0; c < (br + 15) / 16; c++)	//如果有余数多加密一次
				{
					sm4_context ctx;
					sm4_setkey_enc(&ctx, (out->KeyHash + ((c % 2) ? 16 : 0)));
					sm4_crypt_ecb(&ctx, 1, 16, outbuf + 16 * c, outbuf + 16 * c);
				}
			}
			zf_write(out->pFil, outbuf, (c == 0) ? outsize : (c * 16), &bw);		//根据实际长度选择写入
			if (br < FILE_BLOCK || zf_eof(in->pFil) != 0)	//last block exit
			{
				break;
			}
		}
		zihead.frUncompressedCRC = uncomp_crc;
		zihead.NextFileOffset = zf_tell(out->pFil);
		zihead.FileType = in->Type;
		UINT c = 0;
		if (out->Key != 0)
		{
			for (; c < (filenamelen + 15) / 16; c++)	//加密文件名
			{
				sm4_context ctx;
				sm4_setkey_enc(&ctx, (out->KeyHash + ((c % 2) ? 0 : 16)));
				sm4_crypt_ecb(&ctx, 1, 16, in->FileName + 16 * c, in->FileName + 16 * c);
			}
		}
		zf_lseek(out->pFil, thisfile);
		UINT wb;
		zf_write(out->pFil, &zihead, sizeof(ZPACKINFOHEADER), &wb);
		zf_write(out->pFil, in->FileName, filenamelen, &wb);
		//zf_write(out->pFil, in->FileName, c == 0 ? zihead.frFileNameLength : c * 16, &wb);
		zf_lseek(out->pFil, EOF);	//move pointer to the end of file
		//zf_sync(out->pFil);
		//if(0) //test block
		//{	
		//	zf_close(out->pFil);
		//	exit();
		//}
		zf_free(inbuf);
		zf_free(outbuf);
	}
	return 0;
}
UINT zPack_Compress_End(OUT_FILEINFO *out)
{
	if (out->pFil != 0)
	{
		UINT wb;
		ZPACKFILEHEADER zfhead = { 0 };
		BYTE *file;
		zfhead.zfType = 0x214B505A;
		zfhead.zfFileNumber = out->zfFileNumber;
		zfhead.zfBlockSize = FILE_BLOCK / 1024;
		zf_lseek(out->pFil, 0);
		zf_write(out->pFil, &zfhead, sizeof(ZPACKFILEHEADER), &wb);
		zf_lseek(out->pFil, 0);
		file = zf_malloc(zf_size(out->pFil));	//对于PC可以直接读取全部文件
		zf_read(out->pFil, file, zf_size(out->pFil), &wb);
		zfhead.zfCRC32 = GetCRC32(file + 8, zf_size(out->pFil) - 8);
		zf_lseek(out->pFil, 4);
		zf_write(out->pFil, &zfhead.zfCRC32, 4, &wb);
		zf_close(out->pFil);
		zf_free(file);
		return 0;
	}
	else
	{
		return 1;
	}
}

