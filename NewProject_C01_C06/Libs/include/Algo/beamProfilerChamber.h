#pragma once

#include "DaenerysCore.h"

typedef struct t_dcBpInput {
	float	pixelSize;			// Pixel Size, in mm
	float	cropHalfSize;		// number of pixels to crop, 2*cropHalfSize+1 during BP processing
	float	icutoffRatio;		// threshold intensity ratio
	float	diameterAperture;	// Should be 7, in mm
	float	distanceAperture;	// Should be 100, in mm
	float	imgThreshold;		// Should be 2.5;
							//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcBpInput;

/*
*	dcAlgo_processBpFromCsv 
*	WARNING: Slow, use Tiff input instead
*
*	API to process test data from Beam Profiler with CSV file input
*	Input:
*		input			- pointer to an t_dcBpInput structure
*		distance1		- Distance of the detector for the 1st measurement, in mm
*		bpCsvFile1		- Full file path string toward the CSV file for the 1st measurement image
*		distance2		- Distance of the detector for the 2nd measurement, in mm
*		bpCsvFile2		- Full file path string toward the CSV file for the 2nd measurement image
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- [Optional] pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processBpFromCsv(t_dcBpInput* input, 
											float distance1, const char* bpCsvFile1, float distance2, const char* bpCsvFile2, 
											int* outFomCount, double** outFom, char*** outFomName);

/*
*	dcAlgo_processBpFromTiff
*
*	API to process test data from Beam Profiler with Tiff file input
*	Input:
*		input			- pointer to an t_dcBpInput structure
*		distance1		- Distance of the detector for the 1st measurement, in mm
*		bpTiffFile1		- Full file path string toward the Tiff file for the 1st measurement image
*		distance2		- Distance of the detector for the 2nd measurement, in mm
*		bpTiffFile2		- Full file path string toward the Tiff file for the 2nd measurement image
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- [Optional] pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processBpFromTiff(t_dcBpInput* input,
											float distance1, const char* bpTiffFile1, float distance2, const char* bpTiffFile2,
											int* outFomCount, double** outFom, char*** outFomName);
