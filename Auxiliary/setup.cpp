#define DLL_FILE
#include "fvc.h"
#include "setup.h"
#include <string.h>
#include <math.h>

/* ----------------------- */
/*    File Private
/* ----------------------- */

char image_path[MAX_PATH];
char template_path[MAX_PATH];
BOOL TemplateCreated;
BOOL MatchingPerformed;
float similarity;

/* ----------------------- */
/*    Project Private
/* ----------------------- */

FILE * imagefile;
FILE * templatefile;
FILE * configfile;
FILE * outputfile;

/* ----------------------- */
/*    Public Interfaces
/* ----------------------- */

int Set_imagefile (char * filepath) {
    strcpy(image_path, filepath);

    imagefile = fopen(filepath, "rb");
    if (!imagefile) return CANNOT_OPEN_IMAGE_FILE;
//    Load_gray256tif(Upright);
//    fclose(imagefile);
    return SUCCESS;
}

int Set_templatefile (char * filepath) {
    strcpy(template_path, filepath);

    /* Create the fingerprint template */
    // The "template" is simply the mean of the image pixels
    int i;

    float mean = 0;
    for (i=0; i<IMAGE_X*IMAGE_Y; ++i) mean += IMAGE[i];
    mean /= (IMAGE_X*IMAGE_Y);

    if ( ((int)mean)%21==0 ) {
        // Simulate a failure to enroll!
        TemplateCreated = FALSE;
    }
    else TemplateCreated = TRUE;

    if (TRUE) {
        // Saves the template in templatefile
        templatefile = fopen(filepath, "wb");
        if (!templatefile) return CANNOT_WRITE_TEMPLATE;
//        Save_gray256tif(Upright);
//        fclose(templatefile);
    }
    return SUCCESS;


    /* Load fingerprint template file */
    float mean_model/*, mean*/;

    templatefile = fopen(filepath, "rb");
    if (!templatefile) return CANNOT_OPEN_TEMPLATE_FILE;
    fread(&mean_model, 4, 1, templatefile);
    fclose(templatefile);

    /* Matching: a simple difference between the two mean values*/
    mean = 0;
    for (i=0; i<IMAGE_X*IMAGE_Y; ++i) mean += IMAGE[i];
    mean /= (IMAGE_X*IMAGE_Y);

    similarity = 1.0f - (float)(fabs(mean - mean_model) / 255);

    if ( ((int)mean)%19==0 ) {
        // Simulate a failure to match!
        MatchingPerformed = FALSE;
    } else MatchingPerformed = TRUE;

}

int Set_configfile (char * filepath) {
    /* open the configfile */
    configfile = fopen(filepath, "rt");
    // no config data to read...
    if (!configfile) return CANNOT_OPEN_CONFIG_FILE;
    fclose(configfile);
    return SUCCESS;
}

int Set_outputfile (char * filepath) {
    /* Send the results to outputfile */
    outputfile = fopen(filepath, "at");
    if (!outputfile) return CANNOT_OPEN_OUTPUT_FILE;
    if ( fprintf(outputfile, "%s %s %s\n", image_path, template_path, TemplateCreated?"OK":"FAIL") <=0 )
        return CANNOT_UPDATE_OUTPUT_FILE;
    fclose(outputfile);
    return SUCCESS;


    /* Send the results to outputfile */
    outputfile = fopen(filepath, "at");
    if (!outputfile) return CANNOT_OPEN_OUTPUT_FILE; 
    if ( fprintf(outputfile, "%s %s %s %8.6f\n", image_path, template_path, MatchingPerformed?"OK":"FAIL", MatchingPerformed?similarity:0.0F) <=0 )
        return CANNOT_UPDATE_OUTPUT_FILE;
    fclose(outputfile);

}
