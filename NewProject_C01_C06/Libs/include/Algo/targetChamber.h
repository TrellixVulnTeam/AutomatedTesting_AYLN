#pragma once

#include "DaenerysCore.h"

typedef enum {
	k_dcTargetType_noTarget = 0,
	k_dcTargetType_whiteCard, // Lambertian Target, including white/grey/black card
	k_dcTargetType_retroReflective,
} t_dcTargetType;

#pragma mark -
#pragma mark Static Target Algorithm

typedef struct t_dcTargetInput {
	float	GainTIA;		// TIA Gain, in MOhm
	float	modFreq;		// Triangluar wave modulation frequency, in Hz
	float	distance;		// Measured distance, in mm
	uint8_t targetType;		// Target Type, t_dcTargetType
	float 	pts2ignore;	// points to ignore at the beginning and end in raw data
	// parameters for no target functions
	float sigThreshold;
	uint32_t fitBeginIdx;
	uint32_t fitEndIdx;
	float sigVar;
	//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcTargetInput;

/*
*	dcAlgo_processTarget
*
*	API to process test data from Target Chamber
*	Input:
*		input			- pointer to an t_dcTargetInput structure
*		timeData		- pointer to the time stamp data array
*		voltageData		- pointer to the voltage data array
*		dataSize		- size of the timeData and voltageData
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- [Optional] pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processTarget(t_dcTargetInput* input,
										 const double* timeData, const double* voltageData, size_t dataSize,
										 int* outFomCount, double** outFom, char*** outFomName);

/*
*	dcAlgo_processTargetFromFile
*
*	API to process test data from Target Chamber, with input data from csv file
*	Input:
*		input			- pointer to an t_dcTargetInput structure
*		inputFile		- C string to the input csv file
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processTargetFromFile(t_dcTargetInput* input, const char* inputFile,
												 int* outFomCount, double** outFom, char*** outFomName);

#pragma mark -
#pragma mark Multiple Static Target Algorithm

typedef struct t_dcMultiTargetInput {
	float	GainTIA;		// TIA Gain, in MOhm
	float	modFreq;		// Triangluar wave modulation frequency, in Hz
	uint8_t targetType;		// Target Type, t_dcTargetType
	float 	pts2ignore;	// points to ignore at the beginning and end in raw data
	//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcMultiTargetInput;

/*
*	dcAlgo_processMultiTarget
*
*	API to process test data from Target Chamber with multiple distance
*	Input:
*		input				- pointer to an t_dcMultiTargetInput structure
*		timeDataGroup		- pointer to the time stamp data array, 1D should match numOfDis, 2D should match dataSize
*		voltageDataGroup	- pointer to the voltage data array, 1D should match numOfDis, 2D should match dataSize
*		dataSizeGroup		- pointer to the size of the timeData and voltageData, for each target
*		distances			- pointer to the distance data array
*		numOfDis			- number of distance
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount			- pointer to the variable storing the number of FOM to be exported
*		outFom				- pointer to the array of data storing the data of each FOM
*		outFomName			- [Optional] pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processMultiTarget(t_dcMultiTargetInput* input,
											  const double** timeDataGroup, const double** voltageDataGroup, const int* dataSizeGroup,
											  const double* distances, int numOfDis,
											  int* outFomCount, double** outFom, char*** outFomName);

/*
*	dcAlgo_processMultiTargetFromFile
*
*	API to process test data from Target Chamber, with input data from csv file, with multiple distance
*	Input:
*		input			- pointer to an t_dcMultiTargetInput structure
*		inputFileGroup	- C string to the input csv file
*		distances		- pointer to the distance data array
*		numOfDis		- number of distance
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processMultiTargetFromFile(t_dcMultiTargetInput* input,
													  const char** inputFileGroup, const double* distances, int numOfDis,
													  int* outFomCount, double** outFom, char*** outFomName);

#pragma mark -
#pragma mark Multiple Static Dynamic Target Algorithm

typedef struct t_dcDynTargetInput {
	double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
	float	GainTIA;		// TIA Gain, in MOhm
	float	windowFFT;		// FFT window time duration, in second
	float	windowFFTIntv;	// FFT window interval
	float 	cutoffFreq;		// Cut off frequency, in Hz
	uint8_t targetType;		// Target Type, t_dcTargetType
	//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcDynTargetInput;

DCore_API t_mlError dcAlgo_processDynTarget(t_dcDynTargetInput* input,
											const double** timeData, const double** voltageData, const int* dataSize, int numOfDataSet,
											int* outFomCount, double** outFom, char*** outFomName);

DCore_API t_mlError dcAlgo_processDynTargetFromFile(t_dcDynTargetInput* input,
													const char** inputFileGroup, int numOfFiles,
													int* outFomCount, double** outFom, char*** outFomName);

#pragma mark -
#pragma mark Dynamic Target with Triangle Drive Algorithm
typedef struct t_dcDynTargetTriaInput {
	float	GainTIA;		// TIA Gain, in MOhm
	float	triaFreq;		// triangle waveform modulation frequency
	uint8_t targetType;		// Target Type, t_dcTargetType
	float 	pts2ignore;	// points to ignore at the beginning and end in raw data
	float	distance;		// Measured distance, in mm
	//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcDynTargetTriaInput;

DCore_API t_mlError dcAlgo_processDynTargetTria(t_dcDynTargetTriaInput* input,
												const double** timeData, const double** voltageData, const int* dataSize, int numOfDataSet,
												int* outFomCount, double** outFom, char*** outFomName);

DCore_API t_mlError dcAlgo_processDynTargetTriaFromFile(t_dcDynTargetTriaInput* input,
														const char** inputFileGroup, int numOfFiles,
														int* outFomCount, double** outFom, char*** outFomName);

#pragma mark -
#pragma mark Multiple Static Dynamic Target with Envolope Algorithm

typedef struct t_dcDynTargetEnvInput {
	double	velocity[100];	// mm/s, measured velocity on Axis, vendor input
	float	GainTIA;		// TIA Gain, in MOhm
	float	targetAngle;	// 45 deg incident angle of light on target
	float 	FDthr;			// 1.4 uA Threshold for dropout
	float	dataSamplingRate;	// 2.5e6 Hz 
	int		enablePlot;		// 1:enable plot output; 0: disable plot output
							//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_dcDynTargetEnvInput;

DCore_API t_mlError dcAlgo_processDynTargetEnv(t_dcDynTargetEnvInput* input,
	const double** timeData, const double** voltageData, const int* dataSize, int numOfDataSet,
	int* outFomCount, double** outFom, char*** outFomName);

DCore_API t_mlError dcAlgo_processDynTargetEnvFromFile(t_dcDynTargetEnvInput* input,
	const char** inputFileGroup, int numOfFiles,
	int* outFomCount, double** outFom, char*** outFomName);

#pragma mark -
#pragma mark PCB Triangle Drive Audit Algorithm
DCore_API t_mlError dcAlgo_processPCBTriaAuditFromFile(t_dcTargetInput* input,
	const char* inputFile, int* outFomCount, double** outFom, char*** outFomName);

#pragma mark -
#pragma mark PCB DC Drive Audit Algorithm

typedef struct t_pcbDcAuditInput {
	float	GainTIA;		// TIA Gain, in MOhm
	//float	dataSamplingRate;// dataSamplingRate can also be manual input by usery, in Hz
	double	FFTwindow;		// [s] FFT window size: 2e-4
	float 	Iset;			// [uA]
	uint8_t targetType;		// Target Type, t_dcTargetType

	//
	const char*	dutSn;		// DUT Sn used for output result file.
	const char*	outputDir;	// file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		// [Optional] Logger, set to NULL if not needed
}t_pcbDcAuditInput;

/*
*	dcAlgo_processPCBDcAuditFromFile
*
*	API to process test data from Target Chamber, with input data from csv file
*	Input:
*		input			- pointer to an t_dcTargetInput structure
*		inputFile		- C string to the input csv file
*	Output: If NULL in outFomCount or outFom, the FOM output procedure will be skipped
*		outFomCount		- pointer to the variable storing the number of FOM to be exported
*		outFom			- pointer to the array of data storing the data of each FOM
*		outFomName		- pointer to the char array of the data storing the name of each FOM
*/
DCore_API t_mlError dcAlgo_processPCBDcAuditFromFile(t_pcbDcAuditInput* input, const char* inputFile,
	int* outFomCount, double** outFom, char*** outFomName);


typedef struct t_jptMaestroInput {
	double	velocity[100];		// measured velocity or rpm, vendor input, in mm/s
	float	GainTIA;		    // TIA Gain, in MOhm
	float	targetAngle;	    // AOI incident angle of light on target, depends on Corner
	float 	FDthr;			    // Deprecated, not used, but kept here in case
	float	dataSamplingRate;	// 625KHz for JPT tester
	int		enablePlot;         // Enable Plot or not
	int     isNoiseFoM;         // Toggle between noise and Isig FoM
	const char*	dutSn;		    // DUT Sn used for output result file.
	const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
}t_jptMaestroInput;

typedef struct t_retroMaestroInput {
	double	Vrpm;		        // Shaft Rotation, in RPM.
	double  threshold;          // Threshold for Percentage Calc
	float	GainTIA;		    // TIA Gain, in MOhm
	float	targetAngle;	    // NOT USED IN RETRO - AOI incident angle of light on target, depends on Corner
	float	dataSamplingRate;	// 625KHz for JPT tester
	int		enablePlot;         // Enable Plot or not
	int     isNoiseFoM;         // Toggle between noise and Isig FoM
	const char*	dutSn;		    // DUT Sn used for output result file.
	const char*	outputDir;	    // file path for output result file, set to NULL if no output needed
	CML_logManager*	lm;		    // [Optional] Logger, set to NULL if not needed
}t_retroMaestroInput;


#pragma mark -
#pragma mark AC DC OFFSET COMBINE HELPER

DCore_API t_mlError dcAlgo_ACDCCombineStreams(t_jptMaestroInput* input1, t_jptMaestroInput* input2, 
	const double** timeData1, const double** acVoltageData, const int* acDataSize, 
	const double** timeData2, const double** dcVoltageData, const int* dcDataSize,
	const double** timeData3, const double** offsetVoltageData, const int* offsetDataSize,
	const double ACDCGainRatio,
	int* outFomCount1, double** outFom1, char*** outFomName1,
	int* outFomCount2, double** outFom2, char*** outFomName2);

DCore_API t_mlError dcAlgo_ACDCCombineStreamsFromFile(t_jptMaestroInput* input1, t_jptMaestroInput* input2,
	const char* acInputFile, const char* dcInputFile, const char* offsetInputFile,
	const double ACDCGainRatio,
	int* outFomCount1, double** outFom1, char*** outFomName1,
	int* outFomCount2, double** outFom2, char*** outFomName2);

#pragma mark -
#pragma mark JPT MAESTRO DYNAMIC TARGET FFT AND ENVELOPE SINGLE STREAM

DCore_API t_mlError dcAlgo_jptProcessSingle(t_jptMaestroInput* input,
	const double** timeData, const double** voltageData, const int* dataSize, int numOfDataSet,
	int* outFomCount, double** outFom, char*** outFomName);

DCore_API t_mlError dcAlgo_jptProcessSingleFromFile(t_jptMaestroInput* input,
	const char** inputFileGroup, int numOfFiles,
	int* outFomCount, double** outFom, char*** outFomName);

DCore_API t_mlError dcAlgo_retroProcessSingle(t_retroMaestroInput* input,
	const double** timeData, const double** voltageData, const int* dataSize, int numOfDataSet,
	int* outFomCount, double** outFom, char*** outFomName);

DCore_API t_mlError dcAlgo_retroProcessSingleFromFile(t_retroMaestroInput* input,
	const char** inputFileGroup, int numOfFiles,
	int* outFomCount, double** outFom, char*** outFomName);