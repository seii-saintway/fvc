/* ---------------------------------------------------------------
             FVC sample participant - match program

             BioLab University of Bologna - Italy
--------------------------------------------------------------- */

#include <stdio.h>
#include "..\Auxiliary\fvc.h"
#pragma comment(lib, "..\\Debug\\Auxiliary.lib")

int main(int argc, char * argv[]) {
    BOOL Upright = TRUE;

    // Load parameters
    if (argc!=5) {
        printf("\nSyntax error.\nUse: match <imagefile> <templatefile> <configfile> <outputfile>\n");
        return SYNTAX_ERROR;
    }

    Set_imagefile(argv[1]);
    //  Upright = TRUE   requires the image to be loaded Upright:     IMAGE[0] denotes the "top-left" pixel
    //  Upright = FALSE  requires the image to be loaded Upsidedown:  IMAGE[0] denotes the "bottom-left" pixel
    int err = Load_gray256tif(Upright);
    if (err) return TIF_LOAD_ERROR;


    Set_configfile(argv[3]);


    return SUCCESS;
}
