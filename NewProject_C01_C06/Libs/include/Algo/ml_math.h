//
//  ml_math.h
//
//  Created by Joon Kwon on 8/27/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#ifndef _ML_MATH_H_
#define _ML_MATH_H_

#define _USE_MATH_DEFINES // for C++ on Windows
#include <math.h>

#ifdef __ML_USE_AS_EMB__
#include <ml_utils.h>
#else
#include <MLCore/ml_utils.h>
#endif

#ifdef _MSC_VER
#ifdef _IS_VALID_NUMBER_(val)
#undef _IS_VALID_NUMBER_(val)
#define _IS_VALID_NUMBER_(val)	true
#endif
#endif

#ifdef __ML_MATH_NO_DEPRECATED_WARNING__
#define MATH_DEPRECATED(func, msg) func
#else
#define MATH_DEPRECATED(func, msg) DEPRECATED_F(func, msg)
#endif

#if defined(__cplusplus)
extern "C" {
#endif
	
#ifndef MIN
#define	MIN(a,b) (((a)<(b))?(a):(b))
#endif /* MIN */
#ifndef MAX
#define	MAX(a,b) (((a)>(b))?(a):(b))
#endif	/* MAX */
	
#ifndef CLIP
#define	CLIP(min,val,max) (MAX((min),MIN((val),(max))))
#endif	/* CLIP */
	
#if !defined(ABS)
#	define ABS(a) ((a<0.0f)?-(a):(a))
#endif

#if defined(__APPLE__)
#	pragma mark -
#	pragma mark LeaseSquareLineFit
	MATH_DEPRECATED(t_mlError	mlMath_lineFitDouble(double *vx, double *vy, int leng, double *pSlope, double *pIntercept, double* pCoeff, double *pRsq), "Use mlMath_lineFit");

#	pragma mark PolynomialFit
	MATH_DEPRECATED(t_mlError	mlMath_polyFitDouble(double * vx, double * vy, int leng, int order, double * coeff), "Use mlMath_polyFit");
	MATH_DEPRECATED(t_mlError	mlMath_sineFitDouble(double* vx, double* vy, int length, double* amp,double* phs,double* bas), "Use mlMath_sineFit");
#endif
	
#	pragma mark NonLinear Least Square Fit
	// Function f(x) : y = a0 / (x + z0) + c0
	// resNorm = sum((f(x)-y)^2) / leng
	ML_API	t_mlError	mlMath_inverseFitDouble(double* vx, double* vy, int leng, double* a0, double* z0, double* c0, double* resNorm);
	ML_API	t_mlError	mlMath_polynomialFitDouble(double* vx, double* vy, int leng, int order, double* coeff);
	ML_API	t_mlError	mlMath_lineRtoFitDouble(double* vx, double* vy, int leng, double* slope);
	
#	pragma mark Vector / Matrix
#if defined(__APPLE__)
	// Vector DOT Product C = A dot B
	ML_API	float		mlMath_vectorDot(float* A, float* B, int leng);
	ML_API	double		mlMath_vectorDotD(double* A, double* B, int leng);
	// Moore-Penrose PseudoInverse of Matrix m (M x N)
	ML_API	t_mlError	mlMath_matrixPinv(float* mIn, float* mOut, int M, int N);
	ML_API	t_mlError	mlMath_matrixInv(float* mIn, float* mOut, int M, int N);
	ML_API	t_mlError	mlMath_matrixPinvD(double* mIn, double* mOut, int M, int N);
	ML_API	t_mlError	mlMath_matrixInvD(double* mIn, double* mOut, int M, int N);
	ML_API	t_mlError	mlMath_matrixDot(float* aIn, int Ma, int Na, float* bIn, int Mb, int Nb, float* C, int* pMc, int* pNc);
	ML_API	t_mlError	mlMath_matrixDotD(double* aIn, int Ma, int Na, double* bIn, int Mb, int Nb, double* cOut, int* pMc, int* pNc);
#endif
#	pragma mark misc.
	MATH_DEPRECATED(double		mlMath_gaussianFunction(double avg, double stdev, double x), "This function will be removed");
	
#	pragma mark max/min/span/avg/stdev/rms/med
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedUInt8_1D	(uint8_t*	data, int width, int x0, int x1, uint8_t*	pMax, uint8_t*	pMin, uint8_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedSInt8_1D	(int8_t*	data, int width, int x0, int x1, int8_t*	pMax, int8_t*	pMin, int8_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedUInt16_1D	(uint16_t*	data, int width, int x0, int x1, uint16_t*	pMax, uint16_t*	pMin, uint16_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedSInt16_1D	(int16_t*	data, int width, int x0, int x1, int16_t*	pMax, int16_t*	pMin, int16_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedUInt32_1D	(uint32_t*	data, int width, int x0, int x1, uint32_t*	pMax, uint32_t*	pMin, uint32_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedSInt32_1D	(int32_t*	data, int width, int x0, int x1, int32_t*	pMax, int32_t*	pMin, int32_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedFloat_1D	(float*		data, int width, int x0, int x1, float*		pMax, float*	pMin, float*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedDouble_1D	(double*	data, int width, int x0, int x1, double*	pMax, double*	pMin, double*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_1d");
	
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedUInt8_2D	(uint8_t*	data, int width, int x0, int x1, int y0, int y1, uint8_t*	pMax, uint8_t*	pMin, uint8_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedSInt8_2D	(int8_t*	data, int width, int x0, int x1, int y0, int y1, int8_t*	pMax, int8_t*	pMin, int8_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedUInt16_2D	(uint16_t*	data, int width, int x0, int x1, int y0, int y1, uint16_t*	pMax, uint16_t*	pMin, uint16_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedSInt16_2D	(int16_t*	data, int width, int x0, int x1, int y0, int y1, int16_t*	pMax, int16_t*	pMin, int16_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedUInt32_2D	(uint32_t*	data, int width, int x0, int x1, int y0, int y1, uint32_t*	pMax, uint32_t*	pMin, uint32_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedSInt32_2D	(int32_t*	data, int width, int x0, int x1, int y0, int y1, int32_t*	pMax, int32_t*	pMin, int32_t*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedFloat_2D	(float*		data, int width, int x0, int x1, int y0, int y1, float*		pMax, float*	pMin, float*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");
	MATH_DEPRECATED(void mlMath_FindMaxMinSpanAvgStdevRmsMedDouble_2D	(double*	data, int width, int x0, int x1, int y0, int y1, double*	pMax, double*	pMin, double*	pSpan, double* pAvg, double* pStdev, double* pRms, double* pMed), "Use mlMath_stat_2d");

#if defined(__cplusplus)
}
#include <vector>

#pragma mark -
#pragma mark [MAX,MIN,AVG,STDEV,MED]
/*--------------------------------------------------------------------------------------------------
 *	[MAX,MIN,AVG,STDEV,MED]
 *		mlMath_max
 *		mlMath_min
 *		mlMath_avg
 *		mlMath_stdev
 *		mlMath_median
 *		mlMath_stat
 *		mlMath_stat_2d
 *		mlMath_stat_1d
 */
template <typename T>	static inline void SWAP(T& a, T& b){T tmp = a; a=b; b=tmp;}

#pragma mark mlMath_sum
template <typename T>	ML_API	double mlMath_sum(const T* data, const size_t dataLeng){
	if (dataLeng<1) return NAN;
	double sum = 0;
	for (size_t i=0;i<dataLeng;i++){T val=data[i]; if(_IS_VALID_NUMBER_(val)) sum+=val; }
	return sum;
}
template <typename T>	ML_API	T mlMath_sum(const std::vector<T>& data){return mlMath_sum(data.data(),data.size());}

#pragma mark mlMath_max
template <typename T>	ML_API	T mlMath_max(const T* data, const size_t dataLeng, size_t* maxIdx = NULL){
	if (dataLeng<1) return NAN;
	if (maxIdx) *maxIdx = 0;
	T max = data[0]; for (size_t i=0;i<dataLeng;i++){
		T val=data[i]; if(_IS_VALID_NUMBER_(val)) { if(maxIdx) *maxIdx = (val>max)?i:*maxIdx; max=MAX(max,val);}
	}
	return max;}
template <typename T>	ML_API	T mlMath_max(const std::vector<T>& data, size_t* maxIdx = NULL){return mlMath_max(data.data(),data.size(), maxIdx);}

#pragma mark mlMath_min
template <typename T>	ML_API	T mlMath_min(const T* data, const size_t dataLeng, size_t* minIdx = NULL){
	if (dataLeng<1) return NAN;
	if (minIdx) *minIdx = 0;
	T min = data[0]; for (size_t i=0;i<dataLeng;i++){
		T val=data[i]; if(_IS_VALID_NUMBER_(val)) { if(minIdx)*minIdx = (val<min)?i:*minIdx; min = MIN(min,val);}
	}
	return min;}
template <typename T>	ML_API	T mlMath_min(const std::vector<T>& data, size_t* minIdx = NULL){return mlMath_min(data.data(),data.size(), minIdx);}

#pragma mark mlMath_avg
template <typename T>	ML_API	double mlMath_avg(const T* data, const size_t dataLeng){
	if (dataLeng<1) return NAN;
	int cnt = 0; double sum=0; for (int i=0;i<dataLeng;i++){T val=data[i]; if(_IS_VALID_NUMBER_(val)){sum += val;cnt++;}}
	return cnt>0?sum/cnt:NAN;}
template <typename T>	ML_API	double mlMath_avg(const std::vector<T>& data){return mlMath_avg(data.data(),data.size());}

#pragma mark mlMath_stdev
template <typename T>	ML_API	double mlMath_stdev(const T* data, const size_t dataLeng, const double avg = NAN){
	if (dataLeng<2) return NAN;
	double a = _IS_VALID_NUMBER_(avg)?avg:mlMath_avg(data, dataLeng);
	int cnt = 0; double rtn = 0; for (size_t i=0;i<dataLeng;i++){T val=data[i]; if(_IS_VALID_NUMBER_(val)){rtn+= pow(val-a,2);cnt++;}}
	return cnt>1?sqrt(rtn/(cnt-1)):NAN;}
template <typename T>	ML_API	double mlMath_stdev(const std::vector<T>& data, const double avg = NAN){return mlMath_stdev(data.data(),data.size(),avg);}

#pragma mark mlMath_rms
template <typename T>	ML_API	double mlMath_rms(const T* data, const size_t dataLeng){
	if (dataLeng<1) return NAN;
	int cnt = 0; double rtn = 0; for (size_t i=0;i<dataLeng;i++){T val=data[i]; if(_IS_VALID_NUMBER_(val)){rtn+= pow(val,2);cnt++;}}
	return cnt>0?sqrt(rtn/cnt):NAN;}
template <typename T>	ML_API	double mlMath_rms(const std::vector<T>& data){return mlMath_rms(data.data(),data.size());}

#pragma mark mlMath_median
template <typename T>	ML_API	double mlMath_median(const T* data, const size_t dataLeng){
	if (dataLeng<2) return NAN;
	std::vector<T> buff;
	for (size_t i=0;i<dataLeng;i++){T val=data[i]; if(_IS_VALID_NUMBER_(val)) buff.push_back(val);}
	size_t sz = buff.size();
	T* pData = buff.data();
	for (int i=0;i<(sz-1);i++) for (int j=i+1;j<sz;j++) if (pData[i]>pData[j]) SWAP(pData[i], pData[j]);
	return !(sz%2)?(buff[sz/2-1]+buff[sz/2])/2.0f:(buff[sz/2]);}
template <typename T>	ML_API	double mlMath_median(const std::vector<T>& data){return mlMath_median(data.data(),data.size());}

#pragma mark mlMath_stat
#pragma pack(1)
struct t_mlMathStat{
	size_t cnt;
	double maxVal,minVal,avgVal,stdevVal,rmsVal,medianVal;
	t_mlMathStat():cnt(0),maxVal(NAN),minVal(NAN),avgVal(NAN),stdevVal(NAN),rmsVal(NAN),medianVal(NAN){};
};
#pragma pack()
template <typename T>	t_mlMathStat mlMath_stat(const T* data, const size_t dataLeng){
	t_mlMathStat rtn;
	if (dataLeng<1) return rtn;
	rtn.cnt = dataLeng;
	rtn.maxVal = data[0]; rtn.minVal = data[0]; rtn.avgVal = 0;
	for (size_t i=0;i<dataLeng;i++){double val = data[i]; rtn.maxVal = MAX(rtn.maxVal,val); rtn.minVal = MIN(rtn.minVal,val); rtn.avgVal += val;}
	rtn.avgVal/=dataLeng;
	rtn.stdevVal = mlMath_stdev(data, dataLeng, rtn.avgVal);
	rtn.rmsVal = mlMath_rms(data, dataLeng);
	rtn.medianVal = mlMath_median(data, dataLeng);
	return rtn;}
template <typename T>	ML_API	t_mlMathStat mlMath_stat(const std::vector<T>& data){return mlMath_stat(data.data(),data.size());}

#pragma mark mlMath_stat_2d
template <typename T>	ML_API	t_mlMathStat mlMath_stat_2d(const T* data, const size_t width, const size_t x0, const size_t x1, const size_t y0, const size_t y1){
	std::vector<T> d; for (size_t rowIdx=y0; data && rowIdx<y1; rowIdx++) for (size_t colIdx=x0; colIdx<x1; colIdx++) d.push_back(data[rowIdx*width+colIdx]);
	return mlMath_stat(d);}
template <typename T>	ML_API	t_mlMathStat mlMath_stat_2d(const std::vector<T>& data, const size_t width, const size_t x0, const size_t x1, const size_t y0, const size_t y1){
	T* ptr = (T*)data.data(); if (width<y1 || x1<=x0 || y1<=y0 || data.size()<x1*y1) ptr = NULL;
	return mlMath_stat_2d(ptr, width, x0, x1, y0, y1);}

#pragma mark mlMath_stat_1d
template <typename T>	ML_API	t_mlMathStat mlMath_stat_1d(const T* data, const size_t length, const size_t x0, const size_t x1){
	return mlMath_stat_2d(data, length, x0, x1, 0, 1);}
template <typename T>	ML_API	t_mlMathStat mlMath_stat_1d(const std::vector<T>& data, const size_t x0, const size_t x1){
	return mlMath_stat_1d(data, data.size(), x0, x1);}

#pragma mark -
#pragma mark [LEASE SQUARE LINE FIT & R_squared]
/*--------------------------------------------------------------------------------------------------
 *	[LEASET SQUARE LINE FIT & R_squared]
 *		mlMath_lineFit
 *		calculate slope, intercept and r_squared from vx,vy (dataLen points)
 *			y = slope * x + intercept
 */
#pragma pack(1)
struct t_mlMathLineFit {
	double slope;
	double intercept;
	double r_squared;
	t_mlMathLineFit():slope(NAN),intercept(NAN),r_squared(NAN){};
};
#pragma pack()

#pragma mark mlMath_lineFit
ML_API	t_mlMathLineFit mlMath_lineFit(const double *vx, const double *vy, const size_t dataLen);

template <typename X,typename Y> ML_API	t_mlMathLineFit mlMath_lineFit(const X *x, const Y *y, const size_t dataLen){
	if (typeid(X)==typeid(double) && typeid(Y)==typeid(double))
		return mlMath_lineFit((const double*)x,(const double*)y,dataLen);
	std::vector<double> vx, vy;
	for (size_t i=0; i<dataLen; i++){vx.push_back(x[i]);vy.push_back(y[i]);}
	return mlMath_lineFit(vx.data(),vy.data(),dataLen);}
template <typename X,typename Y> ML_API	t_mlMathLineFit mlMath_lineFit(const std::vector<X>& x, const std::vector<Y>& y){
	if (typeid(X)==typeid(double)&&typeid(Y)==typeid(double))
		return mlMath_lineFit((const double*)x.data(),(const double*)y.data(),x.size());
	std::vector<double> vx(x.begin(),x.end()), vy(y.begin(),y.end());
	return mlMath_lineFit(vx.data(),vy.data(),vx.size());}

#pragma mark -
#pragma mark [POLYNOMIAL FIT]
#if defined(__APPLE__)
/*--------------------------------------------------------------------------------------------------
 *	[POLYNOMIAL FIT]
 *		mlMath_polyFit
 *		calculate polynomial coefficient ('order'-th order) from vx, vy
 *
 *		for example, where order=2, the return value will have the coefficient
 *			y = coeff_2 * x^2 + coeff_1 * x + coeff_0
 *			rtn[0] = coeff_0
 *			rtn[1] = coeff_1
 *			rtn[2] = coeff_2
 */

#pragma mark mlMath_polyFit
ML_API	std::vector<double> mlMath_polyFit(const double *vx, const double *vy, const size_t dataLen, const unsigned order);

ML_API	template <typename X,typename Y> std::vector<double> mlMath_polyFit(const X *x, const Y *y, const size_t dataLen, const unsigned order){
	if (typeid(X)==typeid(double) && typeid(Y)==typeid(double))
		return mlMath_polyFit((const double*)x,(const double*)y,dataLen,order);
	std::vector<double> vx, vy;
	for (size_t i=0; i<dataLen; i++){vx.push_back(x[i]);vy.push_back(y[i]);}
	return mlMath_polyFit(vx.data(),vy.data(),dataLen,order);}
ML_API	template <typename X,typename Y> std::vector<double> mlMath_polyFit(const std::vector<X>& x, const std::vector<Y>& y, const unsigned order){
	if (typeid(X)==typeid(double)&&typeid(Y)==typeid(double))
		return mlMath_polyFit((const double*)x.data(),(const double*)y.data(),x.size(),order);
	std::vector<double> vx(x.begin(),x.end()), vy(y.begin(),y.end());
	return mlMath_polyFit(vx.data(),vy.data(),vx.size(),order);}

#pragma mark -
#pragma mark [SINE FIT]
/*--------------------------------------------------------------------------------------------------
 *	[SINE FIT]
 *		mlMath_sineFit
 *
 *		calculate amplitude, phase, dc_offset from vx,vy (dataLen points)
 *		y = amplitude * cos (x - phase) + dc_offset
 */
#pragma mark pack(1)
struct t_mlMathSineFit {
	double amplitude;
	double phase;
	double dc_offset;
	t_mlMathSineFit():amplitude(NAN),phase(NAN),dc_offset(NAN){};
};
#pragma mark pack()

#pragma mark mlMath_sineFit
ML_API	t_mlMathSineFit mlMath_sineFit(const double* vx, const double* vy, const size_t dataLen);

ML_API	template <typename X,typename Y> t_mlMathSineFit mlMath_sineFit(const X *x, const Y *y, const size_t dataLen){
	if (typeid(X)==typeid(double) && typeid(Y)==typeid(double))
		return mlMath_sineFit((const double*)x,(const double*)y,dataLen);
	std::vector<double> vx, vy;
	for (size_t i=0; i<dataLen; i++){vx.push_back(x[i]);vy.push_back(y[i]);}
	return mlMath_sineFit(vx.data(),vy.data(),dataLen);}
ML_API	template <typename X,typename Y> t_mlMathSineFit mlMath_sineFit(const std::vector<X>& x, const std::vector<Y>& y){
	if (typeid(X)==typeid(double)&&typeid(Y)==typeid(double))
		return mlMath_sineFit((const double*)x.data(),(const double*)y.data(),x.size());
	std::vector<double> vx(x.begin(),x.end()), vy(y.begin(),y.end());
	return mlMath_sineFit(vx.data(),vy.data(),vx.size());}
#endif//__APPLE__

#pragma mark -
#pragma mark [Gaussian FIT]
/*--------------------------------------------------------------------------------------------------
 *	[Gaussian FIT]
 *		mlMath_gaussianFit
 *
 *		calculate mean, std from vx,vy (dataLen points)
 *		y = 1/sqrt(2*std*std*pi) * e^(-(x-mean)^2/(2*std*std))
 *      y =  a1 * exp(-((x-b1)/c1)^2)
 */
#pragma mark mlMath_gaussianFit
#pragma pack(1)
struct t_mlMathGaussianFit {
	double a1; // 1 / sqrt(2 * std * std * pi)
	double c1; // sqrt(2) * std
	double b1; // mean
	t_mlMathGaussianFit():a1(NAN),b1(NAN),c1(NAN){};
};
#pragma pack()
ML_API	t_mlMathGaussianFit	mlMath_gaussianFit(const double* vx, const double* vy, const size_t dataLen);

template <typename X,typename Y> ML_API	t_mlMathGaussianFit mlMath_gaussianFit(const X *x, const Y *y, const size_t dataLen){
	if (typeid(X)==typeid(double) && typeid(Y)==typeid(double))
		return mlMath_gaussianFit((const double*)x,(const double*)y,dataLen);
	std::vector<double> vx, vy;
	for (size_t i=0; i<dataLen; i++){vx.push_back(x[i]);vy.push_back(y[i]);}
	return mlMath_gaussianFit(vx.data(),vy.data(),dataLen);}
template <typename X,typename Y> ML_API	t_mlMathGaussianFit mlMath_gaussianFit(const std::vector<X>& x, const std::vector<Y>& y){
	if (typeid(X)==typeid(double)&&typeid(Y)==typeid(double))
		return mlMath_gaussianFit((const double*)x.data(),(const double*)y.data(),x.size());
	std::vector<double> vx(x.begin(),x.end()), vy(y.begin(),y.end());
	return mlMath_gaussianFit(vx.data(),vy.data(),vx.size());}
#endif

/*--------------------------------------------------------------------------------------------------
 *	filtering - 1D
 */
//	window
ML_API	t_mlError mlMath_filter_window_1d(const double* data, double* out, size_t len, int wnd_sz, double weight_factor=1);
template <typename T> ML_API	std::vector<T>	mlMath_filter_window_1d(const std::vector<T>& data, int wnd_sz, double weight_factor=1){
	std::vector<T> rtn;
	t_mlError err = kMLErr_OK;
	do {
		CHECK_CONDITION(NULL, data.size()>1, kMLErr_invalidDataSize);
		std::vector<double> d(data.begin(),data.end()),o;
		o.resize(data.size());
		err = mlMath_filter_window_1d(d.data(), o.data(), o.size(), wnd_sz);
		CHECK_ERROR(NULL, err);
		for (int i=0;i<data.size();i++) rtn.push_back((typeid(T)==typeid(double)||typeid(T)==typeid(float))?o[i]:(T)round(o[i]));
	} while(0);
	return rtn;
}
//	linear
ML_API	t_mlError mlMath_filter_linear_1d(const double* data, double* out, size_t len, int wnd_sz);
template <typename T> ML_API	std::vector<T>	mlMath_filter_linear_1d(const std::vector<T>& data, int wnd_sz){
	std::vector<T> rtn;
	t_mlError err = kMLErr_OK;
	do {
		CHECK_CONDITION(NULL, data.size()>1, kMLErr_invalidDataSize);
		std::vector<double> d(data.begin(),data.end()),o;
		o.resize(data.size());
		err = mlMath_filter_linear_1d(d.data(), o.data(), o.size(), wnd_sz);
		CHECK_ERROR(NULL, err);
		for (int i=0;i<data.size();i++) rtn.push_back((typeid(T)==typeid(double)||typeid(T)==typeid(float))?o[i]:(T)round(o[i]));
	} while(0);
	return rtn;
}
#if defined(__APPLE__)
//	2nd order
ML_API	t_mlError mlMath_filter_2nd_order_1d(const double* data, double* out, size_t len, int wnd_sz);
template <typename T> ML_API	std::vector<T>	mlMath_filter_2nd_order_1d(const std::vector<T>& data, int wnd_sz){
	std::vector<T> rtn;
	t_mlError err = kMLErr_OK;
	do {
		CHECK_CONDITION(NULL, data.size()>1, kMLErr_invalidDataSize);
		std::vector<double> d(data.begin(),data.end()),o;
		o.resize(data.size());
		err = mlMath_filter_2nd_order_1d(d.data(), o.data(), o.size(), wnd_sz);
		CHECK_ERROR(NULL, err);
		for (int i=0;i<data.size();i++) rtn.push_back((typeid(T)==typeid(double)||typeid(T)==typeid(float))?o[i]:(T)round(o[i]));
	} while(0);
	return rtn;
}
#endif//__APPLE__
#	pragma mark -
#	pragma mark t_mlData_1d
/*--------------------------------------------------------------------------------------------------
 *	t_mlData_1d - test 1d data
 */
#include <vector>

template <typename T>
struct t_mlData_1d:std::vector<T> {
	//
	t_mlData_1d(){(*this).clear();(*this).shrink_to_fit();};
	bool		isValid(){return (*this).size()>0;};
	bool		sameSize(const t_mlData_1d& dat){return dat.size()==(*this).size();}
	
	//	load
	template <typename D>
	t_mlData_1d<T>& load(const D* d, size_t arr_sz){
		(*this).clear();
		for (int i=0;i<arr_sz;i++){
			double val = round(d[i]);
			(*this).push_back((typeid(T)==typeid(D))?d[i]:
							  (typeid(T)==typeid(uint8_t ))?(T)CLIP(0x00		, val,	0xFF	):
							  (typeid(T)==typeid(uint16_t))?(T)CLIP(0x00		, val,	0xFFFF	):
							  (typeid(T)==typeid(uint32_t))?(T)CLIP(0x00		, val,	0xFFFF	):
							  (typeid(T)==typeid(int8_t  ))?(T)CLIP(-0x80		, val,	0x7F	):
							  (typeid(T)==typeid(int16_t ))?(T)CLIP(-0x8000		, val,	0x7FFF	):
							  (typeid(T)==typeid(int32_t ))?(T)CLIP(-0x80000000	, val,	0x7FFF	):
							  d[i]);
		}
		return *this;
	}
	template <typename D>
	t_mlData_1d<T>& operator= (const std::vector<D>& d){
		if (typeid(T)==typeid(D) || typeid(T)==typeid(float) || typeid(T)==typeid(double)){
			(*this).clear();(*this).assign(d.begin(),d.end());
		} else
			load(d.data(), d.size());
		return *this;
	}
	
	//	shift, scaling
	t_mlData_1d<T>&	shift (double a){for(int i=0;i<(*this).size();i++)(*this)[i]+=a;return *this;};
	t_mlData_1d<T>&	scale (double a){for(int i=0;i<(*this).size();i++)(*this)[i]*=a;return *this;};
	t_mlData_1d<T>&	operator +=	(double a){return shift(a);};
	t_mlData_1d<T>	operator +	(double a){t_mlData_1d rtn = *this; return rtn+=a;};
	t_mlData_1d<T>&	operator -=	(double a){return shift(-a);};
	t_mlData_1d<T>	operator -	(double a){t_mlData_1d rtn = *this; return rtn-=a;};
	t_mlData_1d<T>&	operator *=	(double a){return scale(a);};
	t_mlData_1d<T>	operator *	(double a){t_mlData_1d rtn = *this; return rtn*=a;};
	t_mlData_1d<T>&	operator /=	(double a){return scale(1.0/a);};
	t_mlData_1d<T>	operator /	(double a){t_mlData_1d rtn = *this; return rtn*=1.0f/a;};

	//	add, subtraction, multiplication, division
	//	!! W,H,data size should be same !!
	//	!!   ASSERTION FAIL OTHERWISE   !!
	t_mlData_1d<T>&	add (const t_mlData_1d& dat){assert(sameSize(dat));for(int i=0;i<(*this).size();i++)(*this)[i]+=dat[i];return *this;};
	t_mlData_1d<T>&	mlt (const t_mlData_1d& dat){assert(sameSize(dat));for(int i=0;i<(*this).size();i++)(*this)[i]*=dat[i];return *this;};
	t_mlData_1d<T>&	operator +=	(const t_mlData_1d& dat){return add(dat);};
	t_mlData_1d<T>	operator +	(const t_mlData_1d& dat){t_mlData_1d rtn = *this; return rtn+=dat;};
	t_mlData_1d<T>&	operator -=	(const t_mlData_1d& dat){return add(-dat);};
	t_mlData_1d<T>	operator -	(const t_mlData_1d& dat){t_mlData_1d rtn = *this; return rtn-=dat;};
	t_mlData_1d<T>&	operator *=	(const t_mlData_1d& dat){return mlt(dat);};
	t_mlData_1d<T>	operator *	(const t_mlData_1d& dat){t_mlData_1d rtn = *this; return rtn*=dat;};
	t_mlData_1d<T>&	operator /=	(const t_mlData_1d& dat){return mlt(1.0f/dat);};
	t_mlData_1d<T>	operator /	(const t_mlData_1d& dat){t_mlData_1d rtn = *this; return rtn/=dat;};

	//	stat
	double			sumVal(){return mlMath_sum((*this));};
	double			maxVal(){return mlMath_max((*this));};
	double			minVal(){return mlMath_min((*this));};
	double			avgVal(){return mlMath_avg((*this));};
	double			stdevVal(){return mlMath_stdev((*this));};

	//	filtering
	std::vector<double>	filter_2nd_order(int wnd_sz){std::vector<double> rtn = mlMath_filter_2nd_order_1d((*this), wnd_sz);return rtn;};
	std::vector<double>	filter_linear(int wnd_sz){std::vector<double> rtn = mlMath_filter_linear_1d((*this), wnd_sz);return rtn;};
	std::vector<double>	filter_window(int wnd_sz, float weight_factor){std::vector<double> rtn = mlMath_filter_window_1d((*this), wnd_sz, weight_factor);return rtn;};
};

#	pragma mark -
#	pragma mark t_mlData_2d
/*--------------------------------------------------------------------------------------------------
 *	t_mlData_2d - test 2d data
 */

template <typename T>
struct t_mlData_2d:std::vector<T> {
	size_t w,h;
	//
	t_mlData_2d():w(0),h(0){(*this).clear();};
	bool		isValid(){return w>0 && h>0 && (*this).size()>0 && w*h==(*this).size();};
	bool		isSameSize(const t_mlData_2d& img){return img.w==w && img.h==h && img.size()==(*this).size();}
	
	//	load
	template <typename D>
	void		load(size_t _w, size_t _h, const D* d, size_t d_arr_sz){
		w=_w;h=_h;(*this).clear();
		for (int i=0;i<w*h;i++){
			double val = round(d[i]);
			(*this).push_back((typeid(T)==typeid(D))?d[i]:
							  (typeid(T)==typeid(uint8_t ))?(T)CLIP(0x00		, val,	0xFF	):
							  (typeid(T)==typeid(uint16_t))?(T)CLIP(0x00		, val,	0xFFFF	):
							  (typeid(T)==typeid(uint32_t))?(T)CLIP(0x00		, val,	0xFFFF	):
							  (typeid(T)==typeid(int8_t  ))?(T)CLIP(-0x80		, val,	0x7F	):
							  (typeid(T)==typeid(int16_t ))?(T)CLIP(-0x8000		, val,	0x7FFF	):
							  (typeid(T)==typeid(int32_t ))?(T)CLIP(-0x80000000	, val,	0x7FFF	):
							  d[i]);
		}
	}
	template <typename D>
	t_mlData_2d<T>& operator= (const t_mlData_2d<D>& d){
		if (typeid(T)==typeid(D) || typeid(T)==typeid(float) || typeid(T)==typeid(double)){
			w=d.w;h=d.h;(*this).clear();(*this).assign(d.begin(),d.end());
		} else {
			load(d.w, d.h, d.data(), d.size());
		}
		return *this;
	}

	t_mlData_2d<T>&	flip_vert(){
		t_mlData_2d<T> rtn;
		size_t line_buff_sz = w*sizeof(T); T* buff = (T*)malloc(line_buff_sz); assert(buff);
		T* data = (*this).data();
		for (int r=0;r<(h/2);r++){memcpy(buff,data+r*w,line_buff_sz);memcpy(data+r*w,data+(h-1-r)*w,line_buff_sz);memcpy(data+(h-1-r)*w,buff,line_buff_sz);}
		return *this;
	}
	t_mlData_2d<T>&	flip_hori(){
		if (!isValid()) return *this;
		T* data = (*this).data();
		for (int r=0;r<h;r++) for (int c=0;c<w/2;c++) {T tmp = data[r*w+c]; data[r*w+c]=data[r*w+w-1-c]; data[r*w+w-1-c]=tmp;}
		return *this;
	}
	t_mlData_2d<T>&	crop(size_t x, size_t y, size_t n_w, size_t n_h){
		if (!isValid() || x<0 || y<0 || n_w<1 || n_h<1 || w<x || h<y || w<(x+n_w) || h<(y+n_h)) return *this;
		T* dst = (*this).data();
		for (size_t r=y;r<(y+n_h);r++) for (size_t c=x;c<(x+n_w);c++){*dst=(*this)[r*w+c];dst++;};
		w = n_w; h = n_h; (*this).resize(w*h);
		return *this;
	}
	t_mlData_2d<T>&	scale(size_t new_w, size_t new_h){
		t_mlData_2d<T> buff;
		if (!isValid())	return *this;
		buff.w=new_w; buff.h=new_h; buff.resize(new_w*new_h);
		double ratio_vert = h/(double)new_h;
		double ratio_hori = w/(double)new_w;
		T* src = (*this).data();
		T* dst = buff.data();
		for (size_t rowIdx=0;rowIdx<new_h;rowIdx++)
			for (size_t colIdx=0;colIdx<new_w;colIdx++){
				size_t bgn_x = MAX(round(colIdx*ratio_hori-ratio_hori/2)	,0	);
				size_t end_x = MIN(round(colIdx*ratio_hori+ratio_hori/2+1)	,w-1);
				size_t bgn_y = MAX(round(rowIdx*ratio_vert-ratio_vert/2)	,0	);
				size_t end_y = MIN(round(rowIdx*ratio_vert+ratio_vert/2+1)	,h-1);
				double sum = 0; int cnt = 0;
				for (size_t y=bgn_y;y<=end_y;y++) for (size_t x=bgn_x;x<=end_x;x++) {sum+=src[y*w+x];cnt++;}
				
				double val = round(sum/cnt);
				dst[rowIdx*buff.w+colIdx] =((typeid(T)==typeid(float ) || typeid(T)==typeid(double))?sum/cnt:
											(typeid(T)==typeid(uint8_t ))?(T)CLIP(0x00		 , val,	0xFF	):
											(typeid(T)==typeid(uint16_t))?(T)CLIP(0x00		 , val,	0xFFFF	):
											(typeid(T)==typeid(uint32_t))?(T)CLIP(0x00		 , val,	0xFFFF	):
											(typeid(T)==typeid(int8_t  ))?(T)CLIP(-0x80		 , val,	0x7F	):
											(typeid(T)==typeid(int16_t ))?(T)CLIP(-0x8000	 , val,	0x7FFF	):
											(typeid(T)==typeid(int32_t ))?(T)CLIP(-0x80000000, val,	0x7FFF	):
											sum/cnt);
			}
		w=new_w;h=new_h;memcpy((*this).data(),buff.data(),w*h*sizeof(T));
		(*this).resize(w*h);
		return *this;
	}
	
	//	shift, scaling
	t_mlData_2d<T>&	shift (double a){for(int i=0;i<(*this).size();i++)(*this)[i]+=a;return *this;};
	t_mlData_2d<T>&	scale (double a){for(int i=0;i<(*this).size();i++)(*this)[i]*=a;return *this;};
	t_mlData_2d<T>&	operator +=	(double a){return shift(a);};
	t_mlData_2d<T>	operator +	(double a){t_mlData_2d<T> rtn = *this; return rtn+=a;};
	t_mlData_2d<T>&	operator -=	(double a){return shift(-a);};
	t_mlData_2d<T>	operator -	(double a){t_mlData_2d<T> rtn = *this; return rtn-=a;};
	t_mlData_2d<T>&	operator *=	(double a){return scale(a);};
	t_mlData_2d<T>	operator *	(double a){t_mlData_2d<T> rtn = *this; return rtn*=a;};
	t_mlData_2d<T>&	operator /=	(double a){return scale(1.0f/a);};
	t_mlData_2d<T>	operator /	(double a){t_mlData_2d<T> rtn = *this; return rtn/=a;};

	//	add, subtraction, multiplication, division
	//	!! W,H,data size should be same !!
	//	!!   ASSERTION FAIL OTHERWISE   !!
	t_mlData_2d<T>&	add (const t_mlData_2d<T>& img){assert(isSameSize(img));for(int i=0;i<(*this).size();i++)(*this)[i]+=img[i];return *this;};
	t_mlData_2d<T>&	sub (const t_mlData_2d<T>& img){assert(isSameSize(img));for(int i=0;i<(*this).size();i++)(*this)[i]-=img[i];return *this;};
	t_mlData_2d<T>&	mlt (const t_mlData_2d<T>& img){assert(isSameSize(img));for(int i=0;i<(*this).size();i++)(*this)[i]*=img[i];return *this;};
	t_mlData_2d<T>&	div (const t_mlData_2d<T>& img){assert(isSameSize(img));for(int i=0;i<(*this).size();i++)(*this)[i]/=img[i];return *this;};
	t_mlData_2d<T>&	operator +=	(const t_mlData_2d<T>& img){return add(img);};
	t_mlData_2d<T>	operator +	(const t_mlData_2d<T>& img){t_mlData_2d<T> rtn = *this; return rtn+=img;};
	t_mlData_2d<T>&	operator -=	(const t_mlData_2d<T>& img){return sub(img);};
	t_mlData_2d<T>	operator -	(const t_mlData_2d<T>& img){t_mlData_2d<T> rtn = *this; return rtn-=img;};
	t_mlData_2d<T>&	operator *=	(const t_mlData_2d<T>& img){return mlt(img);};
	t_mlData_2d<T>	operator *	(const t_mlData_2d<T>& img){t_mlData_2d<T> rtn = *this; return rtn*=img;};
	t_mlData_2d<T>&	operator /=	(const t_mlData_2d<T>& img){return div(img);};
	t_mlData_2d<T>	operator /	(const t_mlData_2d<T>& img){t_mlData_2d<T> rtn = *this; return rtn/=img;};

	//	stat
	double			sumVal(){return mlMath_sum((*this));};
	double			maxVal(){return mlMath_max((*this));};
	double			minVal(){return mlMath_min((*this));};
	double			avgVal(){return mlMath_avg((*this));};
	double			stdevVal(){return mlMath_stdev((*this));};
	
	//	filtering
	t_mlData_2d<T>&	filter_window(int wnd_sz, float weight_factor){
		std::vector<double> buff;
		if (!isValid() || wnd_sz<1 || weight_factor<0 || 1<weight_factor) return *this;
		buff.resize(w*h);
		T* src = (*this).data();
		T* dst = buff.data();
		for (size_t rowIdx=0;rowIdx<h;rowIdx++)
			for (size_t colIdx=0;colIdx<w;colIdx++){
				double dist_max = sqrt(pow(wnd_sz/2,2)+pow(wnd_sz/2,2));
				double val = src[rowIdx*w+colIdx];
				if (_IS_VALID_NUMBER_(val)){
					size_t bgn_x = MAX(round(colIdx-wnd_sz/2)	,0	);
					size_t end_x = MIN(round(colIdx+wnd_sz/2+1)	,w-1);
					size_t bgn_y = MAX(round(rowIdx-wnd_sz/2)	,0	);
					size_t end_y = MIN(round(rowIdx+wnd_sz/2+1)	,h-1);
					float total = 0;
					float t_weight = 0;
					for (size_t x=bgn_x;x<=end_x;x++)
						for (size_t y=bgn_y;y<=end_y;y++){
							double v = src[y*w+x];
							if (_IS_VALID_NUMBER_(v)){
								float weight = 1-sqrt(pow(x-colIdx,2)+pow(y-rowIdx,2))/dist_max*(1-weight_factor);
								total+=v*weight;
								t_weight+=weight;
							}
						}
					val = round(total/t_weight);
					dst[rowIdx*w+colIdx] = ((typeid(T)==typeid(float ) || typeid(T)==typeid(double))?total/t_weight:
											(typeid(T)==typeid(uint8_t ))?(T)CLIP(0x00		 , val,	0xFF	):
											(typeid(T)==typeid(uint16_t))?(T)CLIP(0x00		 , val,	0xFFFF	):
											(typeid(T)==typeid(uint32_t))?(T)CLIP(0x00		 , val,	0xFFFF	):
											(typeid(T)==typeid(int8_t  ))?(T)CLIP(-0x80		 , val,	0x7F	):
											(typeid(T)==typeid(int16_t ))?(T)CLIP(-0x8000	 , val,	0x7FFF	):
											(typeid(T)==typeid(int32_t ))?(T)CLIP(-0x80000000, val,	0x7FFF	):
											total/t_weight);
				} else {
					dst[rowIdx*w+colIdx]=NAN;
				}
			}
		memcpy(src,dst,sizeof(T)*buff.size());
		return *this;
	};
};

#endif//ifndef _ML_MATH_H_
