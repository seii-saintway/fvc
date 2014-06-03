/* ---------------------------------------------------------------
             FVC sample participant - enroll program

             BioLab University of Bologna - Italy
--------------------------------------------------------------- */

#include <stdio.h>
#include "..\Auxiliary\fvc.h"
#pragma comment(lib, "..\\Debug\\Auxiliary.lib")

int main(int argc, char * argv[]) {
    BOOL Upright = TRUE;
    BOOL Upsidedown = FALSE;

    // Load parameters
    if (argc!=5) {
        printf("\nSyntax error.\nUse: enroll <imagefile> <templatefile> <configfile> <outputfile>\n");
        return SYNTAX_ERROR;
    }

    Set_imagefile(argv[1]);
    //  Upright = TRUE   requires the image to be loaded Upright:     IMAGE[0] denotes the "top-left" pixel
    //  Upright = FALSE  requires the image to be loaded Upsidedown:  IMAGE[0] denotes the "bottom-left" pixel
    int err = Load_gray256tif(Upright);
    if (err) return TIF_LOAD_ERROR;

    Set_templatefile(argv[2]);
    Save_gray256tif(Upsidedown);

    Set_configfile(argv[3]);

    Set_outputfile(argv[4]);

    return SUCCESS;
}
