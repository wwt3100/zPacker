#pragma once
#include <Windows.h>

typedef struct tagZPACKFILEHEADER
{
	UINT16 zfType;
	DWORD zfSize;		//�ļ���С
	DWORD zfCRC32;		//�ļ�CRCУ��
	UINT16 zfFileNumber;	//�����ļ���
	UINT16 zfFileFlag;		//�ļ���Ϣ bit  | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	                        //		        �ļ�������|�ļ�����                                           |ѹ���ȼ�(1/2/3)
	DWORD zfOffBits;
} ZPACKFILEHEADER;

typedef struct tagZPACKFILERECORD{
	DWORD		signature;
	DWORD		frCompressedSize;  //  ѹ����Ĵ�С
	DWORD		frUncompressedSize;  // δѹ���Ĵ�С
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
