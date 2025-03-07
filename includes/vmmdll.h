// vmmdll.h : header file to include in projects that use vmm.dll / vmm.so
// 
// Windows may access both UTF-8 *U and Wide-Char *W versions of functions
// while Linux may only access UTF-8 versions. Some functionality may also
// be degraded or unavailable on Linux.
//
// (c) Ulf Frisk, 2018-2021
// Author: Ulf Frisk, pcileech@frizk.net
//
// Header Version: 4.3.3
//

#include "leechcore.h"

#ifndef __VMMDLL_H__
#define __VMMDLL_H__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef _WIN32

#include <Windows.h>
#undef EXPORTED_FUNCTION
#define EXPORTED_FUNCTION
typedef unsigned __int64                    QWORD, *PQWORD;

#endif /* _WIN32 */
#ifdef LINUX

#include <inttypes.h>
#include <stdlib.h>
#undef EXPORTED_FUNCTION
#define EXPORTED_FUNCTION                   __attribute__((visibility("default")))
typedef void                                VOID, *PVOID, *HANDLE, **PHANDLE, *HMODULE;
typedef long long unsigned int              QWORD, *PQWORD, ULONG64, *PULONG64;
typedef size_t                              SIZE_T, *PSIZE_T;
typedef uint64_t                            FILETIME, *PFILETIME;
typedef uint32_t                            DWORD, *PDWORD, *LPDWORD, BOOL, *PBOOL, NTSTATUS;
typedef uint16_t                            WORD, *PWORD;
typedef uint8_t                             BYTE, *PBYTE, *LPBYTE, UCHAR;
typedef char                                CHAR, *PCHAR, *LPSTR, *LPCSTR;
typedef uint16_t                            WCHAR, *PWCHAR, *LPWSTR, *LPCWSTR;
#define MAX_PATH                            260
#define _In_
#define _In_z_
#define _In_opt_
#define _In_reads_(x)
#define _In_reads_opt_(x)
#define _Inout_
#define _Inout_bytecount_(x)
#define _Inout_opt_
#define _Inout_updates_opt_(x)
#define _Out_
#define _Out_opt_
#define _Out_writes_(x)
#define _Out_writes_bytes_opt_(x)
#define _Out_writes_opt_(x)
#define _Out_writes_to_(x,y)
#define _When_(x,y)
#define _Frees_ptr_opt_
#define _Post_ptr_invalid_
#define _Check_return_opt_
#define _Printf_format_string_
#define _Success_(x)

#endif /* LINUX */



//-----------------------------------------------------------------------------
// INITIALIZATION FUNCTIONALITY BELOW:
// Choose one way of initializing the VMM / MemProcFS.
//-----------------------------------------------------------------------------

/*
* Initialize VMM.DLL with command line parameters. For a more detailed info
* about the parameters please see github wiki for MemProcFS and LeechCore.
* NB! LeechCore initialization parameters are _also_ valid to this function.
* Important parameters are:
*    -printf = show printf style outputs.
*    -v -vv -vvv = extra verbosity levels.
*    -device = device as on format for LeechCore - please see leechcore.h or
*              Github documentation for additional information. Some values
*              are: <file>, fpga, usb3380, hvsavedstate, totalmeltdown, pmem
*    -remote = remote LeechCore instance - please see leechcore.h or Github
*              documentation for additional information.
*    -norefresh = disable background refreshes (even if backing memory is
*              volatile memory).
*    -memmap = specify a physical memory map given by file or specify 'auto'.
*              example: -memmap c:\\temp\\my_custom_memory_map.txt
*              example: -memmap auto
*    -pagefile[0-9] = page file(s) to use in addition to physical memory.
*              Normally pagefile.sys have index 0 and swapfile.sys index 1.
*              Page files are in constant flux - do not use if time diff
*              between memory dump and page files are more than few minutes.
*              Example: 'pagefile0 swapfile.sys'
*    -symbolserverdisable = disable symbol server until user change.
*              This parameter will take precedence over registry settings.
*    -waitinitialize = Wait for initialization to complete before returning.
*              Normal use is that some initialization is done asynchronously
*              and may not be completed when initialization call is completed.
*              This includes virtual memory compression, registry and more.
*              Example: '-waitinitialize'
*    -userinteract = allow vmm.dll to, on the console, query the user for
*              information such as, but not limited to, leechcore device options.
*              Default: user interaction = disabled.
*    -forensic = start a forensic scan of the physical memory immediately after
*              startup if possible. Allowed parameter values range from 0-4.
*              Note! forensic mode is not available for live memory.
*              1 = forensic mode with in-memory sqlite database.
*              2 = forensic mode with temp sqlite database deleted upon exit.
*              3 = forensic mode with temp sqlite database remaining upon exit.
*              4 = forensic mode with static named sqlite database (vmm.sqlite3).
*              Example -forensic 4
*
* -- argc
* -- argv
* -- ppLcErrorInfo = optional pointer to receive a function allocated memory of
*              struct LC_CONFIG_ERRORINFO with extended error information upon
*              failure. Any memory received should be free'd by caller by
*              calling LcMemFree().
* -- return = success/fail
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_Initialize(_In_ DWORD argc, _In_ LPSTR argv[]);

EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_InitializeEx(_In_ DWORD argc, _In_ LPSTR argv[], _Out_opt_ PPLC_CONFIG_ERRORINFO ppLcErrorInfo);

/*
* Close an initialized instance of VMM.DLL and clean up all allocated resources
* including plugins, linked PCILeech.DLL and other memory resources.
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_Close();

/*
* Free memory allocated by the VMMDLL.
* -- pvMem
*/
EXPORTED_FUNCTION
VOID VMMDLL_MemFree(_Frees_ptr_opt_ PVOID pvMem);



//-----------------------------------------------------------------------------
// CONFIGURATION SETTINGS BELOW:
// Configure MemProcFS or the underlying memory
// acquisition devices.
//-----------------------------------------------------------------------------

/*
* Options used together with the functions: VMMDLL_ConfigGet & VMMDLL_ConfigSet
* Options are defined with either: VMMDLL_OPT_* in this header file or as
* LC_OPT_* in leechcore.h
* For more detailed information check the sources for individual device types.
*/
#define VMMDLL_OPT_CORE_PRINTF_ENABLE                   0x4000000100000000  // RW
#define VMMDLL_OPT_CORE_VERBOSE                         0x4000000200000000  // RW
#define VMMDLL_OPT_CORE_VERBOSE_EXTRA                   0x4000000300000000  // RW
#define VMMDLL_OPT_CORE_VERBOSE_EXTRA_TLP               0x4000000400000000  // RW
#define VMMDLL_OPT_CORE_MAX_NATIVE_ADDRESS              0x4000000800000000  // R

#define VMMDLL_OPT_CORE_SYSTEM                          0x2000000100000000  // R
#define VMMDLL_OPT_CORE_MEMORYMODEL                     0x2000000200000000  // R

#define VMMDLL_OPT_CONFIG_IS_REFRESH_ENABLED            0x2000000300000000  // R - 1/0
#define VMMDLL_OPT_CONFIG_TICK_PERIOD                   0x2000000400000000  // RW - base tick period in ms
#define VMMDLL_OPT_CONFIG_READCACHE_TICKS               0x2000000500000000  // RW - memory cache validity period (in ticks)
#define VMMDLL_OPT_CONFIG_TLBCACHE_TICKS                0x2000000600000000  // RW - page table (tlb) cache validity period (in ticks)
#define VMMDLL_OPT_CONFIG_PROCCACHE_TICKS_PARTIAL       0x2000000700000000  // RW - process refresh (partial) period (in ticks)
#define VMMDLL_OPT_CONFIG_PROCCACHE_TICKS_TOTAL         0x2000000800000000  // RW - process refresh (full) period (in ticks)
#define VMMDLL_OPT_CONFIG_VMM_VERSION_MAJOR             0x2000000900000000  // R
#define VMMDLL_OPT_CONFIG_VMM_VERSION_MINOR             0x2000000A00000000  // R
#define VMMDLL_OPT_CONFIG_VMM_VERSION_REVISION          0x2000000B00000000  // R
#define VMMDLL_OPT_CONFIG_STATISTICS_FUNCTIONCALL       0x2000000C00000000  // RW - enable function call statistics (.status/statistics_fncall file)
#define VMMDLL_OPT_CONFIG_IS_PAGING_ENABLED             0x2000000D00000000  // RW - 1/0

#define VMMDLL_OPT_WIN_VERSION_MAJOR                    0x2000010100000000  // R
#define VMMDLL_OPT_WIN_VERSION_MINOR                    0x2000010200000000  // R
#define VMMDLL_OPT_WIN_VERSION_BUILD                    0x2000010300000000  // R
#define VMMDLL_OPT_WIN_SYSTEM_UNIQUE_ID                 0x2000010400000000  // R

#define VMMDLL_OPT_FORENSIC_MODE                        0x2000020100000000  // RW - enable/retrieve forensic mode type [0-4].

#define VMMDLL_OPT_REFRESH_ALL                          0x2001ffff00000000  // W - refresh all caches
#define VMMDLL_OPT_REFRESH_FREQ_MEM                     0x2001000200000000  // W - refresh memory cache (excl. TLB) [partial 33%/call]
#define VMMDLL_OPT_REFRESH_FREQ_TLB                     0x2001000400000000  // W - refresh page table (TLB) cache [partial 33%/call]
#define VMMDLL_OPT_REFRESH_FREQ_FAST                    0x2001040000000000  // W - refresh fast frequency - incl. partial process refresh
#define VMMDLL_OPT_REFRESH_FREQ_MEDIUM                  0x2001000100000000  // W - refresh medium frequency - incl. full process refresh
#define VMMDLL_OPT_REFRESH_FREQ_SLOW                    0x2001001000000000  // W - refresh slow frequency.
#define VMMDLL_OPT_REFRESH_PROCESS                      0x2001000100000000  // W - DEPRECATED: refresh process listings
#define VMMDLL_OPT_REFRESH_READ                         0x2001000200000000  // W - DEPRECATED: refresh physical read cache
#define VMMDLL_OPT_REFRESH_TLB                          0x2001000400000000  // W - DEPRECATED: refresh page table (TLB) cache
#define VMMDLL_OPT_REFRESH_PAGING                       0x2001000800000000  // W - DEPRECATED: refresh virtual memory 'paging' cache
#define VMMDLL_OPT_REFRESH_REGISTRY                     0x2001001000000000  // W - DEPRECATED:
#define VMMDLL_OPT_REFRESH_USER                         0x2001002000000000  // W - DEPRECATED:
#define VMMDLL_OPT_REFRESH_PHYSMEMMAP                   0x2001004000000000  // W - DEPRECATED:
#define VMMDLL_OPT_REFRESH_PFN                          0x2001008000000000  // W - DEPRECATED:
#define VMMDLL_OPT_REFRESH_OBJ                          0x2001010000000000  // W - DEPRECATED:
#define VMMDLL_OPT_REFRESH_NET                          0x2001020000000000  // W - DEPRECATED:

static LPCSTR VMMDLL_MEMORYMODEL_TOSTRING[4] = { "N/A", "X86", "X86PAE", "X64" };

typedef enum tdVMMDLL_MEMORYMODEL_TP {
    VMMDLL_MEMORYMODEL_NA       = 0,
    VMMDLL_MEMORYMODEL_X86      = 1,
    VMMDLL_MEMORYMODEL_X86PAE   = 2,
    VMMDLL_MEMORYMODEL_X64      = 3
} VMMDLL_MEMORYMODEL_TP;

typedef enum tdVMMDLL_SYSTEM_TP {
    VMMDLL_SYSTEM_UNKNOWN_X64   = 1,
    VMMDLL_SYSTEM_WINDOWS_X64   = 2,
    VMMDLL_SYSTEM_UNKNOWN_X86   = 3,
    VMMDLL_SYSTEM_WINDOWS_X86   = 4
} VMMDLL_SYSTEM_TP;

/*
* Set a device specific option value. Please see defines VMMDLL_OPT_* for infor-
* mation about valid option values. Please note that option values may overlap
* between different device types with different meanings.
* -- fOption
* -- pqwValue = pointer to ULONG64 to receive option value.
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_ConfigGet(_In_ ULONG64 fOption, _Out_ PULONG64 pqwValue);

/*
* Set a device specific option value. Please see defines VMMDLL_OPT_* for infor-
* mation about valid option values. Please note that option values may overlap
* between different device types with different meanings.
* -- fOption
* -- qwValue
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_ConfigSet(_In_ ULONG64 fOption, _In_ ULONG64 qwValue);



//-----------------------------------------------------------------------------
// FORWARD DECLARATIONS:
//-----------------------------------------------------------------------------

typedef struct tdVMMDLL_MAP_PFN *PVMMDLL_MAP_PFN;



//-----------------------------------------------------------------------------
// LINUX SPECIFIC DEFINES:
//-----------------------------------------------------------------------------
#ifdef LINUX

#define IMAGE_SIZEOF_SHORT_NAME              8

typedef struct _IMAGE_DATA_DIRECTORY {
    DWORD   VirtualAddress;
    DWORD   Size;
} IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

typedef struct _IMAGE_SECTION_HEADER {
    BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
    union {
        DWORD   PhysicalAddress;
        DWORD   VirtualSize;
    } Misc;
    DWORD   VirtualAddress;
    DWORD   SizeOfRawData;
    DWORD   PointerToRawData;
    DWORD   PointerToRelocations;
    DWORD   PointerToLinenumbers;
    WORD    NumberOfRelocations;
    WORD    NumberOfLinenumbers;
    DWORD   Characteristics;
} IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef struct _SERVICE_STATUS {
    DWORD   dwServiceType;
    DWORD   dwCurrentState;
    DWORD   dwControlsAccepted;
    DWORD   dwWin32ExitCode;
    DWORD   dwServiceSpecificExitCode;
    DWORD   dwCheckPoint;
    DWORD   dwWaitHint;
} SERVICE_STATUS, *LPSERVICE_STATUS;
#endif /* LINUX */



//-----------------------------------------------------------------------------
// VFS - VIRTUAL FILE SYSTEM FUNCTIONALITY BELOW:
// NB! VFS FUNCTIONALITY REQUIRES PLUGINS TO BE INITIALIZED
//     WITH CALL TO VMMDLL_InitializePlugins().
// This is the core of MemProcFS. All implementation and analysis towards
// the virtual file system (vfs) is possible by using functionality below. 
//-----------------------------------------------------------------------------

#define VMMDLL_STATUS_SUCCESS                       ((NTSTATUS)0x00000000L)
#define VMMDLL_STATUS_UNSUCCESSFUL                  ((NTSTATUS)0xC0000001L)
#define VMMDLL_STATUS_END_OF_FILE                   ((NTSTATUS)0xC0000011L)
#define VMMDLL_STATUS_FILE_INVALID                  ((NTSTATUS)0xC0000098L)
#define VMMDLL_STATUS_FILE_SYSTEM_LIMITATION        ((NTSTATUS)0xC0000427L)

#define VMMDLL_VFS_FILELIST_EXINFO_VERSION          1
#define VMMDLL_VFS_FILELIST_VERSION                 2
#define VMMDLL_VFS_FILELISTBLOB_VERSION             0xf88f0001

typedef struct tdVMMDLL_VFS_FILELIST_EXINFO {
    DWORD dwVersion;
    BOOL fCompressed;                   // set flag FILE_ATTRIBUTE_COMPRESSED - (no meaning but shows gui artifact in explorer.exe)
    union {
        FILETIME ftCreationTime;        // 0 = default time
        QWORD qwCreationTime;
    };
    union {
        FILETIME ftLastAccessTime;      // 0 = default time
        QWORD qwLastAccessTime;
    };
    union {
        FILETIME ftLastWriteTime;       // 0 = default time
        QWORD qwLastWriteTime;
    };
} VMMDLL_VFS_FILELIST_EXINFO, *PVMMDLL_VFS_FILELIST_EXINFO;

typedef struct tdVMMDLL_VFS_FILELIST2 {
    DWORD dwVersion;
    VOID(*pfnAddFile)     (_Inout_ HANDLE h, _In_ LPSTR uszName, _In_ ULONG64 cb, _In_opt_ PVMMDLL_VFS_FILELIST_EXINFO pExInfo);
    VOID(*pfnAddDirectory)(_Inout_ HANDLE h, _In_ LPSTR uszName, _In_opt_ PVMMDLL_VFS_FILELIST_EXINFO pExInfo);
    HANDLE h;
} VMMDLL_VFS_FILELIST2, *PVMMDLL_VFS_FILELIST2;

typedef struct tdVMMDLL_VFS_FILELISTBLOB_ENTRY {
    ULONG64 ouszName;                       // byte offset to string from VMMDLL_VFS_FILELISTBLOB.uszMultiText
    ULONG64 cbFileSize;                     // -1 == directory
    VMMDLL_VFS_FILELIST_EXINFO ExInfo;      // optional ExInfo
} VMMDLL_VFS_FILELISTBLOB_ENTRY, *PVMMDLL_VFS_FILELISTBLOB_ENTRY;

typedef struct tdVMMDLL_VFS_FILELISTBLOB {
    DWORD dwVersion;                        // VMMDLL_VFS_FILELISTBLOB_VERSION
    DWORD cbStruct;
    DWORD cFileEntry;
    DWORD cbMultiText;
    union {
        LPSTR uszMultiText;
        QWORD _Reserved;
    };
    DWORD _FutureUse[8];
    VMMDLL_VFS_FILELISTBLOB_ENTRY FileEntry[0];
} VMMDLL_VFS_FILELISTBLOB, *PVMMDLL_VFS_FILELISTBLOB;

/*
* Helper functions for callbacks into the VMM_VFS_FILELIST2 structure.
*/
EXPORTED_FUNCTION
VOID VMMDLL_VfsList_AddFile(_In_ HANDLE pFileList, _In_ LPSTR uszName, _In_ ULONG64 cb, _In_opt_ PVMMDLL_VFS_FILELIST_EXINFO pExInfo);
VOID VMMDLL_VfsList_AddFileW(_In_ HANDLE pFileList, _In_ LPWSTR wszName, _In_ ULONG64 cb, _In_opt_ PVMMDLL_VFS_FILELIST_EXINFO pExInfo);
EXPORTED_FUNCTION
VOID VMMDLL_VfsList_AddDirectory(_In_ HANDLE pFileList, _In_ LPSTR uszName, _In_opt_ PVMMDLL_VFS_FILELIST_EXINFO pExInfo);
VOID VMMDLL_VfsList_AddDirectoryW(_In_ HANDLE pFileList, _In_ LPWSTR wszName, _In_opt_ PVMMDLL_VFS_FILELIST_EXINFO pExInfo);
EXPORTED_FUNCTION BOOL VMMDLL_VfsList_IsHandleValid(_In_ HANDLE pFileList);

/*
* List a directory of files in MemProcFS. Directories and files will be listed
* by callbacks into functions supplied in the pFileList parameter.
* If information of an individual file is needed it's neccessary to list all
* files in its directory.
* -- [uw]szPath
* -- pFileList
* -- return
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_VfsListU(_In_ LPSTR  uszPath, _Inout_ PVMMDLL_VFS_FILELIST2 pFileList);
_Success_(return) BOOL VMMDLL_VfsListW(_In_ LPWSTR wszPath, _Inout_ PVMMDLL_VFS_FILELIST2 pFileList);

/*
* List a directory of files in MemProcFS and return a VMMDLL_VFS_FILELISTBLOB.
* CALLER FREE: VMMDLL_MemFree(return)
* -- uszPath
* -- return
*/
EXPORTED_FUNCTION
_Success_(return != NULL) PVMMDLL_VFS_FILELISTBLOB VMMDLL_VfsListBlobU(_In_ LPSTR uszPath);

/*
* Read select parts of a file in MemProcFS.
* -- [uw]szFileName
* -- pb
* -- cb
* -- pcbRead
* -- cbOffset
* -- return
*
*/
EXPORTED_FUNCTION
NTSTATUS VMMDLL_VfsReadU(_In_ LPSTR  uszFileName, _Out_writes_to_(cb, *pcbRead) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbRead, _In_ ULONG64 cbOffset);
NTSTATUS VMMDLL_VfsReadW(_In_ LPWSTR wszFileName, _Out_writes_to_(cb, *pcbRead) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbRead, _In_ ULONG64 cbOffset);

/*
* Write select parts to a file in MemProcFS.
* -- [uw]szFileName
* -- pb
* -- cb
* -- pcbWrite
* -- cbOffset
* -- return
*/
EXPORTED_FUNCTION
NTSTATUS VMMDLL_VfsWriteU(_In_ LPSTR  uszFileName, _In_reads_(cb) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbWrite, _In_ ULONG64 cbOffset);
NTSTATUS VMMDLL_VfsWriteW(_In_ LPWSTR wszFileName, _In_reads_(cb) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbWrite, _In_ ULONG64 cbOffset);

/*
* Utility functions for MemProcFS read/write towards different underlying data
* representations.
*/
EXPORTED_FUNCTION NTSTATUS VMMDLL_UtilVfsReadFile_FromPBYTE(_In_ PBYTE pbFile, _In_ ULONG64 cbFile, _Out_writes_to_(cb, *pcbRead) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbRead, _In_ ULONG64 cbOffset);
EXPORTED_FUNCTION NTSTATUS VMMDLL_UtilVfsReadFile_FromQWORD(_In_ ULONG64 qwValue, _Out_writes_to_(cb, *pcbRead) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbRead, _In_ ULONG64 cbOffset, _In_ BOOL fPrefix);
EXPORTED_FUNCTION NTSTATUS VMMDLL_UtilVfsReadFile_FromDWORD(_In_ DWORD dwValue, _Out_writes_to_(cb, *pcbRead) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbRead, _In_ ULONG64 cbOffset, _In_ BOOL fPrefix);
EXPORTED_FUNCTION NTSTATUS VMMDLL_UtilVfsReadFile_FromBOOL(_In_ BOOL fValue, _Out_writes_to_(cb, *pcbRead) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbRead, _In_ ULONG64 cbOffset);
EXPORTED_FUNCTION NTSTATUS VMMDLL_UtilVfsWriteFile_BOOL(_Inout_ PBOOL pfTarget, _In_reads_(cb) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbWrite, _In_ ULONG64 cbOffset);
EXPORTED_FUNCTION NTSTATUS VMMDLL_UtilVfsWriteFile_DWORD(_Inout_ PDWORD pdwTarget, _In_reads_(cb) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbWrite, _In_ ULONG64 cbOffset, _In_ DWORD dwMinAllow);



//-----------------------------------------------------------------------------
// PLUGIN MANAGER FUNCTIONALITY BELOW:
// Function and structures to initialize and use MemProcFS plugin functionality.
// The plugin manager is started by a call to function:
// VMM_VfsInitializePlugins. Each built-in plugin and external plugin of which
// the DLL name matches m_*.dll will receive a call to its InitializeVmmPlugin
// function. The plugin/module may decide to call pfnPluginManager_Register to
// register plugins in the form of different names one or more times.
// Example of registration function in a plugin DLL below: 
// 'VOID InitializeVmmPlugin(_In_ PVMM_PLUGIN_REGINFO pRegInfo)'
//-----------------------------------------------------------------------------

/*
* Initialize all potential plugins, both built-in and external, that maps into
* MemProcFS. Please note that plugins are not loaded by default - they have to
* be explicitly loaded by calling this function. They will be unloaded on a
* general close of the vmm dll.
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_InitializePlugins();

#define VMMDLL_PLUGIN_CONTEXT_MAGIC                 0xc0ffee663df9301c
#define VMMDLL_PLUGIN_CONTEXT_VERSION               4
#define VMMDLL_PLUGIN_REGINFO_MAGIC                 0xc0ffee663df9301d
#define VMMDLL_PLUGIN_REGINFO_VERSION               13
#define VMMDLL_PLUGIN_FORENSIC_JSONDATA_VERSION     0xc0ee0001

#define VMMDLL_PLUGIN_NOTIFY_VERBOSITYCHANGE        0x01
#define VMMDLL_PLUGIN_NOTIFY_REFRESH_FAST           0x05    // refresh fast event   - at partial process refresh.
#define VMMDLL_PLUGIN_NOTIFY_REFRESH_MEDIUM         0x02    // refresh medium event - at full process refresh.
#define VMMDLL_PLUGIN_NOTIFY_REFRESH_SLOW           0x04    // refresh slow event   - at registry refresh.

#define VMMDLL_PLUGIN_NOTIFY_FORENSIC_INIT          0x01000100
#define VMMDLL_PLUGIN_NOTIFY_FORENSIC_INIT_COMPLETE 0x01000200

typedef HANDLE                                      *PVMMDLL_PLUGIN_INTERNAL_CONTEXT;

typedef struct tdVMMDLL_PLUGIN_CONTEXT {
    ULONG64 magic;
    WORD wVersion;
    WORD wSize;
    DWORD dwPID;
    PVOID pProcess;
    LPSTR uszModule;
    LPSTR uszPath;
    PVOID pvReserved1;
    PVMMDLL_PLUGIN_INTERNAL_CONTEXT ctxM;       // optional internal module context.
} VMMDLL_PLUGIN_CONTEXT, *PVMMDLL_PLUGIN_CONTEXT;

typedef struct tdVMMDLL_PLUGIN_FORENSIC_JSONDATA {
    DWORD dwVersion;        // must equal VMMDLL_PLUGIN_FORENSIC_JSONDATA_VERSION
    BOOL fVerbose;
    LPSTR szjType;          // log type/name (json encoded)
    DWORD i;
    DWORD dwPID;
    QWORD vaObj;
    BOOL fva[2];            // log va even if zero
    QWORD va[2];
    BOOL fNum[2];           // log num even if zero
    QWORD qwNum[2];
    BOOL fHex[2];           // log hex even if zero
    QWORD qwHex[2];
    // str: will be prioritized in order: szu > wsz.
    LPCSTR usz[2];          // str: utf-8 encoded
    LPCWSTR wsz[2];         // str: wide
    BYTE _Reserved[0x4000+256];
} VMMDLL_PLUGIN_FORENSIC_JSONDATA, *PVMMDLL_PLUGIN_FORENSIC_JSONDATA;

typedef struct tdVMMDLL_PLUGIN_FORENSIC_INGEST_PHYSMEM {
    DWORD cMEMs;
    BOOL fValid;
    QWORD paBase;
    PPMEM_SCATTER ppMEMs;
    PVMMDLL_MAP_PFN pPfnMap;
} VMMDLL_PLUGIN_FORENSIC_INGEST_PHYSMEM, *PVMMDLL_PLUGIN_FORENSIC_INGEST_PHYSMEM;

typedef struct tdVMMDLL_PLUGIN_REGINFO {
    ULONG64 magic;
    WORD wVersion;
    WORD wSize;
    VMMDLL_MEMORYMODEL_TP tpMemoryModel;
    VMMDLL_SYSTEM_TP tpSystem;
    HMODULE hDLL;
    BOOL(*pfnPluginManager_Register)(struct tdVMMDLL_PLUGIN_REGINFO *pPluginRegInfo);
    DWORD _Reserved[32];
    // python plugin information - not for general use
    struct {
        BOOL fPythonStandalone;
        DWORD _Reserved;
        HMODULE hReservedDllPython3;
        HMODULE hReservedDllPython3X;
    } python;
    // general plugin registration info to be filled out by the plugin below:
    struct {
        PVMMDLL_PLUGIN_INTERNAL_CONTEXT ctxM;   // optional internal module context [must be cleaned by pfnClose() call].
        CHAR uszPathName[128];
        BOOL fRootModule;
        BOOL fProcessModule;
        BOOL fRootModuleHidden;
        BOOL fProcessModuleHidden;
        CHAR sTimelineNameShort[6];
        CHAR _Reserved[2];
        CHAR uszTimelineFile[32];
        CHAR _Reserved2[32];
    } reg_info;
    // function plugin registration info to be filled out by the plugin below:
    struct {
        BOOL(*pfnList)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP, _Inout_ PHANDLE pFileList);
        NTSTATUS(*pfnRead)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP, _Out_writes_to_(cb, *pcbRead) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbRead,  _In_ ULONG64 cbOffset);
        NTSTATUS(*pfnWrite)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP, _In_reads_(cb) PBYTE pb, _In_ DWORD cb, _Out_ PDWORD pcbWrite, _In_ ULONG64 cbOffset);
        VOID(*pfnNotify)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP, _In_ DWORD fEvent, _In_opt_ PVOID pvEvent, _In_opt_ DWORD cbEvent);
        VOID(*pfnClose)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP);
        BOOL(*pfnVisibleModule)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP);
        PVOID pvReserved[10];
    } reg_fn;
    // Optional forensic plugin functionality for forensic (more comprehensive)
    // analysis of various data. Functions are optional. Functions are called
    // in single-threaded mode regards to the plugin itself - but 'ingest'
    // functions are called in-parallel multi-threaded between plugins.
    // Functions are called in the order of:
    // pfnInitialize(), pfnIngest*(), pfnTimeline(), pfnLogJSON(), pfnFinalize()
    struct {
        PVOID(*pfnInitialize)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP);
        VOID(*pfnFinalize)(_In_opt_ PVOID ctxfc);
        VOID(*pfnTimeline)(
            _In_opt_ PVOID ctxfc,
            _In_ HANDLE hTimeline,
            _In_ VOID(*pfnAddEntry)(_In_ HANDLE hTimeline, _In_ QWORD ft, _In_ DWORD dwAction, _In_ DWORD dwPID, _In_ DWORD dwData32, _In_ QWORD qwData64, _In_ LPSTR uszText),
            _In_ VOID(*pfnEntryAddBySql)(_In_ HANDLE hTimeline, _In_ DWORD cEntrySql, _In_ LPSTR *pszEntrySql));
        VOID(*pfnIngestPhysmem)(_In_opt_ PVOID ctxfc, _In_ PVMMDLL_PLUGIN_FORENSIC_INGEST_PHYSMEM pIngestPhysmem);
        VOID(*pfnIngestFinalize)(_In_opt_ PVOID ctxfc);
        PVOID pvReserved[10];
        VOID(*pfnLogJSON)(_In_ PVMMDLL_PLUGIN_CONTEXT ctxP, _In_ VOID(*pfnLogJSON)(_In_ PVMMDLL_PLUGIN_FORENSIC_JSONDATA pData));
    } reg_fnfc;
    // Additional system information - read/only by the plugins.
    struct {
        BOOL f32;
        DWORD dwVersionMajor;
        DWORD dwVersionMinor;
        DWORD dwVersionBuild;
        DWORD _Reserved[32];
    } sysinfo;
} VMMDLL_PLUGIN_REGINFO, *PVMMDLL_PLUGIN_REGINFO;



//-----------------------------------------------------------------------------
// VMM CORE FUNCTIONALITY BELOW:
// Vmm core functaionlity such as read (and write) to both virtual and physical
// memory. NB! writing will only work if the target is supported - i.e. not a
// memory dump file...
// To read physical memory specify dwPID as (DWORD)-1
//-----------------------------------------------------------------------------

#define VMMDLL_PID_PROCESS_WITH_KERNELMEMORY        0x80000000      // Combine with dwPID to enable process kernel memory (NB! use with extreme care).

// FLAG used to supress the default read cache in calls to VMM_MemReadEx()
// which will lead to the read being fetched from the target system always.
// Cached page tables (used for translating virtual2physical) are still used.
#define VMMDLL_FLAG_NOCACHE                         0x0001  // do not use the data cache (force reading from memory acquisition device)
#define VMMDLL_FLAG_ZEROPAD_ON_FAIL                 0x0002  // zero pad failed physical memory reads and report success if read within range of physical memory.
#define VMMDLL_FLAG_FORCECACHE_READ                 0x0008  // force use of cache - fail non-cached pages - only valid for reads, invalid with VMM_FLAG_NOCACHE/VMM_FLAG_ZEROPAD_ON_FAIL.
#define VMMDLL_FLAG_NOPAGING                        0x0010  // do not try to retrieve memory from paged out memory from pagefile/compressed (even if possible)
#define VMMDLL_FLAG_NOPAGING_IO                     0x0020  // do not try to retrieve memory from paged out memory if read would incur additional I/O (even if possible).
#define VMMDLL_FLAG_NOCACHEPUT                      0x0100  // do not write back to the data cache upon successful read from memory acquisition device.
#define VMMDLL_FLAG_CACHE_RECENT_ONLY               0x0200  // only fetch from the most recent active cache region when reading.
#define VMMDLL_FLAG_NO_PREDICTIVE_READ              0x0400  // do not perform additional predictive page reads (default on smaller requests).

/*
* Read memory in various non-contigious locations specified by the pointers to
* the items in the ppMEMs array. Result for each unit of work will be given
* individually. No upper limit of number of items to read, but no performance
* boost will be given if above hardware limit. Max size of each unit of work is
* one 4k page (4096 bytes). Reads must not cross 4k page boundaries. Reads must
* start at even DWORDs (4-bytes).
* -- dwPID - PID of target process, (DWORD)-1 to read physical memory.
* -- ppMEMs = array of scatter read headers.
* -- cpMEMs = count of ppMEMs.
* -- flags = optional flags as given by VMMDLL_FLAG_*
* -- return = the number of successfully read items.
*/
EXPORTED_FUNCTION
DWORD VMMDLL_MemReadScatter(_In_ DWORD dwPID, _Inout_ PPMEM_SCATTER ppMEMs, _In_ DWORD cpMEMs, _In_ DWORD flags);

/*
* Read a single 4096-byte page of memory.
* -- dwPID - PID of target process, (DWORD)-1 to read physical memory.
* -- qwA
* -- pbPage
* -- return = success/fail (depending if all requested bytes are read or not).
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_MemReadPage(_In_ DWORD dwPID, _In_ ULONG64 qwA, _Inout_bytecount_(4096) PBYTE pbPage);

/*
* Read a contigious arbitrary amount of memory.
* -- dwPID - PID of target process, (DWORD)-1 to read physical memory.
* -- qwA
* -- pb
* -- cb
* -- return = success/fail (depending if all requested bytes are read or not).
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_MemRead(_In_ DWORD dwPID, _In_ ULONG64 qwA, _Out_writes_(cb) PBYTE pb, _In_ DWORD cb);

/*
* Read a contigious amount of memory and report the number of bytes read in pcbRead.
* -- dwPID - PID of target process, (DWORD)-1 to read physical memory.
* -- qwA
* -- pb
* -- cb
* -- pcbRead
* -- flags = flags as in VMMDLL_FLAG_*
* -- return = success/fail. NB! reads may report as success even if 0 bytes are
*        read - it's recommended to verify pcbReadOpt parameter.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_MemReadEx(_In_ DWORD dwPID, _In_ ULONG64 qwA, _Out_writes_(cb) PBYTE pb, _In_ DWORD cb, _Out_opt_ PDWORD pcbReadOpt, _In_ ULONG64 flags);

/*
* Prefetch a number of addresses (specified in the pA array) into the memory
* cache. This function is to be used to batch larger known reads into local
* cache before making multiple smaller reads - which will then happen from
* the cache. Function exists for performance reasons.
* -- dwPID = PID of target process, (DWORD)-1 for physical memory.
* -- pPrefetchAddresses = array of addresses to read into cache.
* -- cPrefetchAddresses
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_MemPrefetchPages(_In_ DWORD dwPID, _In_reads_(cPrefetchAddresses) PULONG64 pPrefetchAddresses, _In_ DWORD cPrefetchAddresses);

/*
* Write a contigious arbitrary amount of memory. Please note some virtual memory
* such as pages of executables (such as DLLs) may be shared between different
* virtual memory over different processes. As an example a write to kernel32.dll
* in one process is likely to affect kernel32 in the whole system - in all
* processes. Heaps and Stacks and other memory are usually safe to write to.
* Please take care when writing to memory!
* -- dwPID = PID of target process, (DWORD)-1 to read physical memory.
* -- qwA
* -- pb
* -- cb
* -- return = TRUE on success, FALSE on partial or zero write.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_MemWrite(_In_ DWORD dwPID, _In_ ULONG64 qwA, _In_reads_(cb) PBYTE pb, _In_ DWORD cb);

/*
* Translate a virtual address to a physical address by walking the page tables
* of the specified process.
* -- dwPID
* -- qwVA
* -- pqwPA
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_MemVirt2Phys(_In_ DWORD dwPID, _In_ ULONG64 qwVA, _Out_ PULONG64 pqwPA);



//-----------------------------------------------------------------------------
// VMM PROCESS MAP FUNCTIONALITY BELOW:
// Functionality for retrieving process related collections of items such as
// page table map (PTE), virtual address descriptor map (VAD), loaded modules,
// heaps and threads.
//-----------------------------------------------------------------------------

#define VMMDLL_MAP_PTE_VERSION              2
#define VMMDLL_MAP_VAD_VERSION              6
#define VMMDLL_MAP_VADEX_VERSION            3
#define VMMDLL_MAP_MODULE_VERSION           5
#define VMMDLL_MAP_UNLOADEDMODULE_VERSION   2
#define VMMDLL_MAP_EAT_VERSION              2
#define VMMDLL_MAP_IAT_VERSION              2
#define VMMDLL_MAP_HEAP_VERSION             2
#define VMMDLL_MAP_THREAD_VERSION           3
#define VMMDLL_MAP_HANDLE_VERSION           2
#define VMMDLL_MAP_NET_VERSION              3
#define VMMDLL_MAP_PHYSMEM_VERSION          2
#define VMMDLL_MAP_USER_VERSION             2
#define VMMDLL_MAP_SERVICE_VERSION          3

// flags to check for existence in the fPage field of VMMDLL_MAP_PTEENTRY
#define VMMDLL_MEMMAP_FLAG_PAGE_W          0x0000000000000002
#define VMMDLL_MEMMAP_FLAG_PAGE_NS         0x0000000000000004
#define VMMDLL_MEMMAP_FLAG_PAGE_NX         0x8000000000000000
#define VMMDLL_MEMMAP_FLAG_PAGE_MASK       0x8000000000000006

typedef enum tdVMMDLL_PTE_TP {
    VMMDLL_PTE_TP_NA = 0,
    VMMDLL_PTE_TP_HARDWARE = 1,
    VMMDLL_PTE_TP_TRANSITION = 2,
    VMMDLL_PTE_TP_PROTOTYPE = 3,
    VMMDLL_PTE_TP_DEMANDZERO = 4,
    VMMDLL_PTE_TP_COMPRESSED = 5,
    VMMDLL_PTE_TP_PAGEFILE = 6,
} VMMDLL_PTE_TP, *PVMMDLL_PTE_TP;

typedef struct tdVMMDLL_MAP_PTEENTRY {
    QWORD vaBase;
    QWORD cPages;
    QWORD fPage;
    BOOL  fWoW64;
    DWORD _FutureUse1;
    union { LPSTR  uszText; LPWSTR wszText; };              // U/W dependant
    DWORD _Reserved1;
    DWORD cSoftware;    // # software (non active) PTEs in region
} VMMDLL_MAP_PTEENTRY, *PVMMDLL_MAP_PTEENTRY;

typedef struct tdVMMDLL_MAP_VADENTRY {
    QWORD vaStart;
    QWORD vaEnd;
    QWORD vaVad;
    // DWORD 0
    DWORD VadType           : 3;   // Pos 0
    DWORD Protection        : 5;   // Pos 3
    DWORD fImage            : 1;   // Pos 8
    DWORD fFile             : 1;   // Pos 9
    DWORD fPageFile         : 1;   // Pos 10
    DWORD fPrivateMemory    : 1;   // Pos 11
    DWORD fTeb              : 1;   // Pos 12
    DWORD fStack            : 1;   // Pos 13
    DWORD fSpare            : 2;   // Pos 14
    DWORD HeapNum           : 7;   // Pos 16
    DWORD fHeap             : 1;   // Pos 23
    DWORD cwszDescription   : 8;   // Pos 24
    // DWORD 1
    DWORD CommitCharge      : 31;   // Pos 0
    DWORD MemCommit         : 1;    // Pos 31
    DWORD u2;
    DWORD cbPrototypePte;
    QWORD vaPrototypePte;
    QWORD vaSubsection;
    union { LPSTR  uszText; LPWSTR wszText; };              // U/W dependant
    DWORD _FutureUse1;
    DWORD _Reserved1;
    QWORD vaFileObject;             // only valid if fFile/fImage _and_ after wszText is initialized
    DWORD cVadExPages;              // number of "valid" VadEx pages in this VAD.
    DWORD cVadExPagesBase;          // number of "valid" VadEx pages in "previous" VADs
    QWORD _Reserved2;
} VMMDLL_MAP_VADENTRY, *PVMMDLL_MAP_VADENTRY;

typedef struct tdVMMDLL_MAP_VADEXENTRY {
    VMMDLL_PTE_TP tp;
    DWORD iPML;
    QWORD va;
    QWORD pa;
    QWORD pte;
    struct {
        DWORD _Reserved1;
        VMMDLL_PTE_TP tp;
        QWORD pa;
        QWORD pte;
    } proto;
    QWORD vaVadBase;
} VMMDLL_MAP_VADEXENTRY, *PVMMDLL_MAP_VADEXENTRY;

typedef enum tdVMMDLL_MODULE_TP {
    VMMDLL_MODULE_TP_NORMAL = 0,
    VMMDLL_MODULE_TP_DATA = 1,
    VMMDLL_MODULE_TP_NOTLINKED = 2,
    VMMDLL_MODULE_TP_INJECTED = 3,
} VMMDLL_MODULE_TP;

typedef struct tdVMMDLL_MAP_MODULEENTRY {
    QWORD vaBase;
    QWORD vaEntry;
    DWORD cbImageSize;
    BOOL  fWoW64;
    union { LPSTR  uszText; LPWSTR wszText; };              // U/W dependant
    DWORD _Reserved3;
    DWORD _Reserved4;
    union { LPSTR  uszFullName; LPWSTR wszFullName; };      // U/W dependant
    VMMDLL_MODULE_TP tp;
    DWORD cbFileSizeRaw;
    DWORD cSection;
    DWORD cEAT;
    DWORD cIAT;
    DWORD _Reserved2;
    QWORD _Reserved1[2];
} VMMDLL_MAP_MODULEENTRY, *PVMMDLL_MAP_MODULEENTRY;

typedef struct tdVMMDLL_MAP_UNLOADEDMODULEENTRY {
    QWORD vaBase;
    DWORD cbImageSize;
    BOOL  fWoW64;
    union { LPSTR  uszText; LPWSTR wszText; };              // U/W dependant
    DWORD _FutureUse1;
    DWORD dwCheckSum;               // user-mode only
    DWORD dwTimeDateStamp;          // user-mode only
    DWORD _Reserved1;
    QWORD ftUnload;                 // kernel-mode only
} VMMDLL_MAP_UNLOADEDMODULEENTRY, *PVMMDLL_MAP_UNLOADEDMODULEENTRY;

typedef struct tdVMMDLL_MAP_EATENTRY {
    QWORD vaFunction;
    DWORD dwOrdinal;
    DWORD oFunctionsArray;          // PIMAGE_EXPORT_DIRECTORY->AddressOfFunctions[oFunctionsArray]
    DWORD oNamesArray;              // PIMAGE_EXPORT_DIRECTORY->AddressOfNames[oNamesArray]
    DWORD _FutureUse1;
    union { LPSTR  uszFunction; LPWSTR wszFunction; };      // U/W dependant
} VMMDLL_MAP_EATENTRY, *PVMMDLL_MAP_EATENTRY;

typedef struct tdVMMDLL_MAP_IATENTRY {
    QWORD vaFunction;
    union { LPSTR  uszFunction; LPWSTR wszFunction; };      // U/W dependant
    DWORD _FutureUse1;
    DWORD _FutureUse2;
    union { LPSTR  uszModule; LPWSTR wszModule; };          // U/W dependant
    struct {
        BOOL f32;
        WORD wHint;
        WORD _Reserved1;
        DWORD rvaFirstThunk;
        DWORD rvaOriginalFirstThunk;
        DWORD rvaNameModule;
        DWORD rvaNameFunction;
    } Thunk;
} VMMDLL_MAP_IATENTRY, *PVMMDLL_MAP_IATENTRY;

typedef struct tdVMMDLL_MAP_HEAPENTRY {
    QWORD vaHeapSegment;
    DWORD cPages;
    DWORD cPagesUnCommitted : 24;
    DWORD HeapId : 7;
    DWORD fPrimary : 1;
} VMMDLL_MAP_HEAPENTRY, *PVMMDLL_MAP_HEAPENTRY;

typedef struct tdVMMDLL_MAP_THREADENTRY {
    DWORD dwTID;
    DWORD dwPID;
    DWORD dwExitStatus;
    UCHAR bState;
    UCHAR bRunning;
    UCHAR bPriority;
    UCHAR bBasePriority;
    QWORD vaETHREAD;
    QWORD vaTeb;
    QWORD ftCreateTime;
    QWORD ftExitTime;
    QWORD vaStartAddress;
    QWORD vaStackBaseUser;          // value from _NT_TIB / _TEB
    QWORD vaStackLimitUser;         // value from _NT_TIB / _TEB
    QWORD vaStackBaseKernel;
    QWORD vaStackLimitKernel;
    QWORD vaTrapFrame;
    QWORD vaRIP;                    // RIP register (if user mode)
    QWORD vaRSP;                    // RSP register (if user mode)
    QWORD qwAffinity;
    DWORD dwUserTime;
    DWORD dwKernelTime;
    UCHAR bSuspendCount;
    UCHAR _FutureUse1[3];
    DWORD _FutureUse2[15];
} VMMDLL_MAP_THREADENTRY, *PVMMDLL_MAP_THREADENTRY;

typedef struct tdVMMDLL_MAP_HANDLEENTRY {
    QWORD vaObject;
    DWORD dwHandle;
    DWORD dwGrantedAccess : 24;
    DWORD iType : 8;
    QWORD qwHandleCount;
    QWORD qwPointerCount;
    QWORD vaObjectCreateInfo;
    QWORD vaSecurityDescriptor;
    union { LPSTR  uszText; LPWSTR wszText; };              // U/W dependant
    DWORD _FutureUse2;
    DWORD dwPID;
    DWORD dwPoolTag;
    DWORD _FutureUse[5];
    union { LPSTR  uszType; LPWSTR wszType; QWORD _Pad1; }; // U/W dependant
} VMMDLL_MAP_HANDLEENTRY, *PVMMDLL_MAP_HANDLEENTRY;

typedef struct tdVMMDLL_MAP_NETENTRY {
    DWORD dwPID;
    DWORD dwState;
    WORD _FutureUse3[3];
    WORD AF;                        // address family (IPv4/IPv6)
    struct {
        BOOL fValid;
        WORD _Reserved;
        WORD port;
        BYTE pbAddr[16];            // ipv4 = 1st 4 bytes, ipv6 = all bytes
        union { LPSTR  uszText; LPWSTR wszText; };          // U/W dependant
    } Src;
    struct {
        BOOL fValid;
        WORD _Reserved;
        WORD port;
        BYTE pbAddr[16];            // ipv4 = 1st 4 bytes, ipv6 = all bytes
        union { LPSTR  uszText; LPWSTR wszText; };          // U/W dependant
    } Dst;
    QWORD vaObj;
    QWORD ftTime;
    DWORD dwPoolTag;
    DWORD _FutureUse4;
    union { LPSTR  uszText; LPWSTR wszText; };              // U/W dependant
    DWORD _FutureUse2[4];
} VMMDLL_MAP_NETENTRY, *PVMMDLL_MAP_NETENTRY;

typedef struct tdVMMDLL_MAP_PHYSMEMENTRY {
    QWORD pa;
    QWORD cb;
} VMMDLL_MAP_PHYSMEMENTRY, *PVMMDLL_MAP_PHYSMEMENTRY;

typedef struct tdVMMDLL_MAP_USERENTRY {
    DWORD _FutureUse1[2];
    union { LPSTR  uszText; LPWSTR wszText; };              // U/W dependant
    ULONG64 vaRegHive;
    union { LPSTR  uszSID; LPWSTR wszSID; };                // U/W dependant
    DWORD _FutureUse2[2];
} VMMDLL_MAP_USERENTRY, *PVMMDLL_MAP_USERENTRY;

typedef struct tdVMMDLL_MAP_SERVICEENTRY {
    QWORD vaObj;
    DWORD dwOrdinal;
    DWORD dwStartType;
    SERVICE_STATUS ServiceStatus;
    union { LPSTR  uszServiceName; LPWSTR wszServiceName; QWORD _Reserved1; };  // U/W dependant
    union { LPSTR  uszDisplayName; LPWSTR wszDisplayName; QWORD _Reserved2; };  // U/W dependant
    union { LPSTR  uszPath;        LPWSTR wszPath;        QWORD _Reserved3; };  // U/W dependant
    union { LPSTR  uszUserTp;      LPWSTR wszUserTp;      QWORD _Reserved4; };  // U/W dependant
    union { LPSTR  uszUserAcct;    LPWSTR wszUserAcct;    QWORD _Reserved5; };  // U/W dependant
    union { LPSTR  uszImagePath;   LPWSTR wszImagePath;   QWORD _Reserved6; };  // U/W dependant
    DWORD dwPID;
    DWORD _FutureUse1;
    QWORD _FutureUse2;
} VMMDLL_MAP_SERVICEENTRY, *PVMMDLL_MAP_SERVICEENTRY;

typedef struct tdVMMDLL_MAP_PTE {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    PBYTE pbMultiText;              // NULL or multi-wstr pointed into by VMMDLL_MAP_VADENTRY.wszText
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_PTEENTRY pMap[];     // map entries.
} VMMDLL_MAP_PTE, *PVMMDLL_MAP_PTE;

typedef struct tdVMMDLL_MAP_VAD {
    DWORD dwVersion;
    DWORD _Reserved1[4];
    DWORD cPage;                    // # pages in vad map.
    PBYTE pbMultiText;              // NULL or multi-wstr pointed into by VMMDLL_MAP_VADENTRY.wszText
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_VADENTRY pMap[];     // map entries.
} VMMDLL_MAP_VAD, *PVMMDLL_MAP_VAD;

typedef struct tdVMMDLL_MAP_VADEX {
    DWORD dwVersion;
    DWORD _Reserved1[4];
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_VADEXENTRY pMap[];   // map entries.
} VMMDLL_MAP_VADEX, *PVMMDLL_MAP_VADEX;

typedef struct tdVMMDLL_MAP_MODULE {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    PBYTE pbMultiText;              // multi-wstr pointed into by VMMDLL_MAP_MODULEENTRY.wszText
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_MODULEENTRY pMap[];  // map entries.
} VMMDLL_MAP_MODULE, *PVMMDLL_MAP_MODULE;

typedef struct tdVMMDLL_MAP_UNLOADEDMODULE {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    PBYTE pbMultiText;              // multi-wstr pointed into by VMMDLL_MAP_MODULEENTRY.wszText
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_UNLOADEDMODULEENTRY pMap[];  // map entries.
} VMMDLL_MAP_UNLOADEDMODULE, *PVMMDLL_MAP_UNLOADEDMODULE;

typedef struct tdVMMDLL_MAP_EAT {
    DWORD dwVersion;
    DWORD dwOrdinalBase;
    DWORD cNumberOfNames;
    DWORD cNumberOfFunctions;
    DWORD _Reserved1[4];
    QWORD vaModuleBase;
    QWORD vaAddressOfFunctions;
    QWORD vaAddressOfNames;
    PBYTE pbMultiText;              // multi-str pointed into by VMM_MAP_EATENTRY.wszFunction
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_EATENTRY pMap[];     // map entries.
} VMMDLL_MAP_EAT, *PVMMDLL_MAP_EAT;

typedef struct tdVMMDLL_MAP_IAT {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    QWORD vaModuleBase;
    PBYTE pbMultiText;              // multi-str pointed into by VMM_MAP_EATENTRY.[wszFunction|wszModule]
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_IATENTRY pMap[];     // map entries.
} VMMDLL_MAP_IAT, *PVMMDLL_MAP_IAT;

typedef struct tdVMMDLL_MAP_HEAP {
    DWORD dwVersion;
    DWORD _Reserved1[8];
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_HEAPENTRY pMap[];    // map entries.
} VMMDLL_MAP_HEAP, *PVMMDLL_MAP_HEAP;

typedef struct tdVMMDLL_MAP_THREAD {
    DWORD dwVersion;
    DWORD _Reserved[8];
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_THREADENTRY pMap[];  // map entries.
} VMMDLL_MAP_THREAD, *PVMMDLL_MAP_THREAD;

typedef struct tdVMMDLL_MAP_HANDLE {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    PBYTE pbMultiText;              // multi-wstr pointed into by VMMDLL_MAP_HANDLEENTRY.wszText
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_HANDLEENTRY pMap[];  // map entries.
} VMMDLL_MAP_HANDLE, *PVMMDLL_MAP_HANDLE;

typedef struct tdVMMDLL_MAP_NET {
    DWORD dwVersion;
    DWORD _Reserved1;
    PBYTE pbMultiText;              // multi-wstr pointed into by VMM_MAP_NETENTRY.wszText
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_NETENTRY pMap[];     // map entries.
} VMMDLL_MAP_NET, *PVMMDLL_MAP_NET;

typedef struct tdVMMDLL_MAP_PHYSMEM {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    DWORD cMap;                     // # map entries.
    DWORD _Reserved2;
    VMMDLL_MAP_PHYSMEMENTRY pMap[]; // map entries.
} VMMDLL_MAP_PHYSMEM, *PVMMDLL_MAP_PHYSMEM;

typedef struct tdVMMDLL_MAP_USER {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    PBYTE pbMultiText;              // multi-wstr pointed into by VMMDLL_MAP_USERENTRY.wszText
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_USERENTRY pMap[];    // map entries.
} VMMDLL_MAP_USER, *PVMMDLL_MAP_USER;

typedef struct tdVMMDLL_MAP_SERVICE {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    PBYTE pbMultiText;              // multi-wstr pointed into by VMMDLL_MAP_SERVICEENTRY.wsz*
    DWORD cbMultiText;
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_SERVICEENTRY pMap[]; // map entries.
} VMMDLL_MAP_SERVICE, *PVMMDLL_MAP_SERVICE;

/*
* Retrieve the memory map entries based on hardware page tables (PTE) for the
* process. If pPteMap is set to NULL the number of bytes required will be
* returned in parameter pcbPteMap.
* Entries returned are sorted on VMMDLL_MAP_PTEENTRY.vaBase
* -- dwPID
* -- pPteMap = buffer of minimum byte length *pcbPteMap or NULL.
* -- pcbPteMap = pointer to byte count of pPteMap buffer.
* -- fIdentifyModules = try identify modules as well (= slower)
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetPteU(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbPteMap) PVMMDLL_MAP_PTE pPteMap, _Inout_ PDWORD pcbPteMap, _In_ BOOL fIdentifyModules);
_Success_(return) BOOL VMMDLL_Map_GetPteW(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbPteMap) PVMMDLL_MAP_PTE pPteMap, _Inout_ PDWORD pcbPteMap, _In_ BOOL fIdentifyModules);

/*
* Retrieve memory map entries based on virtual address descriptor (VAD) for
* the process. If pVadMap is set to NULL the number of bytes required
* will be returned in parameter pcbVadMap.
* Entries returned are sorted on VMMDLL_MAP_VADENTRY.vaStart
* -- dwPID
* -- pVadMap = buffer of minimum byte length *pcbVadMap or NULL.
* -- pcbVadMap = pointer to byte count of pVadMap buffer.
* -- fIdentifyModules = try identify modules as well (= slower)
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetVadU(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbVadMap) PVMMDLL_MAP_VAD pVadMap, _Inout_ PDWORD pcbVadMap, _In_ BOOL fIdentifyModules);
_Success_(return) BOOL VMMDLL_Map_GetVadW(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbVadMap) PVMMDLL_MAP_VAD pVadMap, _Inout_ PDWORD pcbVadMap, _In_ BOOL fIdentifyModules);

/*
* Retrieve extended memory map information about a select sub-set of the memory
* map. If pVadExMap is set to NULL the number of bytes required will be
* returned in the parameter pcbVadExMap.
* -- dwPID
* -- pVadExMap = buffer of minimum byte length *pcbVadExMap or NULL.
* -- pcbVadExMap = pointer to byte count of pVadExMap buffer.
* -- oPage = offset in number of pages from process start.
* -- cPage = number of pages to process from oPages base.
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_Map_GetVadEx(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbVadExMap) PVMMDLL_MAP_VADEX pVadExMap, _Inout_ PDWORD pcbVadExMap, _In_ DWORD oPage, _In_ DWORD cPage);

/*
* Retrieve the modules (.dlls) for the specified process. If pModuleMap is set
* to NULL the number of bytes required will be returned in parameter pcbModuleMap.
* -- dwPID
* -- pModuleMap = buffer of minimum byte length *pcbModuleMap or NULL.
* -- pcbModuleMap = pointer to byte count of pModuleMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetModuleU(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbModuleMap) PVMMDLL_MAP_MODULE pModuleMap, _Inout_ PDWORD pcbModuleMap);
_Success_(return) BOOL VMMDLL_Map_GetModuleW(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbModuleMap) PVMMDLL_MAP_MODULE pModuleMap, _Inout_ PDWORD pcbModuleMap);

/*
* Retrieve a module (.dll) entry given a process and module name.
* If pModuleMapEntry is set to NULL the number of bytes required
* will be returned in parameter pcbModuleMapEntry.
* If pcbModuleMapEntry is set to NULL the pModuleMapEntry is assumed to be of
* size sizeof(VMMDLL_MAP_MODULEENTRY) and data without names will be copied.
* -- dwPID
* -- [uw]szModuleName = module name (or ""/NULL for 1st module entry).
* -- pModuleMapEntry = buffer of minimum byte length *pcbModuleMapEntry or NULL.
* -- pcbModuleMapEntry = pointer to byte count of pModuleMapEntry buffer or NULL.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetModuleFromNameU(_In_ DWORD dwPID, _In_opt_ LPSTR  uszModuleName, _Out_writes_bytes_opt_(*pcbModuleMapEntry) PVMMDLL_MAP_MODULEENTRY pModuleMapEntry, _Inout_opt_ PDWORD pcbModuleMapEntry);
_Success_(return) BOOL VMMDLL_Map_GetModuleFromNameW(_In_ DWORD dwPID, _In_opt_ LPWSTR wszModuleName, _Out_writes_bytes_opt_(*pcbModuleMapEntry) PVMMDLL_MAP_MODULEENTRY pModuleMapEntry, _Inout_opt_ PDWORD pcbModuleMapEntry);

/*
* Retrieve the unloaded modules (.dll/.sys) for the specified process. If
* pUnloadedModuleMap is set to NULL the number of bytes required will be
* returned in parameter pcbUnloadedModuleMap.
* -- dwPID
* -- pUnloadedModuleMap = buffer of minimum byte length *pcbUnloadedModuleMap or NULL.
* -- pcbUnloadedModuleMap = pointer to byte count of pUnloadedModuleMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetUnloadedModuleU(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbUnloadedModuleMap) PVMMDLL_MAP_UNLOADEDMODULE pUnloadedModuleMap, _Inout_ PDWORD pcbUnloadedModuleMap);
_Success_(return) BOOL VMMDLL_Map_GetUnloadedModuleW(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbUnloadedModuleMap) PVMMDLL_MAP_UNLOADEDMODULE pUnloadedModuleMap, _Inout_ PDWORD pcbUnloadedModuleMap);

/*
* Retrieve the module exported functions from the export address table (EAT).
* If pEatMap is set to NULL the number of bytes required will be returned in
* parameter pcbEatMap.
* -- dwPID
* -- [uw]szModuleName
* -- pEatMap = buffer of minimum byte length *pcbEatMap or NULL.
* -- pcbEatMap = pointer to byte count of pEatMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetEATU(_In_ DWORD dwPID, _In_ LPSTR  uszModuleName, _Out_writes_bytes_opt_(*pcbEatMap) PVMMDLL_MAP_EAT pEatMap, _Inout_ PDWORD pcbEatMap);
_Success_(return) BOOL VMMDLL_Map_GetEATW(_In_ DWORD dwPID, _In_ LPWSTR wszModuleName, _Out_writes_bytes_opt_(*pcbEatMap) PVMMDLL_MAP_EAT pEatMap, _Inout_ PDWORD pcbEatMap);

/*
* Retrieve the module imported functions from the import address table (IAT).
* If pIatMap is set to NULL the number of bytes required will be returned in
* parameter pcbIatMap.
* -- dwPID
* -- [uw]szModuleName
* -- pIatMap = buffer of minimum byte length *pcbIatMap or NULL.
* -- pcbIatMap = pointer to byte count of pIatMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetIATU(_In_ DWORD dwPID, _In_ LPSTR  uszModuleName, _Out_writes_bytes_opt_(*pcbIatMap) PVMMDLL_MAP_IAT pIatMap, _Inout_ PDWORD pcbIatMap);
_Success_(return) BOOL VMMDLL_Map_GetIATW(_In_ DWORD dwPID, _In_ LPWSTR wszModuleName, _Out_writes_bytes_opt_(*pcbIatMap) PVMMDLL_MAP_IAT pIatMap, _Inout_ PDWORD pcbIatMap);

/*
* Retrieve the heaps for the specified process. If pHeapMap is set to NULL
* the number of bytes required will be returned in parameter pcbHeapMap.
* -- dwPID
* -- pHeapMap = buffer of minimum byte length *pcbHeapMap or NULL.
* -- pcbHeapMap = pointer to byte count of pHeapMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetHeap(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbHeapMap) PVMMDLL_MAP_HEAP pHeapMap, _Inout_ PDWORD pcbHeapMap);

/*
* Retrieve the threads for the specified process. If pThreadMap is set to NULL
* the number of bytes required will be returned in parameter pcbThreadMap.
* Entries returned are sorted on VMMDLL_MAP_THREADENTRY.dwTID
* -- dwPID
* -- pThreadMap = buffer of minimum byte length *pcbThreadMap or NULL.
* -- pcbThreadMap = pointer to byte count of pThreadMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetThread(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbThreadMap) PVMMDLL_MAP_THREAD pThreadMap, _Inout_ PDWORD pcbThreadMap);

/*
* Retrieve the handles for the specified process. If pHandleMap is set to NULL
* the number of bytes required will be returned in parameter pcbHandleMap.
* Entries returned are sorted on VMMDLL_MAP_HANDLEENTRY.dwHandle
* -- dwPID
* -- pHandleMap = buffer of minimum byte length *pcbHandleMap or NULL.
* -- pcbHandleMap = pointer to byte count of pHandleMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetHandleU(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbHandleMap) PVMMDLL_MAP_HANDLE pHandleMap, _Inout_ PDWORD pcbHandleMap);
_Success_(return) BOOL VMMDLL_Map_GetHandleW(_In_ DWORD dwPID, _Out_writes_bytes_opt_(*pcbHandleMap) PVMMDLL_MAP_HANDLE pHandleMap, _Inout_ PDWORD pcbHandleMap);

/*
* Retrieve the physical memory ranges from the physical memory map that Windows
* have enumerated.
* -- pPhysMemMap = buffer of minimum byte length *pcbPhysMemMap or NULL.
* -- pcbPhysMemMap = pointer to byte count of pPhysMemMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetPhysMem(_Out_writes_bytes_opt_(*pcbPhysMemMap) PVMMDLL_MAP_PHYSMEM pPhysMemMap, _Inout_ PDWORD pcbPhysMemMap);

/*
* Retrieve the network connection map - consisting of active network connections,
* listening sockets and other networking functionality.
* -- pNetMap = buffer of minimum byte length *pcbNetMap or NULL.
* -- pcbNetMap = pointer to byte count of pNetrMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetNetU(_Out_writes_bytes_opt_(*pcbNetMap) PVMMDLL_MAP_NET pNetMap, _Inout_ PDWORD pcbNetMap);
_Success_(return) BOOL VMMDLL_Map_GetNetW(_Out_writes_bytes_opt_(*pcbNetMap) PVMMDLL_MAP_NET pNetMap, _Inout_ PDWORD pcbNetMap);

/*
* Retrieve the non well known users that are detected in the target system.
* NB! There may be more users in the system than the ones that are detected,
* only users with mounted registry hives may currently be detected - this is
* the normal behaviour for users with active processes.
* -- pUserMap = buffer of minimum byte length *pcbUserMap or NULL.
* -- pcbUserMap = pointer to byte count of pUserMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetUsersU(_Out_writes_bytes_opt_(*pcbUserMap) PVMMDLL_MAP_USER pUserMap, _Inout_ PDWORD pcbUserMap);
_Success_(return) BOOL VMMDLL_Map_GetUsersW(_Out_writes_bytes_opt_(*pcbUserMap) PVMMDLL_MAP_USER pUserMap, _Inout_ PDWORD pcbUserMap);

/*
* Retrieve the services currently known by the service control manager (SCM).
* -- pServiceMap = buffer of minimum byte length *pcbServiceMap or NULL.
* -- pcbServiceMap = pointer to byte count of pServiceMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_Map_GetServicesU(_Out_writes_bytes_opt_(*pcbServiceMap) PVMMDLL_MAP_SERVICE pServiceMap, _Inout_ PDWORD pcbServiceMap);
_Success_(return) BOOL VMMDLL_Map_GetServicesW(_Out_writes_bytes_opt_(*pcbServiceMap) PVMMDLL_MAP_SERVICE pServiceMap, _Inout_ PDWORD pcbServiceMap);



//-----------------------------------------------------------------------------
// WINDOWS SPECIFIC PAGE FRAME NUMBER (PFN) FUNCTIONALITY BELOW
//-----------------------------------------------------------------------------

#define VMMDLL_MAP_PFN_VERSION              1

static LPCSTR VMMDLL_PFN_TYPE_TEXT[] = { "Zero", "Free", "Standby", "Modifiy", "ModNoWr", "Bad", "Active", "Transit" };
static LPCSTR VMMDLL_PFN_TYPEEXTENDED_TEXT[] = { "-", "Unused", "ProcPriv", "PageTable", "LargePage", "DriverLock", "Shareable", "File" };

typedef enum tdVMMDLL_MAP_PFN_TYPE {
    VmmDll_PfnTypeZero = 0,
    VmmDll_PfnTypeFree = 1,
    VmmDll_PfnTypeStandby = 2,
    VmmDll_PfnTypeModified = 3,
    VmmDll_PfnTypeModifiedNoWrite = 4,
    VmmDll_PfnTypeBad = 5,
    VmmDll_PfnTypeActive = 6,
    VmmDll_PfnTypeTransition = 7
} VMMDLL_MAP_PFN_TYPE;

typedef enum tdVMMDLL_MAP_PFN_TYPEEXTENDED {
    VmmDll_PfnExType_Unknown = 0,
    VmmDll_PfnExType_Unused = 1,
    VmmDll_PfnExType_ProcessPrivate = 2,
    VmmDll_PfnExType_PageTable = 3,
    VmmDll_PfnExType_LargePage = 4,
    VmmDll_PfnExType_DriverLocked = 5,
    VmmDll_PfnExType_Shareable = 6,
    VmmDll_PfnExType_File = 7,
} VMMDLL_MAP_PFN_TYPEEXTENDED;

typedef struct tdVMMDLL_MAP_PFNENTRY {
    DWORD dwPfn;
    VMMDLL_MAP_PFN_TYPEEXTENDED tpExtended;
    struct {        // Only valid if active non-prototype PFN
        union {
            DWORD dwPid;
            DWORD dwPfnPte[5];  // PFN of paging levels 1-4 (x64)
        };
        QWORD va;               // valid if non-zero
    } AddressInfo;
    QWORD vaPte;
    QWORD OriginalPte;
    union {
        DWORD _u3;
        struct {
            WORD ReferenceCount;
            // MMPFNENTRY
            BYTE PageLocation       : 3;    // Pos 0  - VMMDLL_MAP_PFN_TYPE
            BYTE WriteInProgress    : 1;    // Pos 3
            BYTE Modified           : 1;    // Pos 4
            BYTE ReadInProgress     : 1;    // Pos 5
            BYTE CacheAttribute     : 2;    // Pos 6
            BYTE Priority           : 3;    // Pos 0
            BYTE Rom_OnProtectedStandby : 1;// Pos 3
            BYTE InPageError        : 1;    // Pos 4
            BYTE KernelStack_SystemChargedPage : 1; // Pos 5
            BYTE RemovalRequested   : 1;    // Pos 6
            BYTE ParityError        : 1;    // Pos 7
        };
    };
    union {
        QWORD _u4;
        struct {
            DWORD PteFrame;
            DWORD PteFrameHigh      : 4;    // Pos 32
            DWORD _Reserved         : 21;   // Pos 36
            DWORD PrototypePte      : 1;    // Pos 57
            DWORD PageColor         : 6;    // Pos 58
        };
    };
    DWORD _FutureUse[6];
} VMMDLL_MAP_PFNENTRY, *PVMMDLL_MAP_PFNENTRY;

typedef struct tdVMMDLL_MAP_PFN {
    DWORD dwVersion;
    DWORD _Reserved1[5];
    DWORD cMap;                     // # map entries.
    VMMDLL_MAP_PFNENTRY pMap[];     // map entries.
} VMMDLL_MAP_PFN, *PVMMDLL_MAP_PFN;

/*
* Retrieve information about scattered PFNs. The PFNs are returned in order of
* in which they are stored in the pPfns set.
* -- pPfns
* -- cPfns
* -- pPfnMap = buffer of minimum byte length *pcbPfnMap or NULL.
* -- pcbPfnMap = pointer to byte count of pPhysMemMap buffer.
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_Map_GetPfn(_In_ DWORD pPfns[], _In_ DWORD cPfns, _Out_writes_bytes_opt_(*pcbPfnMap) PVMMDLL_MAP_PFN pPfnMap, _Inout_ PDWORD pcbPfnMap);



//-----------------------------------------------------------------------------
// VMM PROCESS FUNCTIONALITY BELOW:
// Functionality below is mostly relating to Windows processes.
//-----------------------------------------------------------------------------

/*
* Retrieve an active process given it's name. Please note that if multiple
* processes with the same name exists only one will be returned. If required to
* parse all processes with the same name please iterate over the PID list by
* calling VMMDLL_PidList together with VMMDLL_ProcessGetInformation.
* -- szProcName = process name case insensitive.
* -- pdwPID = pointer that will receive PID on success.
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_PidGetFromName(_In_ LPSTR szProcName, _Out_ PDWORD pdwPID);

/*
* List the PIDs in the system.
* -- pPIDs = DWORD array of at least number of PIDs in system, or NULL.
* -- pcPIDs = size of (in number of DWORDs) pPIDs array on entry, number of PIDs in system on exit.
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_PidList(_Out_writes_opt_(*pcPIDs) PDWORD pPIDs, _Inout_ PULONG64 pcPIDs); 

#define VMMDLL_PROCESS_INFORMATION_MAGIC        0xc0ffee663df9301e
#define VMMDLL_PROCESS_INFORMATION_VERSION      6

typedef struct tdVMMDLL_PROCESS_INFORMATION {
    ULONG64 magic;
    WORD wVersion;
    WORD wSize;
    VMMDLL_MEMORYMODEL_TP tpMemoryModel;    // as given by VMMDLL_MEMORYMODEL_* enum
    VMMDLL_SYSTEM_TP tpSystem;              // as given by VMMDLL_SYSTEM_* enum
    BOOL fUserOnly;                         // only user mode pages listed
    DWORD dwPID;
    DWORD dwPPID;
    DWORD dwState;
    CHAR szName[16];
    CHAR szNameLong[64];
    ULONG64 paDTB;
    ULONG64 paDTB_UserOpt;                  // may not exist
    struct {
        ULONG64 vaEPROCESS;
        ULONG64 vaPEB;
        ULONG64 _Reserved1;
        BOOL fWow64;
        DWORD vaPEB32;                  // WoW64 only
        DWORD dwSessionId;
        ULONG64 qwLUID;
        CHAR szSID[MAX_PATH];
    } win;
} VMMDLL_PROCESS_INFORMATION, *PVMMDLL_PROCESS_INFORMATION;

/*
* Retrieve various process information from a PID. Process information such as
* name, page directory bases and the process state may be retrieved.
* -- dwPID
* -- pProcessInformation = if null, size is given in *pcbProcessInfo
* -- pcbProcessInformation = size of pProcessInfo (in bytes) on entry and exit
* -- return = success/fail.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_ProcessGetInformation(_In_ DWORD dwPID, _Inout_opt_ PVMMDLL_PROCESS_INFORMATION pProcessInformation, _In_ PSIZE_T pcbProcessInformation);

#define VMMDLL_PROCESS_INFORMATION_OPT_STRING_PATH_KERNEL           1
#define VMMDLL_PROCESS_INFORMATION_OPT_STRING_PATH_USER_IMAGE       2
#define VMMDLL_PROCESS_INFORMATION_OPT_STRING_CMDLINE               3

/*
* Retrieve a string value belonging to a process. The function allocates a new
* string buffer and returns the requested string in it. The string is always
* NULL terminated. On failure NULL is returned.
* NB! CALLER IS RESPONSIBLE FOR VMMDLL_MemFree return value!
* CALLER FREE: VMMDLL_MemFree(return)
* -- dwPID
* -- fOptionString = string value to retrieve as given by VMMDLL_PROCESS_INFORMATION_OPT_STRING_*
* -- return - fail: NULL, success: the string - NB! must be VMMDLL_MemFree'd by caller!
*/
EXPORTED_FUNCTION
LPSTR VMMDLL_ProcessGetInformationString(_In_ DWORD dwPID, _In_ DWORD fOptionString);

/*
* Retrieve information about: Data Directories, Sections, Export Address Table
* and Import Address Table (IAT).
* If the pData == NULL upon entry the number of entries of the pData array must
* have in order to be able to hold the data is returned.
* -- dwPID
* -- [uw]szModule
* -- pData
* -- cData
* -- pcData
* -- return = success/fail.
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_ProcessGetDirectoriesU(_In_ DWORD dwPID, _In_ LPSTR  uszModule, _Out_writes_(16) PIMAGE_DATA_DIRECTORY pData, _In_ DWORD cData, _Out_ PDWORD pcData);
_Success_(return) BOOL VMMDLL_ProcessGetDirectoriesW(_In_ DWORD dwPID, _In_ LPWSTR wszModule, _Out_writes_(16) PIMAGE_DATA_DIRECTORY pData, _In_ DWORD cData, _Out_ PDWORD pcData);
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_ProcessGetSectionsU(_In_ DWORD dwPID, _In_ LPSTR  uszModule, _Out_opt_ PIMAGE_SECTION_HEADER pData, _In_ DWORD cData, _Out_ PDWORD pcData);
_Success_(return) BOOL VMMDLL_ProcessGetSectionsW(_In_ DWORD dwPID, _In_ LPWSTR wszModule, _Out_opt_ PIMAGE_SECTION_HEADER pData, _In_ DWORD cData, _Out_ PDWORD pcData);

/*
* Retrieve the virtual address of a given function inside a process/module.
* -- dwPID
* -- [uw]szModuleName
* -- szFunctionName
* -- return = virtual address of function, zero on fail.
*/
EXPORTED_FUNCTION
ULONG64 VMMDLL_ProcessGetProcAddressU(_In_ DWORD dwPID, _In_ LPSTR  uszModuleName, _In_ LPSTR szFunctionName);
ULONG64 VMMDLL_ProcessGetProcAddressW(_In_ DWORD dwPID, _In_ LPWSTR wszModuleName, _In_ LPSTR szFunctionName);

/*
* Retrieve the base address of a given module.
* -- dwPID
* -- [uw]szModuleName
* -- return = virtual address of module base, zero on fail.
*/
EXPORTED_FUNCTION
ULONG64 VMMDLL_ProcessGetModuleBaseU(_In_ DWORD dwPID, _In_ LPSTR  uszModuleName);
ULONG64 VMMDLL_ProcessGetModuleBaseW(_In_ DWORD dwPID, _In_ LPWSTR wszModuleName);



//-----------------------------------------------------------------------------
// WINDOWS SPECIFIC DEBUGGING / SYMBOL FUNCTIONALITY BELOW:
//-----------------------------------------------------------------------------

/*
* Load a .pdb symbol file and return its associated module name upon success.
* -- dwPID
* -- vaModuleBase
* -- szModuleName = buffer to receive module name upon success.
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_PdbLoad(_In_ DWORD dwPID, _In_ ULONG64 vaModuleBase, _Out_writes_(MAX_PATH) LPSTR szModuleName);

/*
* Retrieve a symbol virtual address given a module name and a symbol name.
* NB! not all modules may exist - initially only module "nt" is available.
* NB! if multiple modules have the same name the 1st to be added will be used.
* -- szModule
* -- cbSymbolAddressOrOffset = symbol virtual address or symbol offset.
* -- szSymbolName = buffer to receive symbol name upon success.
* -- pdwSymbolDisplacement = displacement from the beginning of the symbol.
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_PdbSymbolName(_In_ LPSTR szModule, _In_ QWORD cbSymbolAddressOrOffset, _Out_writes_(MAX_PATH) LPSTR szSymbolName, _Out_opt_ PDWORD pdwSymbolDisplacement);

/*
* Retrieve a symbol virtual address given a module name and a symbol name.
* NB! not all modules may exist - initially only module "nt" is available.
* NB! if multiple modules have the same name the 1st to be added will be used.
* -- szModule
* -- szSymbolName
* -- pvaSymbolAddress
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_PdbSymbolAddress(_In_ LPSTR szModule, _In_ LPSTR szSymbolName, _Out_ PULONG64 pvaSymbolAddress);

/*
* Retrieve a type size given a module name and a type name.
* NB! not all modules may exist - initially only module "nt" is available.
* NB! if multiple modules have the same name the 1st to be added will be used.
* -- szModule
* -- szTypeName
* -- pcbTypeSize
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_PdbTypeSize(_In_ LPSTR szModule, _In_ LPSTR szTypeName, _Out_ PDWORD pcbTypeSize);

/*
* Locate the offset of a type child - typically a sub-item inside a struct.
* NB! not all modules may exist - initially only module "nt" is available.
* NB! if multiple modules have the same name the 1st to be added will be used.
* -- szModule
* -- uszTypeName
* -- uszTypeChildName
* -- pcbTypeChildOffset
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_PdbTypeChildOffset(_In_ LPSTR szModule, _In_ LPSTR uszTypeName, _In_ LPSTR uszTypeChildName, _Out_ PDWORD pcbTypeChildOffset);



//-----------------------------------------------------------------------------
// WINDOWS SPECIFIC REGISTRY FUNCTIONALITY BELOW:
//-----------------------------------------------------------------------------

#define VMMDLL_REGISTRY_HIVE_INFORMATION_MAGIC      0xc0ffee653df8d01e
#define VMMDLL_REGISTRY_HIVE_INFORMATION_VERSION    3

typedef struct td_VMMDLL_REGISTRY_HIVE_INFORMATION {
    ULONG64 magic;
    WORD wVersion;
    WORD wSize;
    BYTE _FutureReserved1[0x14];
    ULONG64 vaCMHIVE;
    ULONG64 vaHBASE_BLOCK;
    DWORD cbLength;
    CHAR uszName[128];
    CHAR uszNameShort[32 + 1];
    CHAR uszHiveRootPath[MAX_PATH];
    QWORD _FutureReserved[0x10];
} VMMDLL_REGISTRY_HIVE_INFORMATION, *PVMMDLL_REGISTRY_HIVE_INFORMATION;

/*
* Retrieve information about the registry hives in the target system.
* -- pHives = buffer of cHives * sizeof(VMMDLL_REGISTRY_HIVE_INFORMATION) to receive information about all hives. NULL to receive # hives in pcHives.
* -- cHives
* -- pcHives = if pHives == NULL: # total hives. if pHives: # read hives.
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_WinReg_HiveList(_Out_writes_(cHives) PVMMDLL_REGISTRY_HIVE_INFORMATION pHives, _In_ DWORD cHives, _Out_ PDWORD pcHives);

/*
* Read a contigious arbitrary amount of registry hive memory and report the
* number of bytes read in pcbRead.
* NB! Address space does not include regf registry hive file header!
* -- vaCMHive
* -- ra
* -- pb
* -- cb
* -- pcbReadOpt
* -- flags = flags as in VMMDLL_FLAG_*
* -- return = success/fail. NB! reads may report as success even if 0 bytes are
*        read - it's recommended to verify pcbReadOpt parameter.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_WinReg_HiveReadEx(_In_ ULONG64 vaCMHive, _In_ DWORD ra, _Out_ PBYTE pb, _In_ DWORD cb, _Out_opt_ PDWORD pcbReadOpt, _In_ ULONG64 flags);

/*
* Write a virtually contigious arbitrary amount of memory to a registry hive.
* NB! Address space does not include regf registry hive file header!
* -- vaCMHive
* -- ra
* -- pb
* -- cb
* -- return = TRUE on success, FALSE on partial or zero write.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_WinReg_HiveWrite(_In_ ULONG64 vaCMHive, _In_ DWORD ra, _In_ PBYTE pb, _In_ DWORD cb);

/*
* Enumerate registry sub keys - similar to WINAPI function 'RegEnumKeyExW.'
* Please consult WINAPI function documentation for information.
* May be called with HKLM base or virtual address of CMHIVE base examples:
*   1) 'HKLM\SOFTWARE\Key\SubKey'
*   2) 'HKLM\ORPHAN\SAM\Key\SubKey'              (orphan key)
*   3) '0x<vaCMHIVE>\ROOT\Key\SubKey'
*   4) '0x<vaCMHIVE>\ORPHAN\Key\SubKey'          (orphan key)
* -- uszFullPathKey
* -- dwIndex = sub-key index 0..N (-1 for key).
* -- lpName
* -- lpcchName
* -- lpftLastWriteTime
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_WinReg_EnumKeyExU(
    _In_ LPSTR uszFullPathKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(*lpcchName) LPSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Out_opt_ PFILETIME lpftLastWriteTime
);

/*
* Enumerate registry values given a registry key - similar to WINAPI function
* 'EnumValueW'. Please consult WINAPI function documentation for information.
* May be called in two ways:
* May be called with HKLM base or virtual address of CMHIVE base examples:
*   1) 'HKLM\SOFTWARE\Key\SubKey'
*   2) 'HKLM\ORPHAN\SAM\Key\SubKey'              (orphan key)
*   3) '0x<vaCMHIVE>\ROOT\Key\SubKey'
*   4) '0x<vaCMHIVE>\ORPHAN\Key\SubKey'          (orphan key)
* -- uszFullPathKey
* -- dwIndex
* -- lpValueName
* -- lpcchValueName
* -- lpType
* -- lpData
* -- lpcbData
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_WinReg_EnumValueU(
    _In_ LPSTR uszFullPathKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(*lpcchValueName) LPSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_opt_(*lpcbData) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
);

/*
* Query a registry value given a registry key/value path - similar to WINAPI
* function 'RegQueryValueEx'.
* Please consult WINAPI function documentation for information.
* May be called with HKLM base or virtual address of CMHIVE base examples:
*   1) 'HKLM\SOFTWARE\Key\SubKey\Value'
*   2) 'HKLM\ORPHAN\SAM\Key\SubKey\'             (orphan key and default value)
*   3) '0x<vaCMHIVE>\ROOT\Key\SubKey\Value'
*   4) '0x<vaCMHIVE>\ORPHAN\Key\SubKey\Value'    (orphan key value)
* -- uszFullPathKeyValue
* -- lpType
* -- lpData
* -- lpcbData
* -- return
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_WinReg_QueryValueExU(
    _In_ LPSTR uszFullPathKeyValue,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_opt_(*lpcbData) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
);

/*
* Enumerate registry sub keys - similar to WINAPI function 'RegEnumKeyExW.'
* Please consult WINAPI function documentation for information.
* May be called with HKLM base or virtual address of CMHIVE base examples:
*   1) 'HKLM\SOFTWARE\Key\SubKey'
*   2) 'HKLM\ORPHAN\SAM\Key\SubKey'              (orphan key)
*   3) '0x<vaCMHIVE>\ROOT\Key\SubKey'
*   4) '0x<vaCMHIVE>\ORPHAN\Key\SubKey'          (orphan key)
* -- wszFullPathKey
* -- dwIndex = sub-key index 0..N (-1 for key).
* -- lpName
* -- lpcchName
* -- lpftLastWriteTime
* -- return
*/
_Success_(return)
BOOL VMMDLL_WinReg_EnumKeyExW(
    _In_ LPWSTR wszFullPathKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(*lpcchName) LPWSTR lpName,
    _Inout_ LPDWORD lpcchName,
    _Out_opt_ PFILETIME lpftLastWriteTime
);

/*
* Enumerate registry values given a registry key - similar to WINAPI function
* 'EnumValueW'. Please consult WINAPI function documentation for information.
* May be called in two ways:
* May be called with HKLM base or virtual address of CMHIVE base examples:
*   1) 'HKLM\SOFTWARE\Key\SubKey'
*   2) 'HKLM\ORPHAN\SAM\Key\SubKey'              (orphan key)
*   3) '0x<vaCMHIVE>\ROOT\Key\SubKey'
*   4) '0x<vaCMHIVE>\ORPHAN\Key\SubKey'          (orphan key)
* -- wszFullPathKey
* -- dwIndex
* -- lpValueName
* -- lpcchValueName
* -- lpType
* -- lpData
* -- lpcbData
* -- return
*/
_Success_(return)
BOOL VMMDLL_WinReg_EnumValueW(
    _In_ LPWSTR wszFullPathKey,
    _In_ DWORD dwIndex,
    _Out_writes_opt_(*lpcchValueName) LPWSTR lpValueName,
    _Inout_ LPDWORD lpcchValueName,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_opt_(*lpcbData) LPBYTE lpData,
    _Inout_opt_ LPDWORD lpcbData
);

/*
* Query a registry value given a registry key/value path - similar to WINAPI
* function 'RegQueryValueEx'.
* Please consult WINAPI function documentation for information.
* May be called with HKLM base or virtual address of CMHIVE base examples:
*   1) 'HKLM\SOFTWARE\Key\SubKey\Value'
*   2) 'HKLM\ORPHAN\SAM\Key\SubKey\'             (orphan key and default value)
*   3) '0x<vaCMHIVE>\ROOT\Key\SubKey\Value'
*   4) '0x<vaCMHIVE>\ORPHAN\Key\SubKey\Value'    (orphan key value)
* -- wszFullPathKeyValue
* -- lpType
* -- lpData
* -- lpcbData
* -- return
*/
_Success_(return)
BOOL VMMDLL_WinReg_QueryValueExW(
    _In_ LPWSTR wszFullPathKeyValue,
    _Out_opt_ LPDWORD lpType,
    _Out_writes_opt_(*lpcbData) LPBYTE lpData,
    _When_(lpData == NULL, _Out_opt_) _When_(lpData != NULL, _Inout_opt_) LPDWORD lpcbData
);



//-----------------------------------------------------------------------------
// WINDOWS SPECIFIC UTILITY FUNCTIONS BELOW:
//-----------------------------------------------------------------------------

typedef struct tdVMMDLL_WIN_THUNKINFO_IAT {
    BOOL fValid;
    BOOL f32;               // if TRUE fn is a 32-bit/4-byte entry, otherwise 64-bit/8-byte entry.
    ULONG64 vaThunk;        // address of import address table 'thunk'.
    ULONG64 vaFunction;     // value if import address table 'thunk' == address of imported function.
    ULONG64 vaNameModule;   // address of name string for imported module.
    ULONG64 vaNameFunction; // address of name string for imported function.
} VMMDLL_WIN_THUNKINFO_IAT, *PVMMDLL_WIN_THUNKINFO_IAT;

/*
* Retrieve information about the import address table IAT thunk for an imported
* function. This includes the virtual address of the IAT thunk which is useful
* for hooking.
* -- dwPID
* -- [uw]szModuleName
* -- szImportModuleName
* -- szImportFunctionName
* -- pThunkIAT
* -- return
*/
EXPORTED_FUNCTION
_Success_(return) BOOL VMMDLL_WinGetThunkInfoIATU(_In_ DWORD dwPID, _In_ LPSTR  uszModuleName, _In_ LPSTR szImportModuleName, _In_ LPSTR szImportFunctionName, _Out_ PVMMDLL_WIN_THUNKINFO_IAT pThunkInfoIAT);
_Success_(return) BOOL VMMDLL_WinGetThunkInfoIATW(_In_ DWORD dwPID, _In_ LPWSTR wszModuleName, _In_ LPSTR szImportModuleName, _In_ LPSTR szImportFunctionName, _Out_ PVMMDLL_WIN_THUNKINFO_IAT pThunkInfoIAT);



//-----------------------------------------------------------------------------
// VMM UTIL FUNCTIONALITY BELOW:
//-----------------------------------------------------------------------------

/*
* Fill a human readable hex ascii memory dump into the caller supplied sz buffer.
* -- pb
* -- cb
* -- cbInitialOffset = offset, must be max 0x1000 and multiple of 0x10.
* -- sz = buffer to fill, NULL to retrieve buffer size in pcsz parameter.
* -- pcsz = IF sz==NULL :: size of buffer (including space for terminating NULL) on exit
*           IF sz!=NULL :: size of buffer on entry, size of characters (excluding terminating NULL) on exit.
*/
EXPORTED_FUNCTION _Success_(return)
BOOL VMMDLL_UtilFillHexAscii(
    _In_reads_opt_(cb) PBYTE pb,
    _In_ DWORD cb,
    _In_ DWORD cbInitialOffset,
    _Out_writes_opt_(*pcsz) LPSTR sz,
    _Inout_ PDWORD pcsz
);



//-----------------------------------------------------------------------------
// DEFAULT (WINDOWS ONLY) COMPATIBILITY FUNCTION DEFINITIONS BELOW:
//-----------------------------------------------------------------------------

#ifdef _WIN32
#define VMMDLL_VfsList                  VMMDLL_VfsListW
#define VMMDLL_VfsRead                  VMMDLL_VfsReadW
#define VMMDLL_VfsWrite                 VMMDLL_VfsWriteW
#define VMMDLL_ProcessGetDirectories    VMMDLL_ProcessGetDirectoriesW
#define VMMDLL_ProcessGetSections       VMMDLL_ProcessGetSectionsW
#define VMMDLL_ProcessGetProcAddress    VMMDLL_ProcessGetProcAddressW
#define VMMDLL_ProcessGetModuleBase     VMMDLL_ProcessGetModuleBaseW
#define VMMDLL_Map_GetPte               VMMDLL_Map_GetPteW
#define VMMDLL_Map_GetVad               VMMDLL_Map_GetVadW
#define VMMDLL_Map_GetModule            VMMDLL_Map_GetModuleW
#define VMMDLL_Map_GetModuleFromName    VMMDLL_Map_GetModuleFromNameW
#define VMMDLL_Map_GetUnloadedModule    VMMDLL_Map_GetUnloadedModuleW
#define VMMDLL_Map_GetEAT               VMMDLL_Map_GetEATW
#define VMMDLL_Map_GetIAT               VMMDLL_Map_GetIATW
#define VMMDLL_Map_GetHandle            VMMDLL_Map_GetHandleW
#define VMMDLL_Map_GetNet               VMMDLL_Map_GetNetW
#define VMMDLL_Map_GetUsers             VMMDLL_Map_GetUsersW
#define VMMDLL_Map_GetServices          VMMDLL_Map_GetServicesW
#define VMMDLL_WinGetThunkInfoIAT       VMMDLL_WinGetThunkInfoIATW
#endif /* _WIN32 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __VMMDLL_H__ */
