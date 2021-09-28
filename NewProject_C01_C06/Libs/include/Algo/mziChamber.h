#pragma once

#include "DaenerysCore.h"

typedef struct t_dcMZIInput {
	float	modFreq;		// Triangluar wave modulation frequency, in Hz
	float	ipp;			// current peak2peak value, in mA
							//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file.
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcMZIInput;

/*
*	dcAlgo_processMZI
*
*	API to process test data from MZI Chamber
*	Input:
*		input			- pointer to an t_dcTargetInput structure
*		timeData		- pointer to the time stamp data array
*		mziOut			- pointer to the MZI output data array
*		pMonitor		- pointer to the power monitoring data array
*		dataSize		- size of the data points
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- [Optional] pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processMZI(t_dcMZIInput* input, const double* timeData, const double* mziOut, const double* pMonitor, int dataSize, 
	int* outFomCount, double** outFom, char*** outFomName);

/*
*	dcAlgo_processMZIFromFile
*
*	API to process test data from MZI Chamber, with input data from csv file
*	Input:
*		input			- pointer to an t_dcMZIInput structure
*		inputFile		- C string to the input csv file
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processMZIFromFile(t_dcMZIInput* input, const char* inputFile, 
	int* outFomCount, double** outFom, char*** outFomName);
