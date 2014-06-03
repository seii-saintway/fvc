/* ---------------------------------------------------------------
               FVC testing header file

            BioLab University of Bologna - Italy

				 v 1.0 - October 2001
--------------------------------------------------------------- */

#ifdef DLL_FILE

#define EXT_API __declspec(dllexport)

#else

#define EXT_API __declspec(dllimport)

#endif


#define MAX_HEAD_SIZE 1024
#define MAX_IMAGE_SIZE 640*640
#define MAX_PATH 256

#define SUCCESS                     0
#define SYNTAX_ERROR                1
#define CANNOT_OPEN_CONFIG_FILE     2
#define CANNOT_OPEN_OUTPUT_FILE     3
#define CANNOT_OPEN_IMAGE_FILE      4
#define CANNOT_OPEN_TEMPLATE_FILE   5
#define TIF_LOAD_ERROR              6
#define CANNOT_WRITE_TEMPLATE       7
#define CANNOT_UPDATE_OUTPUT_FILE   8

#define FALSE               0
#define TRUE                1

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef int                 BOOL;


extern BYTE  EXT_API  FILE_HEAD[MAX_HEAD_SIZE];
extern int   EXT_API  HEAD_SIZE;

extern BYTE  EXT_API  IMAGE[MAX_IMAGE_SIZE];
extern int   EXT_API  IMAGE_X, IMAGE_Y;


extern "C" {

int  EXT_API  Set_imagefile    (char * filepath);
int  EXT_API  Set_templatefile (char * filepath);
int  EXT_API  Set_configfile   (char * filepath);
int  EXT_API  Set_outputfile   (char * filepath);

int  EXT_API  Load_gray256tif  (BOOL Upright);
int  EXT_API  Save_gray256tif  (BOOL Upright);

}
