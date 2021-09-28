/**
 	@brief APIs for File Operation
 	@author Joon Kwon
 	@author Yida Zhang
 	@date 2018-07-02
 	@copyright 2018 Apple. All rights reserved.
 */

#ifndef _ML_FILE_H_
#define _ML_FILE_H_

#ifdef __ML_USE_AS_EMB__
#include <ml_utils.h>
#else
#include <ml_utils.h>
#endif

/** @addtogroup ml_file
 *  @{
 */

#if defined(__cplusplus)
extern "C" {
#endif
	
#pragma mark -
#pragma mark [FILE TOOLS]
	typedef struct {
		bool	isFile;		///< Flag if the entry is a file
		bool	isDirectory;///< Flag if the entry is a directory
		bool    isLink;		///< Flag if the entry is a symbolic link
		char	name[128];	///< Name string. The file name or directory name
	} t_mlDirEntryInfo;
	/** @struct t_mlDirEntryInfo
	 	@brief The structure of a directory entry
	 */
	
	/**
	 *	@brief	This method checks if the input path is a file
	 *	@param[in]	filePathName	the file path name
	 *
	 *	@return	@b TRUE if input path is a file
	 */
	ML_API	bool		mlFile_isFile(const char* filePathName);
	
	/**
	 *	@brief	This method checks if the input path exists in file system
	 *	@param[in]	filePathName	the file path name
	 *
	 *	@return	@b TRUE if input path exists
	 */
	ML_API	bool		mlFile_isFileExist(const char* filePathName);

	/**
	 *	@brief	This method checks if the input path is writable in file system
	 *	@param[in]	pathName	the file path name
	 *
	 *	@return	@b TRUE if input path writable
	 */
	ML_API	bool		mlFile_isPathWritable(const char* pathName);

	/**
	 *	@brief This method calculates the file size given a file descriptor
	 *	@param[in]	fp	the file descriptor
	 *
	 *	@return	size of the file in bytes
	 */
	ML_API	size_t		mlFile_getFileSize(FILE* fp);

    /**
     *    @brief This method calculate the total size of given path
     *    @param[in]    pathName    the file path name
     *
     *    @return    size of the path in bytes
     */
    ML_API    size_t        mlFile_getPathSize(const char* pathName);

	/**
	 *	@brief This method tries to delete input path
	 *	@param[in]	filePathName	the file path name
	 *
	 *	@warning Use system call to run "rm -f", no guarantee complete
	 *	@see mlFile_removeAll for reliable file removal
	 */
	ML_API	void		mlFile_delete(const char* filePathName);
	
	///	This method create the directories recursively
	/**
	 *	@param[in]	pathName	the path of directory to create
	 *
	 *	@return	MesaLibrary t_mlError defined in ml_error.h
	 */
	ML_API	t_mlError	mlFile_mkdir(const char* pathName);
	
	/**
	 *	@brief This method wraps up the standard C file open function
	 *	@n check if pathName is valid string or NOT
	 *	@n check if fileName is valid string or NOT
	 *	@n check if the mode if valid standard C file access mode
	 *	@param[in]	pathName	the path of the file to be opened
	 *	@param[in]	fileName	the name of the file to be opened
	 *	@param[in]	mode		the file access mode
	 *
	 *	@return	file descriptor, or @b NULL if any of the checks failed
	 */
	ML_API	FILE*		mlFile_openFile(const char* pathName, const char* fileName, const char* mode);
	
	/**
	 *	@brief This method allow partial file data reading given a valid file descriptor
	 *	@param[in]	fp			the file descriptor
	 *	@param[in]	offset		the offset from the beginning of the file, in bytes
	 *	@param[in]	len			the length of bytes to read out from the offset
	 *	@param[out]	out_buffer	the buffer to be filled in
	*
	 *	@warning @b ASSERT will be trigger if fp or out_buffer is NULL. Use mlFile_openFile
	 *				to open the file properly and allocate enough buffer
	 *	@warning The caller should FREE out_buffer after use
	 */
	ML_API	void		mlFile_readFileFrom(FILE* fp, size_t offset, size_t len, uint8_t *out_buffer);

	/**
	 *	@brief	This method loads the complete file data to a data pointer of memory
	 *
	 *	@param[in]	filePathName	the file path name
	 *	@param[out]	out_fileSize	[optional] if set, the file size will be returned
	 *
	 *	@return
	 *		The pointer to a buffer, the buffer size to be the same as
	 *		the result of mlFile_getFileSize function.
	 *		@n @b NULL if input file path not existed
	 *	@warning The caller SHOULD FREE the memory
	 */
	ML_API	uint8_t*	mlFile_readFile(const char* filePathName, size_t* out_fileSize);
	
	/**
	 *	@brief	This method parse the intel hex file format to binary data
	 *
	 *	based on the intel hex file format
	 *	@n http://en.wikipedia.org/wiki/Intel_HEX
	 *	@n http://microsym.com/editor/assets/intelhex.pdf
	 *
	 *	@param[in]	hexBuff			the buffer of the hex file
	 *	@param[out]	binBuff			the buffer to store to decoded binary data
	 *	@param[in,out] binBuffSz	[in] the size of the binBuff,
	 *								[out] real size of the decoded binary data
	 *
	 *	@return	MesaLibrary t_mlError defined in ml_error.h
	 *	@warning The caller SHOULD FREE the memory
	 */
	ML_API	t_mlError	mlFile_parseIntelHexFile(char* hexBuff, uint8_t* binBuff, size_t* binBuffSz);

#pragma mark -
#pragma mark [DIR TOOLS]
	/**
	 *	@brief	This method retrieves the number of entries in the direcotry
	 *	@param[in]	dirPathName	the dir path name
	 *	@return	The number of directory entry @n @b 0 if input path not valid, or path not exist
	 */
	ML_API	uint32_t	mlFile_getDirEntryCount(const char* dirPathName);
	
	/**
	 *	@brief	This method fills the 'entries' buffer with the directory entry info
	 *	@param[in]		dirPathName		the dir path name
	 *	@param[out]		pEntries		the pointer to the buffer of t_mlDirEntryInfo
	 *	@param[in,out]	pEntryCount		[in]	the length of the buffer size
	 *						[out]	the number of entry count
	 *	@return	MesaLibrary t_mlError defined in ml_error.h
	 *	@warning The caller pre-allocate memory for the t_mlDirEntryInfo
	 *	with size of directory entry count from mlFile_getDirEntryCount and FREE memory after use.
	 *
	 *	@see mlDir_getDirEntry for the memory management free alternative
	 */
	ML_API	t_mlError	mlFile_getDirEntry(const char* dirPathName, t_mlDirEntryInfo* pEntries, uint32_t* pEntryCount);
	
#if defined(__APPLE__)
	/**
	 *	@brief This method deletes all entries in the direcotry
	 *	NSFileManager based solution. Reliable file removal. Only on macOS
	 
	 *	@param[in]	filePath	the dir path name
	 *	@return	MesaLibrary t_mlError defined in ml_error.h
	 *
	 *	@see mlFile_delete for other platforms
	 */
	t_mlError	mlFile_removeAll(const char* filePath);
	/**
	 *	@brief This method move everything from srcPath to dstPath
	 *	NSFileManager based solution. Only on macOS
	 
	 *	@param[in]	dstPath		the destination path
	 *	@param[in]	srcPath		the source path
	 *	@return	MesaLibrary t_mlError defined in ml_error.h
	 */
	t_mlError	mlFile_moveAll(const char* dstPath, const char* srcPath);
	/**
	 *	@brief This method create a file based on the dataBuf and dataSz. If dataSz == 0. equivalent to touch
	 *	NSFileManager based solution. Only on macOS
	 
	 *	@param[in]	filePath	the path for the file to create
	 *	@param[in]	dataBuf		data buffer for the file
	 *	@param[in]	dataSz		data size for the file
	 *	@return	MesaLibrary t_mlError defined in ml_error.h
	 */
	t_mlError	mlFile_create(const char* filePath, const uint8_t* dataBuf, const int dataSz);
#endif
	
#if defined(__cplusplus)
}
#include <vector>
#include <string>
#include <fstream>
#pragma mark -
#pragma mark [FILE TOOLS, CPP]
/**
 *	@brief	This method copy the file from source to destination
 *	C++ implementation
 *
 *	@param[in]	srcFile		the file source to be copied
 *	@param[in]	desFile		full file path for the destination
 *	@return	MesaLibrary t_mlError defined in ml_error.h
 *
 *	@warning Only support file copy. No action if input path is a directory
 *	@warning The destination must be the complete file path, such as /dest/target.ext
 */
ML_API	t_mlError	mlFile_copyfile(std::string srcFile, std::string desFile);
/**
 *	@brief	This method checks if the input path exists in file system, C++ method
 *	@param[in]	fn	the file path name
 *
 *	@return	@b TRUE if input path exists
 */
ML_API	bool	mlFile_isFileExist(std::string fn);

/**
 *	@brief This method calculates the file size given file path
 *	C++ implementation
 *
 *	@param[in]	fn	the file path name
 *
 *	@return	size of the file in bytes, or @b 0 if file not existed
 */
ML_API	size_t	mlFile_getFileSize(std::string fn);

/**
 *	@brief	This method load the complete file buffer to a vector container
 *	C++ implementation
 *
 *	@param[in]	fn	the file path name
 *
 *	@return
 *		The file data stored in a vector container.
 *		A empty vector if input file path not existed
 */
ML_API	std::vector<uint8_t>	mlFile_readFile(std::string fn);
ML_API	void	mlFile_readFile(const std::string& fn, std::string& stringBuffer);

#pragma mark -
#pragma mark [DIR TOOLS, CPP]
/*--------------------------------------------------------------------------------------------------
 *	[DIR TOOLS, CPP]
 */

/// Enum types of directory entry
typedef enum {
	kDirEntryType_unknown = 0,
	kDirEntryType_file,
	kDirEntryType_directory,
	kDirEntryType_link,
	//
	kDirEntryType_MAX,
} t_mlDirEntryType;

static const char* kDirEntryTypeStr[] = {
	"unknown",
	" file  ",
	"  dir  ",
	" link  ",
};

struct IML_dirEntry{
	t_mlDirEntryType	type;		///< @brief Type, file or directory
	std::string			name;		///< @brief Name string. The file name or directory name
	long 				lastASec;	///< @ Last Access s
	long				lastAnSec;	///< @ Last Access nsec
	long				lastMSec;	///< @ Last Modify s
	long				lastMnSec;	///< @ Last Modify nsec
	long				lastCSec;	///< @ Last Change s
	long				lastCnSec;	///< @ Last Change nsec
	IML_dirEntry():type(kDirEntryType_unknown),
	lastASec(0),lastAnSec(0),lastMSec(0),lastMnSec(0),lastCSec(0),lastCnSec(0){};
	virtual ~IML_dirEntry(){};
};
/** \struct IML_dirEntry
 	@brief The structure of a directory entry, C++ implementation
 */

/**
 *	@brief	This method fills a vector container with the directory entry info
 *
 *	@param[in]		dirPath		the dir path name
 *	@param[in]		withStat	generate stat of each file
 *	@return	A vector container of directory entries with format of IML_dirEntry
 */
ML_API	std::vector<IML_dirEntry> mlDir_getDirEntry(std::string dirPath, bool withStat = true);
/**
 *	@brief	This method recursively walk the directory and retrieve all the file paths
 *
 *	@param[in]		dirPath		the directory path name
 *	@param[out]		fileList	the list of file path relative the input directory path.
 *								An empty list will be returned if the input path not exist.
 */
ML_API	void mlDir_walkDirEntry(std::string dirPath, std::vector<std::string>& fileList);

#endif

/** @} */

#endif//ifndef _ML_FILE_H_
