//
//  ml_csv.h
//
//  Created by Joon Kwon on 10/2/14.
//  Copyright (c) 2014 Apple Inc. All rights reserved.
//

#ifndef _ML_CSV_H_
#define _ML_CSV_H_

#ifdef __ML_USE_AS_EMB__
#include <ml_typedef.h>
#include <ml_log.h>
#else
#include <ml_typedef.h>
#include <ml_log.h>
#endif

#if defined(__cplusplus)
#include <unordered_map>

/*	CML_csvWriter
 *	creates/write csv file
 *	procedure:
 *		. create the instance
 *		. [optional] set maximum data count (default 2048), through setEntryCntMax
 *		. [optional] add attributes
 *		. set serial number
 *		. add data
 *		. call 'write' function for actual file writting.
 *			csv file title and version string should be provided for the intiial file creation
 */
class CML_csvWriter: public CML_baseObject {
protected:
	uint32_t				_entryCntMax;
	const t_mlDataItem**	_pAttrItems;
	uint32_t				_attrCnt;
	const t_mlDataItem**	_pDataItems;
	uint32_t				_dataCnt;
public:
	CML_csvWriter();
	CML_csvWriter(CML_logManager* lm, int entryCountMax = 2048);
	~CML_csvWriter();
	t_mlError	setLogManager(CML_logManager* lm);
	t_mlError	setEntryCntMax(uint32_t entryCntMax);
	//
	void		reset();
	//
	t_mlError	addAttr(const t_mlDataItem* pDataItem);
	int			attrCount(){return INSTANCE_VALID?_attrCnt:0;};
	t_mlError	addData(const t_mlDataItem* pDataItem);
	int			dataCount(){return INSTANCE_VALID?_dataCnt:0;};
	//
	t_mlError	write(const char* const filePathName,
					  const char* strTitle,
					  const char* strVersion,
					  const char* serialNumber,
					  t_mlError testErrCode,
					  const char* testErrCodeStr,
					  const char* strTime_start,
					  const char* strTime_stop,
					  double timestamp,
					  double testTime);
};

#include <vector>
#include <string>
/*	CML_csvParser
 *	parse csv file
 */
class CML_csvParser: public CML_baseObject {
protected:
	std::string			_stationType;
	std::string			_swVersion;
	//
	std::string								_fileBuffer;
	std::vector<std::string>				_titleRow;
	std::vector<std::string>				_lowerLimitRow;
	std::vector<std::string>				_upperLimitRow;
	std::vector<std::vector<std::string> >	_dataRow;
public:
	CML_csvParser();
	CML_csvParser(CML_logManager* lm);
	~CML_csvParser();
	t_mlError	setLogManager(CML_logManager* lm);
public:
	t_mlError		load(const char* csvFilePath, t_mlVbsLvl vbsLvl = kVL2);
	int				getDataCol();
	int				getDataRow();
	int				findColIdx(const char* title);
	double			getData(int dataSetIdx, int dataItemIdx);
	std::string		getStr(int dataSetIdx, int dataItemIdx);
};

/***************************************************************************************************
 *	CML_csv
 *
 */
#pragma mark -
#pragma mark CML_csv

// anchors
typedef enum {
	k_mlCsv_type_normal = 0,
	k_mlCsv_type_test_limit,
	k_mlCsv_type_grr_config,
	k_mlCsv_type_test_limit_legacy,
	//
	k_mlCsv_type_MAX,
	k_mlCsv_type_unknown = -1,
} t_mlCsv_type;

static const char* k_mlCsv_anchors[k_mlCsv_type_MAX] = {
	"SerialNumber",	//	normal test result csv file
	"SpecItem",		//	test limit
	"GrrConfig",	//	grr config
	"spec_symbol",	//	test limit legacy, from Mesa and other old project
	//
};

// version info prefix
#define k_mlCsv_sw_version_info_prefix		"sw_version:"
#define k_mlCsv_spec_version_info_prefix	"spec_version:"

// header
#define k_mlCsv_header_marker		"->"	// anything that has "->" is regarded as the header
#define k_mlCsv_marker_limit_upper_legacy	"upper_limit"
#define k_mlCsv_marker_limit_lower_legacy	"lower_limit"
#define k_mlCsv_marker_limit_upper	"Upper Limit"
#define k_mlCsv_marker_limit_lower	"Lower Limit"
#define k_mlCsv_marker_ers_upper	"Upper ERS"
#define k_mlCsv_marker_ers_lower	"Lower ERS"
#define k_mlCsv_marker_unit			"Unit"
#define k_mlCsv_marker_grr_stdev	"GRR Stdev"
#define k_mlCsv_marker_grr_limit	"GRR Limit"

// the list of the pre-defined non-parametric columns
static const char* k_mlCsv_columnToExclude[] = {
	"SerialNumber",
	"StartTime",
	"EndTime",
	"SwVersion",
	"Version",
	"TesterID",
	"Tester ID",
	"StationID",
	"Station ID",
	"Time Stamp",
	"TimeStamp",
};

// column information
struct t_mlCsvColInfo{
	std::string		title;
	bool		isParametric;
	//	data / spec csv
	double		limit_upper;
	double		limit_lower;
	double		ers_upper;
	double		ers_lower;
	//	grr config csv
	double		grr_stdev;
	double		grr_limit;
	//	common
	std::string		str_unit;
	//
	t_mlCsvColInfo():isParametric(false),
	limit_upper(_NA_ML_),limit_lower(_NA_ML_),
	ers_upper(_NA_ML_),ers_lower(_NA_ML_),grr_stdev(_NA_ML_),grr_limit(_NA_ML_){};
};

// column statistics
struct t_mlCsvColStat{
	std::vector<double>	data;
	double		maxVal;
	double		minVal;
	double		avg;
	double		stdev;
	double		cpl, cpu, cpk;
	double		yield;
	t_mlCsvColStat():maxVal(_NA_ML_),minVal(_NA_ML_),avg(_NA_ML_),stdev(_NA_ML_),cpl(_NA_ML_),cpu(_NA_ML_),cpk(_NA_ML_),yield(_NA_ML_){};
};

class ML_API CML_csv: public CML_baseObject {
protected:
	t_mlCsv_type			_type;
	std::string					_grp_name;	// group name
	//unordered_map<std::string, size_t>	_col_name;
	std::vector<t_mlCsvColInfo>	_col_info;
	std::vector<std::vector<std::string>>	_buff_str;
	std::vector<std::vector<double>>	_buff_val;
	std::vector<t_mlCsvColStat>	_col_stat;
	//
	std::string					_title;
	std::string					_sw_version;
	std::string					_spec_version;
	//
	void calculate_col_stat();
public:
	CML_csv():_type(k_mlCsv_type_unknown){ML_SET_CLASS_NAME};
	CML_csv(std::string fn, bool allowAnyFormat = false):CML_csv(){load(fn, allowAnyFormat);};
	virtual ~CML_csv(){};
	t_mlError setLogManager(CML_logManager* lm);
public:
	t_mlError		load(std::string fn, bool allowAnyFormat = false);
	const std::string&	sw_version()	{	return _sw_version;			};
	const std::string&	spec_version()	{	return _spec_version;		};
	t_mlCsv_type	type()			{	return _type;				};
	size_t			rows()			{	return _buff_str.size();	};
	size_t			cols()			{	return _col_info.size();	};
	CML_csv&		merge(const CML_csv& csv);
public:
	uint32_t		timeSort();
public:
	CML_csv&		operator +=	(const CML_csv& csv){return merge(csv);};
	CML_csv			operator +	(const CML_csv& csv){CML_csv rtn = *this; return rtn+=csv;};
public:
	std::vector<CML_csv> getGroups(std::string str_group_col_name);
	const std::string&	grp_name(){	return _grp_name;	};
public:
	const std::vector<t_mlCsvColInfo>&	col_info(){	return _col_info;	};
	const std::vector<std::vector<std::string>>&	buff_str(){	return _buff_str;	};
	const std::vector<std::vector<double>>&	buff_val(){	return _buff_val;	};
	const std::vector<t_mlCsvColStat>&	col_stat(){ calculate_col_stat(); return _col_stat;	};
	int				getColIdx(std::string colName);
	t_mlCsvColInfo	getColInfo(size_t rowIdx, std::string colName);
	std::string			getColStr(size_t rowIdx, std::string colName);
	double			getColVal(size_t rowIdx, std::string colName);
public:
	void			print(FILE* fp = stdout, const char delimiter='\t');
	void			exportCsv(std::string fileName);
};

#pragma mark -
#pragma mark Standalone CSV parser
DEPRECATED_F(typedef struct t_mlCsvFormat{
	size_t 								numOfRows;
	std::vector<size_t> 				rowStartPos;
	std::vector<size_t>					rowLen;
	std::vector<std::vector<size_t> > 	itemStartPos;
	std::vector<std::vector<size_t> > 	itemLen;
	t_mlCsvFormat(){clear();}
	void clear() {
		numOfRows = 0;
		std::vector<size_t>().swap(rowStartPos);
		std::vector<size_t>().swap(rowLen);
		std::vector<std::vector<size_t> >().swap(itemStartPos);
		std::vector<std::vector<size_t> >().swap(itemLen);
	}
} t_mlCsvFormat, "Slow. Use t_mlCsvDetail instead");

typedef class ML_API t_mlCsvDetail{
	size_t			_maxNumOfRows;
	size_t 			_numOfRows;
	size_t*			_rowStartPos;
	size_t*			_rowLen;
	size_t*			_rowItemSize;
	size_t**		_itemStartPos;
	size_t**		_itemLen;
public:
	t_mlCsvDetail();
	~t_mlCsvDetail();
public:
	void resize(size_t maxRows);
	void resizeItem(size_t rIdx, size_t maxItem);
	//
	void oneMoreRow();
	void setRowStartPos(size_t rIdx, size_t pos);
	void setRowLen(size_t rIdx, size_t len);
	void setItemStartPos(size_t rIdx, size_t iIdx, size_t pos);
	void setItemLen(size_t rIdx, size_t iIdx, size_t len);
	//
	const size_t numOfRows() const;
	const size_t rowStartPos(size_t rIdx) const;
	const size_t rowLen(size_t rIdx) const;
	const size_t rowItemSize(size_t rIdx) const;
	const size_t* itemStartPos(size_t rIdx) const;
	const size_t* itemLen(size_t rIdx) const;
	const size_t itemStartPos(size_t rIdx, size_t iIdx) const;
	const size_t itemLen(size_t rIdx, size_t iIdx) const;
} t_mlCsvDetail;

DEPRECATED_F(t_mlError mlCsv_decodeFile(CML_logManager* lm, const std::string& fnIn, std::string& fileBufOut, t_mlCsvFormat& fileFormat),
			 "Slow. Use mlCsv_getFileFormat instead");
DEPRECATED_F(t_mlError mlCsv_decodeData(CML_logManager* lm, const std::string& fileData, std::string& fileBufOut, t_mlCsvFormat& fileFormat),
			 "Slow. Use mlCsv_getDataFormat instead");
ML_API t_mlError mlCsv_getFileFormat(CML_logManager* lm, const std::string& fnIn,
									 t_mlCsvDetail& fileFormat);
ML_API t_mlError mlCsv_getDataFormat(CML_logManager* lm, const uint8_t* fileData, const size_t fileSize,
									 t_mlCsvDetail& fileFormat);

#pragma mark <> IML_categoryData
struct IML_categoryData {
	std::string name;
	//
	const t_mlCsvColInfo* pColInfo;
	const t_mlCsvColStat* pColStat;
	//
	int			decimalPoint;
	double		disp_max, disp_min;
	//
	bool		isSelected;
	double		r,g,b,a;
	double		bar_width, bar_left;
	int			bar_max;
	std::vector<uint32_t> bar;
	//
	IML_categoryData(){reset();};
	IML_categoryData(CML_csv* csv, int colIdx, const char* strName){reset();setColStat(csv,colIdx,strName);}
	virtual ~IML_categoryData(){};
	void reset();
	//
	void setColStat(CML_csv* csv, int colIdx, const char* strName);
	void calculateBound(bool includeLimits, int stdevAt);
	void prepHistogram(double barWidth, double totalAvg, double dispMin, double dispMax);
	//
	static IML_categoryData createCategoryData(CML_csv* pCsv, int colIdx, const char* strName=NULL);
};

#endif//defined(__cplusplus)
#endif//ifndef _ML_CSV_H_
