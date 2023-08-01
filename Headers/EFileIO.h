//**************************************************************************
//:>
//  Copyright © 1999-2008 Axeda Corporation.  All rights reserved.
//:>
//**************************************************************************
//:>
//:> Filename   :  EFileIO.h
//:>  
//:> Subsystem  :  Kernel/Base
//:>
//:> Description:  Portable classes for Files
//:>
//:> Classes    :  EFileIO
//:>
//**************************************************************************
#ifndef __EFILEIO_H__
#define __EFILEIO_H__       

// group=File IO

#ifdef WIN32
    #ifdef _DV_EXPORT
       #define _DV_DECLSPEC __declspec(dllexport)
       #define _DV_EXTERN
    #else
       #define _DV_DECLSPEC __declspec(dllimport)
       #define _DV_EXTERN   extern
    #endif
#else
   #define _DV_DECLSPEC
   #define _DV_EXTERN
#endif

#include <sys/stat.h>

#ifdef WIN32
#include <io.h>
#endif

// group=File Defines

// Opens a file only for reading.
#define UOPEN_READ  0x00000002

// Opens a file only for writing.
#define UOPEN_WRTE  0x00000004

// Opens a file for reading and writing.
#define UOPEN_RDWT  (UOPEN_READ | UOPEN_WRTE)

// Opens a file only if it exists.
#define UOPEN_EXST  0x00000008

// Creates a file if one does not exist.
#define UOPEN_CRTE  0x00000010

// Opens for BINARY I/O (no CR-LF expansion).
#define UOPEN_RAW   0x00000020

// Always open and trancate
#define UOPEN_TRUN  0x00000040

// Opens with sharing for read
#define UOPEN_SHRD  0x00000080

// Opens with sharing for write
#define UOPEN_SHWR  0x00000100

// Assign restricted access permissions when creating file
#define UOPEN_RESTRICTED  0x00000200

// File Handle Definition.

#ifdef WIN32 
  #define EFILE_HANDLE  HANDLE
#else
  #define EFILE_HANDLE  int
#endif

// _wfinddata_t is used directly in EFileIO::Find*() function signatures.
// In Visual C++ 6.0, _wfinddata_t was declared as a struct, whereas in newer
// Visual C++ versions, it is a preprocessor definition that maps either to
// _wfinddata32_t or _wfinddata64i32_t, depending on whether _USE_32BIT_TIME_T
// is defined. In order to provide binary compatibility with Agent extensions
// that were built using Visual C++ 6.0 and use EFileIO::Find*(),
// struct _wfinddata_t is redefined below.
#if defined(WIN32)
typedef _wfinddata_t __native__wfinddata_t;
#if defined(_USE_32BIT_TIME_T) && defined(_wfinddata_t)
#undef _wfinddata_t
struct _wfinddata_t {
        unsigned    attrib;
        __time32_t  time_create;    /* -1 for FAT file systems */
        __time32_t  time_access;    /* -1 for FAT file systems */
        __time32_t  time_write;
        _fsize_t    size;
        wchar_t     name[260];
};
#endif
#endif

// File attributes
#define EFILE_ATTR_NONE             0
#define EFILE_ATTR_FILE             1
#define EFILE_ATTR_DIRECTORY        2

// write mode used to write a unicode string: AS IS, UTF8ENCODE, TRANCATE TO 1 BYTE
enum EFILE_WRITE_MODE {EFILE_WRITE_UNICODE, EFILE_WRITE_UTF8ENCODE, EFILE_WRITE_ASCII};

// group=File IO

// Forward Reference
class EByteArray;

/**************************************************************************
EFileIO is a portable class for doing file operations. The constructor
takes a file name and automatically opens that file. The functions that
get the file size and time will fail if IsOpen() is not True. If IsOpen() 
is not True, it's because the file does not exist or cannot be opened. 

  
The static function DeleteFile() can be called without an object, as follows:

bool ok = EFileIO::DeleteFile(_T("filename"));
**************************************************************************/
class _DV_DECLSPEC EFileIO
{
public:
    typedef enum { FromStart, FromCurrent, FromEnd } FilePositionStart;

public:
    // Constructor opens the named file. Use IsOpen() to check if the file
    // opened successfully.
    EFileIO(const TCHAR* lpFileName,EULONG32 ulFlags = 0);

    // Closes the file if needed.
    ~EFileIO();
    
    void CloseFile();        // done automatically in destructor.

    // Tests if the file exists. Returns True if path exists.
    static bool Exists(TCHAR* path, DWORD* pAttrib = NULL);

    // Tests if the file opened successfully. Returns True if the file is open.
    bool  IsOpen();
    
    // Returns True if the file read is at EOF.
    bool  IsEOF() {return m_EOF;}

    // Returns the file size if the file is open; otherwise, returns 0.
    DWORD GetFileSize();

    // Gives name of file
    EString& GetFileName();
    
    // Returns the time of the last write. 
    ETime& GetWriteTime();

    // Returns t/f for success code.  
    // if true; pWriteTime contains the time of the last file write. 
    static bool GetWriteTime(const TCHAR*, ETime* pWriteTime);
    
    // Deletes the named file. This static function can be used without
    // creating an EFileIO object or opening the file.
    static bool DeleteFile(const TCHAR* pszFileName, bool bForceDelete = false);   

    // Returns a handle to opened file.
    EFILE_HANDLE  GetFileHandle() {return m_file;} 

    // Creates a directory.
    static bool CreateDir(TCHAR *lpDirName);

    // Removes directory (must be empty)
    static bool RemoveDir(TCHAR* lpDirName);

    // Recursivly creates directories
    static bool CreateRecursiveDir(TCHAR* pPath);

    // Retrieves current directory
    static bool GetCurrentDir(EString* pesDirName);

    // Changes current directory
    static bool ChangeDir(TCHAR* lpDirName);

    // strips non-alpha numeric characters from string (in-place) and re-adjusts
    // size of EString.
    static void StripNonAlphaNumeric(EString* pesDirectory);

    // return t/f if sub directory (folder name) is valid.  Thus, the name can be used
    // in CreateDir w/o error.
    static bool IsValidSubDirectory(TCHAR* pszSubDir);

    // pesSubDir is modified as necessary to be a valid sub directory name.
    static void GetValidSubDirectory(/* in/ out */ EString* pesSubDir);

    // Writes data to a buffer.
    bool WriteToFile(const void* lpBuffer,DWORD sizeToWrite,DWORD& sizeWritten);

    // Writes a TCHAR* to file
    bool WriteToFile(TCHAR* pBuffer, DWORD& sizeWritten, EFILE_WRITE_MODE mode = EFILE_WRITE_UNICODE);

    // Reads data from a file.
    bool ReadFrmFile(void* lpBuffer,DWORD sizeToRead,DWORD& sizeRead);

    // Positions a file.
    long PositionFile(long toWhere, FilePositionStart fromWhere);

    // Deletes. 
    bool Delete();

    //  Calculate MD5 for entire file.  Return true if checksum is valid.
    //  Output rByteArray contains the checksum series of bytes.
    bool MD5Checksum(EByteArray& rByteArray);

    // Set the file size.  Useful for making the file shorter then it current length.
    // Return true if successful.
    // static
    static bool SetFileSize(TCHAR* pszFileName, long fileSize);

    // Change name of file pszOldName to pszNewName.
    // Important: won't work on an open file, or if file pszNewName exists
    static bool RenameFile(TCHAR* pszOldName, TCHAR* pszNewName);

    // move file pszOldName to pszNewName.  Allows local moves and move to new directory.
    // Important: won't work on an open file, or if file pszNewName exists
    // true --> successful.
    static bool MoveFile(TCHAR* pszOldName, TCHAR* pszNewName);

    // CopyFile() copy a file.
    // return true if file is copied
    static bool CopyFile(TCHAR* pszOldName, TCHAR* pszNewName, bool bFailIfExists);

    // Change mode function.  Sets the file/directory read/write attributes.
    // pmode is to set the "permissions"
    static bool Chmod(TCHAR* pszFileName, int pMode);

    // converts file path to local conventions
    static void ConvertPathString(EString* inOutPath);
    static void ConvertPathString(TCHAR* inOutPath);

    // create fully qualified file name from O.S. root. 
    // (Win32 example:  C:\Axeda\Gateway\file.txt).
    // If the name starts with "\" or "x:" --> no change.
    // The Agent root is assumed to be the current working directory.
    // ASSUMES filename has already been through ConverPathString()
    static void CreateFullyQualifiedName(EString& resFilename);

// TCN -- matches closely to _wfindfirst,_wfindnext and _findclose:
// long _wfindfirst( wchar_t *filespec, struct _wfinddata_t *fileinfo );
// int _wfindnext( long handle, struct _wfinddata_t *fileinfo );
// int _findclose( long handle );
    static LONG_PTR FindFirst( wchar_t *filespec, struct _wfinddata_t *fileinfo );
    static int FindNext( LONG_PTR handle, struct _wfinddata_t *fileinfo );
    static int FindClose( LONG_PTR handle );
#ifdef UNIX
  static bool MatchPattern(char* pString, char* pPattern);
#endif

    // Removes directory and all its contents
    static bool RemoveDirAndContents(TCHAR* lpDirName);

    static EString GetDirEntryName(TCHAR *pszFileName);
    static EString GetDirName(TCHAR *pszFileName);

    // non-static version of SetFileSize()
    bool SetFileSize(long fileSize);

    // Methods supporting 64-bit file pointer
    LONGLONG GetFileSize64(void);
    static LONGLONG GetFileSize64(TCHAR* pszFileName);
    LONGLONG PositionFile64(LONGLONG toWhere, FilePositionStart fromWhere);
    bool SetFileSize64(LONGLONG fileSize);
    static bool SetFileSize64(TCHAR* pszFileName, LONGLONG fileSize);

    // Returns file path that is mapped using the home directory and/or
    // explicit file path mappings, if any. The result is converted to
    // absolute path, if requested.
    static EString GetMappedFileName(const TCHAR *pszFileName, bool bAbsolute = false);

    // Flushes file buffers to disk
    bool FlushBuffers(void);

    // Determines whether the given path is absolute.
    static bool IsAbsolutePath(const TCHAR *pszPath);

    static EString PrefixPath(const TCHAR *pszPath, const TCHAR *pszPrefix);

    // Sets the home directory.
    static void SetHomeDirectory(const TCHAR *pszHomeDirectory);

    // Returns the home directory.
    static EString& GetHomeDirectory(void);

    // Initializes file path mapping.
    static bool InitFilePathMapping(void);

    // Tests if the file exists without path mapping.
    static bool ExistsUnmapped(TCHAR* path, DWORD* pAttrib = NULL);

#ifdef WIN32
    // Adds the drive letter to an absolute path, if it is missing.
    static EString NormalizeAbsolutePath(const TCHAR *pszPath);
#endif

#ifdef UNDER_CE
    static void InitCurrentDir(HMODULE hModule);
#endif

private:
    bool         m_EOF;
    ETime        m_WriteTime;
    EString      strName;
    EFILE_HANDLE m_file; 

    static EString m_esHomeDirectory;
};

#ifdef UNIX
class FIND_FILE_INFO
{
public:
    FIND_FILE_INFO(void);
    ~FIND_FILE_INFO();

    char *m_pFilePath;
    char *m_pFilePattern;
    DIR  *m_pDir;
};
#endif

#endif // __EFILEIO_H__
