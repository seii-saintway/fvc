#define DLL_FILE
#include "fvc.h"
#include "setup.h"

/* ----------------------- */
/*    File Private
/* ----------------------- */

BYTE buffer[512];

DWORD in_dword(DWORD i) {
    DWORD v=0;

    v=v|(buffer[i]);
    v=v|(buffer[i+1]<<8);
    v=v|(buffer[i+2]<<16);
    v=v|(buffer[i+3]<<24);
    return v;
}

WORD in_word(DWORD i) {
    WORD v=0;

    v=v|(buffer[i]);
    v=v|(buffer[i+1]<<8);
    return v;
}

/* ----------------------- */
/*    Public Interfaces
/* ----------------------- */

BYTE FILE_HEAD[MAX_HEAD_SIZE];
int HEAD_SIZE;

BYTE IMAGE[MAX_IMAGE_SIZE];
int IMAGE_X, IMAGE_Y;

// Load a 256 gray-scale uncompressed TIF image into the global array IMAGE
int Load_gray256tif(BOOL Upright) {
    /* Load a tif imagefile.
       The image is loaded by rows into the global array IMAGE:
       - IMAGE_X and IMAGE_Y are the width and the height of the image respectively.
       - The generic pixel [x,y], where x=0..IMAGE_X-1 and y=0..IMAGE_Y-1,
         can be accessed as IMAGE[y*IMAGE_X+x]
    */
    FILE* fp = imagefile;
    DWORD ifd_offset;
    WORD directory_entry_count;
    WORD offset;
    DWORD strip_offset, data_offset;
    BOOL strip_based = FALSE;
    BYTE* pimage;
    int i;

    if (fread(buffer,8,1,fp)!=1) return 1;
    if (in_word(0)!=0x4949) return 2;
    if (in_word(2)!=0x002a) return 3;
    ifd_offset = in_dword(4);

    if (fseek(fp,ifd_offset,SEEK_SET)) return 1;
    if (fread(buffer,2,1,fp)!=1) return 1;
    directory_entry_count=in_word(0);

    if (fread(buffer,directory_entry_count*12,1,fp)!=1) return 1;

    offset = 0;
    while (directory_entry_count > 0) {
        switch (in_word(offset)) {
        case 0x00fe: if (in_word(offset+8) != 0) return 4; break;
        case 0x0100: IMAGE_X = in_word(offset+8); break;
        case 0x0101: IMAGE_Y = in_word(offset+8); break;
        case 0x0102: if (in_word(offset+8) != 8) return 5; break;
        case 0x0103: if (in_word(offset+8) != 1) return 6; break;
        case 0x0106: if (in_word(offset+8) != 1) return 7; break;
        case 0x0111: strip_offset = in_word(offset+8); break;
        case 0x0115: if (in_word(offset+8) != 1) return 8; break;
        case 0x0116: if (in_word(offset+8) != IMAGE_Y) strip_based=TRUE; break;
        case 0x011c: if (in_word(offset+8) != 1) return 11; break;
        }
        offset += 12;
        -- directory_entry_count;
    }

    if (strip_based) {
        if (fseek(fp, strip_offset, SEEK_SET)) return 1;
        if (fread(buffer, 4, 1, fp) != 1) return 1;
        data_offset = in_dword(0);
    } else data_offset = strip_offset;
    HEAD_SIZE = data_offset;

    if( fseek(fp, 0, SEEK_SET) ) return 1;
    if( fread(FILE_HEAD, HEAD_SIZE, 1, fp) != 1 ) return 1;

    if( fseek(fp, data_offset, SEEK_SET) ) return 1;
    if (Upright) {
        pimage = IMAGE;
        for (i=0; i<IMAGE_Y; ++i) {
            if( fread(pimage, IMAGE_X, 1, fp) != 1 ) return 1;
            pimage += IMAGE_X;
        }
    } else {
        pimage = IMAGE + IMAGE_X*(IMAGE_Y-1);
        for (i=0; i<IMAGE_Y; ++i) {
            if( fread(pimage, IMAGE_X, 1, fp) != 1 ) return 1;
            pimage -= IMAGE_X;
        }
    }

    return 0;
}

int Save_gray256tif(BOOL Upright) {
    FILE* fp = templatefile;
    BYTE* pimage;
    int i;

    if( fwrite(FILE_HEAD, HEAD_SIZE, 1, fp) != 1 ) return 1;

    if (Upright) {
        pimage = IMAGE;
        for (i=0; i<IMAGE_Y; ++i) {
            if( fwrite(pimage, IMAGE_X, 1, fp) != 1 ) return 1;
            pimage += IMAGE_X;
        }
    } else {
        pimage = IMAGE + IMAGE_X*(IMAGE_Y-1);
        for (i=0; i<IMAGE_Y; ++i) {
            if( fwrite(pimage, IMAGE_X, 1, fp) != 1 ) return 1;
            pimage -= IMAGE_X;
        }
    }

    return 0;
}
