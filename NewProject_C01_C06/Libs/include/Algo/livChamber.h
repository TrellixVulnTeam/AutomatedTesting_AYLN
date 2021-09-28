#pragma once

#include "DaenerysCore.h"

typedef struct t_dcPdLinearityInput {
	float	iLoBound;
	float	iHiBound;
	float	pLoBound;
	float	pHiBound;
	float	modFreq;
	float 	dataSamplingRate;
	float	binNo;
//	float	sigThreshold;
//	float	fitBeginIdx;
//	float 	fitEndIdx;
//	float	sigVar;
	
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file.
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcPdLinearityInput;

/*
*	dcAlgo_processPdLinearity
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


DCore_API t_mlError dcAlgo_processPdLinearity(t_dcPdLinearityInput* input,
											  const double* ild, const double* popt, const double* wl, const double* v, const double* ipd, int dataSize,
											  int* outFomCount, double** outFom, char*** outFomName);

DCore_API t_mlError dcAlgo_processPdLinearityFromFile(t_dcPdLinearityInput* input, const char* inputFile,
													  int* outFomCount, double** outFom, char*** outFomName);
