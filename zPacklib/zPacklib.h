#pragma once
#include <stdio.h>
#include <malloc.h>
#include"lib_sm/sm3.h"
#include"lib_sm/sm4.h"

#define IN_BEFFER_LEN	128
#define OUT_BEFFER_LEN	128		//缓冲区长度 16的整数倍	建议不小于512

#define VOID void
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef enum {
	FR_OK = 0,				/* (0) Succeeded */
	FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */
	FR_INT_ERR,				/* (2) Assertion failed */
	FR_NOT_READY,			/* (3) The physical drive cannot work */
	FR_NO_FILE,				/* (4) Could not find the file */
	FR_NO_PATH,				/* (5) Could not find the path */
	FR_INVALID_NAME,		/* (6) The path name format is invalid */
	FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	FR_EXIST,				/* (8) Access denied due to prohibited access */
	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	FR_NOT_ENABLED,			/* (12) The volume has no work area */
	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any problem */
	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > FF_FS_LOCK */
	FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */
} FRESULT;

#define	FA_READ				0x01
#define	FA_WRITE			0x02
#define	FA_OPEN_EXISTING	0x00
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define	FA_OPEN_APPEND		0x30

#define ZPACK_FIL FILE

typedef struct tagZPACKFILEHEADER
{
	DWORD zfType;	//0x5A50414B
	DWORD zfCRC32;		//文件CRC校验
	WORD zfFileNumber;	//含有文件数
	WORD zfFileFlag;		//文件信息 bit  | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	                        //		        文件名加密|文件加密                                           |压缩等级(1/2/3)
	DWORD zfBlockSize;	//单位kb
} ZPACKFILEHEADER;

typedef struct tagZPACKFILERECORD{
	DWORD		Signage;	//0x5A50414B
	DWORD		NextFileOffset;
	DWORD		frUncompressedCRC;
	WORD		frFileNameLength;
	WORD        FileNameOffset;
} ZPACKINFOHEADER;


typedef struct tagINFILEINFO
{
	WORD		Type;		//0->其他文件  1->带文件夹文件	//-1->文件继续写
	CHAR		FileName[255];
	ZPACK_FIL *pFil;
}IN_FILEINFO;
typedef struct tagOUTFILEINFO
{
	CHAR	*Key;
	CHAR	KeyHash[32];
	CHAR	FilePath[255];
	ZPACK_FIL *pFil;
}OUT_FILEINFO;

enum zPackStateFlags
{
	ZPACK_OK,
	ZPACK_CRC_ERROR,

	ZPACK_FILE_NOT_OPEN,
};



#if defined (__cplusplus)
extern "C" {
#endif
	FRESULT zf_open(
		ZPACK_FIL* fp,           /* [OUT] Pointer to the file object structure */
		const CHAR* path, /* [IN] File name */
		BYTE mode          /* [IN] Mode flags */
	);
	FRESULT zf_read(
		ZPACK_FIL* fp,     /* [IN] File object */
		void* buff,  /* [OUT] Buffer to store read data */
		UINT btr,    /* [IN] Number of bytes to read */
		UINT* br     /* [OUT] Number of bytes read */
	);
	FRESULT zf_write(
		ZPACK_FIL* fp,          /* [IN] Pointer to the file object structure */
		const void* buff, /* [IN] Pointer to the data to be written */
		UINT btw,         /* [IN] Number of bytes to write */
		UINT* bw          /* [OUT] Pointer to the variable to return number of bytes written */
	);
	FRESULT zf_close(
		ZPACK_FIL* fp     /* [IN] Pointer to the file object */
	);
	FRESULT zf_lseek(
		ZPACK_FIL*    fp,  /* [IN] File object */
		size_t ofs  /* [IN] File read/write pointer */
	);
	size_t zf_tell(
		ZPACK_FIL* fp   /* [IN] File object */
	);
	FRESULT zf_size(
		ZPACK_FIL* fp   /* [IN] File object */
	);

	size_t ZPack_GetFileSize(ZPACK_FIL*    fp);


	size_t zPack_compress(const void *source, char *destination, size_t size);
	size_t zPack_Decompress(const char *source, void *destination);
	size_t zPcak_GetSize_decompressed(const void *source);
	size_t zPack_GetSize_compressed(const char *source);

	UINT zPack_Compress_Start(IN_FILEINFO *in, OUT_FILEINFO *dest);
	UINT zPack_Compress_File(IN_FILEINFO *in, OUT_FILEINFO *dest, UINT *n);
	UINT zPack_Compress_End(OUT_FILEINFO *dest);

	DWORD GetCRC32(void *pStart, DWORD uSize);
	DWORD GetCRC32Ex(DWORD InitValue, void *pStart, DWORD uSize);

#if defined (__cplusplus)
}
#endif
