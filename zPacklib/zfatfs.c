#include"zPacklib.h"
#include <stdio.h>

//#include <stdlib.h>

#if (defined(__X86__) || defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(__386__) || defined(__x86_64__) || defined(_M_X64))
#define X86X64
#endif

#ifdef X86X64

FRESULT zf_open(
	ZPACK_FIL** fp,           /* [OUT] Pointer to the file object structure */
	const CHAR* path, /* [IN] File name */
	BYTE mode          /* [IN] Mode flags */
)
{
	char strmode[10] = { 0 };
	switch (mode)
	{
		case FA_READ:
			strcpy_s(strmode, 9, "rb");
			break;
		case FA_CREATE_ALWAYS | FA_WRITE:
			strcpy_s(strmode, 9, "wb");
			break;
		case FA_CREATE_ALWAYS | FA_WRITE | FA_READ:
			strcpy_s(strmode, 9, "wb+");
			break;
		case FA_OPEN_APPEND | FA_WRITE:
			strcpy_s(strmode, 9, "ab");
			break;
		case FA_OPEN_APPEND | FA_WRITE | FA_READ:
			strcpy_s(strmode, 9, "ab+");
			break;
		case FA_CREATE_NEW | FA_WRITE:
			strcpy_s(strmode, 9, "wx");
			break;
		case FA_CREATE_NEW | FA_WRITE | FA_READ:
			strcpy_s(strmode, 9, "wb+x");
			break;
		case FA_READ | FA_WRITE:
		default:	//Ä¬ÈÏ¶ÁÐ´
			strcpy_s(strmode, 9, "rb+");
			break;

	}
	errno_t err = fopen_s(fp, path, strmode); //open file for read/write  
	return err;
}
FRESULT zf_read(
	ZPACK_FIL* fp,     /* [IN] File object */
	void* buff,  /* [OUT] Buffer to store read data */
	UINT btr,    /* [IN] Number of bytes to read */
	UINT* br     /* [OUT] Number of bytes read */
)
{
	*br = fread_s(buff, btr, 1, btr, fp);
	return ferror(fp);
}
FRESULT zf_write(
	ZPACK_FIL* fp,          /* [IN] Pointer to the file object structure */
	const void* buff, /* [IN] Pointer to the data to be written */
	UINT btw,         /* [IN] Number of bytes to write */
	UINT* bw          /* [OUT] Pointer to the variable to return number of bytes written */
)
{
	*bw = fwrite(buff, 1, btw, fp);
	return ferror(fp);
}
//FRESULT zf_close(
//	ZPACK_FIL* fp     /* [IN] Pointer to the file object */
//)
//{
//	fclose(fp);
//	return 0;
//}
//FRESULT zf_lseek(
//	ZPACK_FIL*    fp,  /* [IN] File object */
//	size_t ofs  /* [IN] File read/write pointer */
//)
//{
//	return fseek(fp, ofs, SEEK_SET);
//}
//size_t zf_tell(
//	ZPACK_FIL* fp   /* [IN] File object */
//)
//{
//	return ftell(fp);
//}

size_t zf_size(
	ZPACK_FIL* fp   /* [IN] File object */
)
{
	size_t loc = ftell(fp);
	size_t fsize;
	fseek(fp, 0L, SEEK_END);
	fsize = ftell(fp);
	fseek(fp, loc, SEEK_SET);
	return fsize;
}

/*The f_eof function returns a non-zero value if the read/write pointer has reached end of the file;
otherwise it returns a zero.*/
//int zf_eof(
//	ZPACK_FIL* fp   /* [IN] File object */
//)
//{
//	return feof(fp);
//}

#endif // X86X64

