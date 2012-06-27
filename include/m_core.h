/*

Miranda IM: the free IM client for Microsoft* Windows*

Copyright 2000-2008 Miranda ICQ/IM project,
all portions of this codebase are copyrighted to the people
listed in contributors.txt.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef M_CORE_H__
#define M_CORE_H__ 1

#ifdef MIR_CORE_EXPORTS
	#define MIR_CORE_DLL(T) __declspec(dllexport) T __cdecl
#else
	#define MIR_CORE_DLL(T) __declspec(dllimport) T __cdecl
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

///////////////////////////////////////////////////////////////////////////////
// events, hooks & services

#define MAXMODULELABELLENGTH 64

typedef int (*MIRANDAHOOK)(WPARAM, LPARAM);
typedef int (*MIRANDAHOOKPARAM)(WPARAM, LPARAM, LPARAM);
typedef int (*MIRANDAHOOKOBJ)(void*, WPARAM, LPARAM);
typedef int (*MIRANDAHOOKOBJPARAM)(void*, WPARAM, LPARAM, LPARAM);

typedef INT_PTR (*MIRANDASERVICE)(WPARAM, LPARAM);
typedef INT_PTR (*MIRANDASERVICEPARAM)(WPARAM, LPARAM, LPARAM);
typedef INT_PTR (*MIRANDASERVICEOBJ)(void*, LPARAM, LPARAM);
typedef INT_PTR (*MIRANDASERVICEOBJPARAM)(void*, WPARAM, LPARAM, LPARAM);

#ifdef _WIN64
	#define CALLSERVICE_NOTFOUND      ((INT_PTR)0x8000000000000000)
#else
	#define CALLSERVICE_NOTFOUND      ((int)0x80000000)
#endif

MIR_CORE_DLL(HANDLE) CreateHookableEvent(const char *name);
MIR_CORE_DLL(int)    DestroyHookableEvent(HANDLE hEvent);
MIR_CORE_DLL(int)    SetHookDefaultForHookableEvent(HANDLE hEvent, MIRANDAHOOK pfnHook);
MIR_CORE_DLL(int)    CallPluginEventHook(HINSTANCE hInst, HANDLE hEvent, WPARAM wParam, LPARAM lParam);
MIR_CORE_DLL(int)    CallHookSubscribers(HANDLE hEvent, WPARAM wParam, LPARAM lParam);
MIR_CORE_DLL(int)    NotifyEventHooks(HANDLE hEvent, WPARAM wParam, LPARAM lParam);

MIR_CORE_DLL(HANDLE) HookEvent(const char* name, MIRANDAHOOK hookProc);
MIR_CORE_DLL(HANDLE) HookEventParam(const char* name, MIRANDAHOOKPARAM hookProc, LPARAM lParam);
MIR_CORE_DLL(HANDLE) HookEventObj(const char* name, MIRANDAHOOKOBJ hookProc, void* object);
MIR_CORE_DLL(HANDLE) HookEventObjParam(const char* name, MIRANDAHOOKOBJPARAM hookProc, void* object, LPARAM lParam);
MIR_CORE_DLL(HANDLE) HookEventMessage(const char* name, HWND hwnd, UINT message);
MIR_CORE_DLL(int)    UnhookEvent(HANDLE hHook);
MIR_CORE_DLL(void)   KillObjectEventHooks(void* pObject);
MIR_CORE_DLL(void)   KillModuleEventHooks(HINSTANCE pModule);

MIR_CORE_DLL(HANDLE) CreateServiceFunction(const char *name, MIRANDASERVICE serviceProc);
MIR_CORE_DLL(HANDLE) CreateServiceFunctionParam(const char *name, MIRANDASERVICEPARAM serviceProc, LPARAM lParam);
MIR_CORE_DLL(HANDLE) CreateServiceFunctionObj(const char *name, MIRANDASERVICEOBJ serviceProc, void* object);
MIR_CORE_DLL(HANDLE) CreateServiceFunctionObjParam(const char *name, MIRANDASERVICEOBJPARAM serviceProc, void* object, LPARAM lParam);
MIR_CORE_DLL(int)    DestroyServiceFunction(HANDLE hService);
MIR_CORE_DLL(int)    ServiceExists(const char *name);

MIR_CORE_DLL(INT_PTR) CallService(const char *name, WPARAM wParam, LPARAM lParam);
MIR_CORE_DLL(INT_PTR) CallServiceSync(const char *name, WPARAM wParam, LPARAM lParam);

MIR_CORE_DLL(int)     CallFunctionAsync(void (__stdcall *func)(void *), void *arg);
MIR_CORE_DLL(void)    KillModuleServices(HINSTANCE hInst);
MIR_CORE_DLL(void)    KillObjectServices(void* pObject);

#if defined(_STATIC)
__declspec(dllexport) INT_PTR CallContactService(HANDLE, const char *, WPARAM, LPARAM);
__declspec(dllexport) INT_PTR CallProtoService(const char *szModule, const char *szService, WPARAM wParam, LPARAM lParam);
#else
MIR_CORE_DLL(INT_PTR) CallContactService(HANDLE, const char *, WPARAM, LPARAM);
MIR_CORE_DLL(INT_PTR) CallProtoService(const char *szModule, const char *szService, WPARAM wParam, LPARAM lParam);
#endif

///////////////////////////////////////////////////////////////////////////////
// exceptions

typedef DWORD (__cdecl *pfnExceptionFilter)(DWORD code, EXCEPTION_POINTERS* info);

MIR_CORE_DLL(pfnExceptionFilter) GetExceptionFilter(void);
MIR_CORE_DLL(pfnExceptionFilter) SetExceptionFilter(pfnExceptionFilter pMirandaExceptFilter);

///////////////////////////////////////////////////////////////////////////////
// language packs support

#define LANG_UNICODE 0x1000

extern int hLangpack;

MIR_CORE_DLL(void)   LangPackDropUnusedItems(void);
MIR_CORE_DLL(int)    LangPackGetDefaultCodePage( void );
MIR_CORE_DLL(int)    LangPackGetDefaultLocale(void);
MIR_CORE_DLL(TCHAR*) LangPackPcharToTchar(const char* pszStr);

MIR_CORE_DLL(int)    LoadLangPackModule(void);
MIR_CORE_DLL(int)    LoadLangPack(const TCHAR *szLangPack);
MIR_CORE_DLL(void)   ReloadLangpack(TCHAR *pszStr);

MIR_CORE_DLL(char*)  TranslateA_LP(const char* str, int hLang);
MIR_CORE_DLL(WCHAR*) TranslateW_LP(const WCHAR* str, int hLang);
MIR_CORE_DLL(void)   TranslateMenu_LP(HMENU, int hLang);
MIR_CORE_DLL(void)   TranslateDialog_LP(HWND hDlg, int hLang);

#define Translate(s) TranslateA_LP(s, hLangpack)
#define TranslateW(s) TranslateW_LP(s, hLangpack)
#define TranslateMenu(h) TranslateMenu_LP(h,hLangpack)
#define TranslateDialogDefault(h) TranslateDialog_LP(h,hLangpack)

#ifdef _UNICODE
	#define TranslateT(s)	 TranslateW_LP(_T(s),hLangpack)
	#define TranslateTS(s)	 TranslateW_LP(s,hLangpack)
	#define TranslateTH(l,s) TranslateW_LP(s,l)
#else
	#define TranslateT(s)	 TranslateA_LP(s,hLangpack)
	#define TranslateTS(s)	 TranslateA_LP(s,hLangpack)
	#define TranslateTH(l,s) TranslateA_LP(s,l)
#endif

MIR_CORE_DLL(unsigned int) mir_hash(const void * key, unsigned int len);

#pragma optimize("gt", on)
__inline unsigned int mir_hashstr(const char * key)
{
	if (key == NULL) return 0;
	else {
		unsigned int len = (unsigned int)strlen((const char*)key);
		return mir_hash(key, len);
}	}

__inline unsigned int mir_hashstrW(const wchar_t * key)
{
	if (key == NULL) return 0;
	else {
		unsigned int len = (unsigned int)wcslen((const wchar_t*)key);
		return mir_hash(key, len * sizeof(wchar_t));
}	}
#pragma optimize("", on)

#define mir_hashstrT mir_hashstrW

///////////////////////////////////////////////////////////////////////////////
// lists

typedef int (*FSortFunc)(void*, void*);  // sort function prototype

// Assumes first 32 bit value of the data is the numeric key
// and uses it to perform sort/search operations, this results
// in much better performance as no compare function calls needed
// Incredibly useful for Hash Tables
#define NumericKeySort (FSortFunc)(void*) -1
#define HandleKeySort  (FSortFunc)(void*) -2
#define PtrKeySort     (FSortFunc)(void*) -3

typedef struct
{
	void**		items;
	int			realCount;
	int			limit;
	int			increment;

	FSortFunc	sortFunc;
}
	SortedList;

MIR_CORE_DLL(SortedList*) List_Create(int p_limit, int p_increment);
MIR_CORE_DLL(void)        List_Destroy(SortedList* p_list);
MIR_CORE_DLL(void*)       List_Find(SortedList* p_list, void* p_value);
MIR_CORE_DLL(int)         List_GetIndex(SortedList* p_list, void* p_value, int* p_index);
MIR_CORE_DLL(int)         List_IndexOf(SortedList* p_list, void* p_value);
MIR_CORE_DLL(int)         List_Insert(SortedList* p_list, void* p_value, int p_index);
MIR_CORE_DLL(int)         List_InsertPtr(SortedList* list, void* p);
MIR_CORE_DLL(int)         List_Remove(SortedList* p_list, int index);
MIR_CORE_DLL(int)         List_RemovePtr(SortedList* list, void* p);
MIR_CORE_DLL(void)        List_Copy(SortedList* s, SortedList* d, size_t itemSize);
MIR_CORE_DLL(void)        List_ObjCopy(SortedList* s, SortedList* d, size_t itemSize);

///////////////////////////////////////////////////////////////////////////////
// md5 functions

/* Define the state of the MD5 Algorithm. */
typedef unsigned char mir_md5_byte_t; /* 8-bit byte */
typedef unsigned int mir_md5_word_t; /* 32-bit word */

typedef struct mir_md5_state_s {
	mir_md5_word_t count[2];  /* message length in bits, lsw first */
	mir_md5_word_t abcd[4];    /* digest buffer */
	mir_md5_byte_t buf[64];    /* accumulate block */
} mir_md5_state_t;

MIR_CORE_DLL(void) mir_md5_init(mir_md5_state_t *pms);
MIR_CORE_DLL(void) mir_md5_append(mir_md5_state_t *pms, const mir_md5_byte_t *data, int nbytes);
MIR_CORE_DLL(void) mir_md5_finish(mir_md5_state_t *pms, mir_md5_byte_t digest[16]);
MIR_CORE_DLL(void) mir_md5_hash(const mir_md5_byte_t *data, int len, mir_md5_byte_t digest[16]);

///////////////////////////////////////////////////////////////////////////////
// memory functions

MIR_CORE_DLL(void*)  mir_alloc(size_t);
MIR_CORE_DLL(void*)  mir_calloc(size_t);
MIR_CORE_DLL(void*)  mir_realloc(void* ptr, size_t);
MIR_CORE_DLL(void)   mir_free(void* ptr);
MIR_CORE_DLL(char*)  mir_strdup(const char* str);
MIR_CORE_DLL(WCHAR*) mir_wstrdup(const WCHAR* str);
MIR_CORE_DLL(char*)  mir_strndup(const char* str, size_t len);

///////////////////////////////////////////////////////////////////////////////
// modules

MIR_CORE_DLL(void) RegisterModule(HINSTANCE hInst);
MIR_CORE_DLL(void) UnregisterModule(HINSTANCE hInst);

MIR_CORE_DLL(HINSTANCE) GetInstByAddress(void* codePtr);

///////////////////////////////////////////////////////////////////////////////
// path utils

MIR_CORE_DLL(void)   CreatePathToFile(char* wszFilePath);
MIR_CORE_DLL(int)    CreateDirectoryTree(const char* szDir);
MIR_CORE_DLL(int)    PathToAbsolute(const char *pSrc, char *pOut, char* base);
MIR_CORE_DLL(int)    PathToRelative(const char *pSrc, char *pOut);

MIR_CORE_DLL(void)   CreatePathToFileW(WCHAR* wszFilePath);
MIR_CORE_DLL(int)    CreateDirectoryTreeW(const WCHAR* szDir);
MIR_CORE_DLL(int)    PathToAbsoluteW(const WCHAR *pSrc, WCHAR *pOut, WCHAR* base);
MIR_CORE_DLL(int)    PathToRelativeW(const WCHAR *pSrc, WCHAR *pOut);

#define CreatePathToFileT CreatePathToFileW
#define CreateDirectoryTreeT CreateDirectoryTreeW
#define PathToAbsoluteT PathToAbsoluteW
#define PathToRelativeT PathToRelativeW

///////////////////////////////////////////////////////////////////////////////
// print functions

MIR_CORE_DLL(int)    mir_snprintf(char *buffer, size_t count, const char* fmt, ...);
MIR_CORE_DLL(int)    mir_sntprintf(TCHAR *buffer, size_t count, const TCHAR* fmt, ...);
MIR_CORE_DLL(int)    mir_vsnprintf(char *buffer, size_t count, const char* fmt, va_list va);
MIR_CORE_DLL(int)    mir_vsntprintf(TCHAR *buffer, size_t count, const TCHAR* fmt, va_list va);

///////////////////////////////////////////////////////////////////////////////
// sha1 functions

typedef unsigned char mir_sha1_byte_t;
typedef unsigned long mir_sha1_long_t;

#define MIR_SHA1_HASH_SIZE 20

typedef struct {
  mir_sha1_long_t H[5];
  mir_sha1_long_t W[80];
  int lenW;
  mir_sha1_long_t sizeHi, sizeLo;
} mir_sha1_ctx;

MIR_CORE_DLL(void) mir_sha1_init(mir_sha1_ctx *ctx);
MIR_CORE_DLL(void) mir_sha1_append(mir_sha1_ctx *ctx, mir_sha1_byte_t *dataIn, int len);
MIR_CORE_DLL(void) mir_sha1_finish(mir_sha1_ctx *ctx, mir_sha1_byte_t hashout[20]);
MIR_CORE_DLL(void) mir_sha1_hash(mir_sha1_byte_t *dataIn, int len, mir_sha1_byte_t hashout[20]);

///////////////////////////////////////////////////////////////////////////////
// strings

MIR_CORE_DLL(char*)  rtrim(char *str);
MIR_CORE_DLL(WCHAR*) wrtrim(WCHAR *str);

#ifdef _UNICODE
	#define trtrim wrtrim
#else
	#define trtrim rtrim
#endif

MIR_CORE_DLL(char*) ltrim(char *str);   // returns pointer to the beginning of string
MIR_CORE_DLL(char*) ltrimp(char *str);  // returns pointer to the trimmed portion of string

MIR_CORE_DLL(int)   wildcmp(char *name, char *mask);

__inline char* lrtrim(char* str) { return ltrim(rtrim(str)); };
__inline char* lrtrimp(char* str) { return ltrimp(rtrim(str)); };

///////////////////////////////////////////////////////////////////////////////
// text conversion functions

#ifdef _UNICODE
	#define mir_t2a(s) mir_u2a(s)
	#define mir_a2t(s) mir_a2u(s)
	#define mir_t2u(s) mir_wstrdup(s)
	#define mir_u2t(s) mir_wstrdup(s)

	#define mir_t2a_cp(s,c) mir_u2a_cp(s,c)
	#define mir_a2t_cp(s,c) mir_a2u_cp(s,c)
	#define mir_t2u_cp(s,c) mir_wstrdup(s)
	#define mir_u2t_cp(s,c) mir_wstrdup(s)

	#define mir_tstrdup mir_wstrdup
#else
	#define mir_t2a(s) mir_strdup(s)
	#define mir_a2t(s) mir_strdup(s)
	#define mir_t2u(s) mir_a2u(s)
	#define mir_u2t(s) mir_u2a(s)

	#define mir_t2a_cp(s,c) mir_strdup(s)
	#define mir_a2t_cp(s,c) mir_strdup(s)
	#define mir_t2u_cp(s,c) mir_a2u_cp(s,c)
	#define mir_u2t_cp(s,c) mir_u2a_cp(s,c)

	#define mir_tstrdup mir_strdup
#endif

MIR_CORE_DLL(WCHAR*) mir_a2u_cp(const char* src, int codepage);
MIR_CORE_DLL(WCHAR*) mir_a2u(const char* src);
MIR_CORE_DLL(char*)  mir_u2a_cp(const wchar_t* src, int codepage);
MIR_CORE_DLL(char*)  mir_u2a(const wchar_t* src);

#if defined(__cplusplus)

class _A2T
{
	TCHAR* buf;

public:
	__forceinline _A2T(const char* s) : buf(mir_a2t(s)) {}
	__forceinline _A2T(const char* s, int cp) : buf(mir_a2t_cp(s, cp)) {}
	~_A2T() { mir_free(buf); }

	__forceinline operator TCHAR*() const
	{	return buf;
	}
};

class _T2A
{
	char* buf;

public:
	__forceinline _T2A(const TCHAR* s) : buf(mir_t2a(s)) {}
	__forceinline _T2A(const TCHAR* s, int cp) : buf(mir_t2a_cp(s, cp)) {}
	__forceinline ~_T2A() { mir_free(buf); }

	__forceinline operator char*() const
	{	return buf;
	}
};

#endif

///////////////////////////////////////////////////////////////////////////////
// threads

typedef void (__cdecl *pThreadFunc)(void*);
typedef unsigned (__stdcall *pThreadFuncEx)(void*);
typedef unsigned (__cdecl *pThreadFuncOwner)(void *owner, void* param);

MIR_CORE_DLL(INT_PTR) UnwindThreadPush(WPARAM wParam, LPARAM lParam);
MIR_CORE_DLL(INT_PTR) UnwindThreadPop(WPARAM, LPARAM);
MIR_CORE_DLL(void)    UnwindThreadWait(void);

MIR_CORE_DLL(UINT_PTR) forkthread(pThreadFunc, unsigned long stacksize, void *arg);
MIR_CORE_DLL(UINT_PTR) forkthreadex(void *sec, unsigned stacksize, pThreadFuncEx, void* owner, void *arg, unsigned *thraddr);

_inline HANDLE mir_forkthread(pThreadFunc aFunc, void* arg)
{	return (HANDLE)forkthread(aFunc, 0, arg);
}

__inline HANDLE mir_forkthreadex(pThreadFuncEx aFunc, void* arg, int stackSize, unsigned* pThreadID)
{	return (HANDLE)forkthreadex(NULL, stackSize, aFunc, NULL, arg, pThreadID);
}

__inline HANDLE mir_forkthreadowner(pThreadFuncOwner aFunc, void* owner, void* arg, unsigned* pThreadID)
{	return (HANDLE)forkthreadex(NULL, 0, (pThreadFuncEx)aFunc, owner, arg, pThreadID);
}

MIR_CORE_DLL(void) KillObjectThreads(void* pObject);

///////////////////////////////////////////////////////////////////////////////
// utf8 interface

MIR_CORE_DLL(char*) Utf8Decode(char* str, wchar_t** ucs2);
MIR_CORE_DLL(char*) Utf8DecodeCP(char* str, int codepage, wchar_t** ucs2);

MIR_CORE_DLL(wchar_t*) Utf8DecodeW(const char* str);

MIR_CORE_DLL(char*) Utf8Encode(const char* str);
MIR_CORE_DLL(char*) Utf8EncodeCP(const char* src, int codepage);

MIR_CORE_DLL(char*) Utf8EncodeW(const wchar_t* str);

MIR_CORE_DLL(int)   Ucs2toUtf8Len(const wchar_t *src);

#define Utf8DecodeT Utf8DecodeW
#define Utf8EncodeT Utf8EncodeW

#define mir_utf8decode(A, B)      Utf8Decode(A, B)
#define mir_utf8decodecp(A, B, C) Utf8DecodeCP(A, B, C)
#define mir_utf8decodeW(A)   	    Utf8DecodeW(A)
#define mir_utf8encode(A)         Utf8Encode(A)
#define mir_utf8encodecp(A, B)    Utf8EncodeCP(A, B)
#define mir_utf8encodeW(A)        Utf8EncodeW(A)
#define mir_utf8lenW(A)           Ucs2toUtf8Len(A)

__forceinline char* mir_utf8decodeA(const char* src)
{
    char* tmp = mir_strdup(src);
    mir_utf8decode(tmp, NULL);
    return tmp;
}

#if defined(_UNICODE)
	#define mir_utf8decodeT mir_utf8decodeW
	#define mir_utf8encodeT mir_utf8encodeW
#else
	#define mir_utf8decodeT mir_utf8decodeA
	#define mir_utf8encodeT mir_utf8encode
#endif

///////////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
}
#endif

#ifndef MIR_CORE_EXPORTS
	#if !defined( WIN64 )
		#pragma comment(lib, "mir_core.lib")
	#else
		#pragma comment(lib, "mir_core64.lib")
	#endif
#endif

#endif // M_CORE_H
