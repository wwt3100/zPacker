#pragma once
#include <Windows.h>

typedef struct tagZPACKFILEHEADER
{
	UINT16 zfType;
	DWORD zfSize;		//文件大小
	DWORD zfCRC32;		//文件CRC校验
	UINT16 zfFileNumber;	//含有文件数
	UINT16 zfFileFlag;		//文件信息 bit  | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	                        //		        文件名加密|文件加密                                           |压缩等级(1/2/3)
	DWORD zfOffBits;
} ZPACKFILEHEADER;

typedef struct tagZPACKFILERECORD{
	DWORD		signature;
	DWORD		frCompressedSize;  //  压缩后的大小
	DWORD		frUncompressedSize;  // 未压缩的大小
	DWORD		frCompressedCRC;
	DWORD		frUncompressedCRC;
	WORD		frFileNameLength;
	CHAR        *FileName;
	BYTE		*FileData;
} ZPACKINFOHEADER;

enum zPackStateFlags
{
	ZPACK_OK,
	ZPACK_CRC_ERROR,

};

#if defined (__cplusplus)
extern "C" {
#endif

	int zPack_Open(void);
	
	int zPack_CompressFile(void);
	int zPack_CompressFolder(void);


	size_t zPack_compress(const void *source, char *destination, size_t size);
	size_t zPack_Decompress(const char *source, void *destination);
	size_t zPcak_GetSize_decompressed(const void *source);
	size_t zPack_GetSize_compressed(const char *source);

#if defined (__cplusplus)
}
#endif
