// atd.fileop.cpp
#include "atd.h"
#include <shellapi.h>
#define FOF_NO_UI	(FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR)
using namespace atd;
//====================================================
//= atd::fileop
//====================================================
struct fileop::book fileop::book;
DWORD fileop::operation(const string &from, const string &to, UINT func)
{
	struct { string from, to; } path = 
	{
		from + "\1\1", 
		to   + "\1\1", 
	};
	string *ppp[10] = { &path.from, &path.to };
	for (string **pp = ppp; *pp; pp++)
	{
		string &s = **pp;
		for (string::iterator i = s.begin(), e = s.end(); i != e; ++i)
		{
			if (*i == '\1') *i = '\0';
		}
	}
	
	SHFILEOPSTRUCT fos = {0};
	fos.wFunc = func;
	fos.pFrom = &path.from[0];
	fos.pTo   = &path.to  [0];
	fos.fFlags = FOF_ALLOWUNDO | FOF_NO_UI;

	return ::SHFileOperation(&fos);
}
DWORD fileop::move(const string &from, const string &to)
{
	return operation(from, to, FO_MOVE);
}
DWORD fileop::copy(const string &from, const string &to)
{
	return operation(from, to, FO_COPY);
}
DWORD fileop::remove(const string &from)
{
	return operation(from, "", FO_DELETE);
}
//====================================================
//= atd::fileop::book
//====================================================
fileop::book::book()
{
	//http://msdn.microsoft.com/ja-jp/library/windows/desktop/bb762164(v=vs.85).aspx
	generic::status::primitive todo[100] = 
	{
		//Error Code				Value		Meaning
		{/*DE_SAMEFILE				*/0x71		, "The source and destination files are the same file."}, 
		{/*DE_MANYSRC1DEST			*/0x72		, "Multiple file paths were specified in the source buffer, but only one destination file path."}, 
		{/*DE_DIFFDIR				*/0x73		, "Rename operation was specified but the destination path is a different directory. Use the move operation instead."}, 
		{/*DE_ROOTDIR				*/0x74		, "The source is a root directory, which cannot be moved or renamed."}, 
		{/*DE_OPCANCELLED			*/0x75		, "The operation was canceled by the user, or silently canceled if the appropriate flags were supplied to SHFileOperation."}, 
		{/*DE_DESTSUBTREE			*/0x76		, "The destination is a subtree of the source."}, 
		{/*DE_ACCESSDENIEDSRC		*/0x78		, "Security settings denied access to the source."}, 
		{/*DE_PATHTOODEEP			*/0x79		, "The source or destination path exceeded or would exceed MAX_PATH."}, 
		{/*DE_MANYDEST				*/0x7A		, "The operation involved multiple destination paths, which can fail in the case of a move operation."}, 
		{/*DE_INVALIDFILES			*/0x7C		, "The path in the source or destination or both was invalid."}, 
		{/*DE_DESTSAMETREE			*/0x7D		, "The source and destination have the same parent folder."}, 
		{/*DE_FLDDESTISFILE			*/0x7E		, "The destination path is an existing file."}, 
		{/*DE_FILEDESTISFLD			*/0x80		, "The destination path is an existing folder."}, 
		{/*DE_FILENAMETOOLONG		*/0x81		, "The name of the file exceeds MAX_PATH."}, 
		{/*DE_DEST_IS_CDROM			*/0x82		, "The destination is a read-only CD-ROM, possibly unformatted."}, 
		{/*DE_DEST_IS_DVD			*/0x83		, "The destination is a read-only DVD, possibly unformatted."}, 
		{/*DE_DEST_IS_CDRECORD		*/0x84		, "The destination is a writable CD-ROM, possibly unformatted."}, 
		{/*DE_FILE_TOO_LARGE		*/0x85		, "The file involved in the operation is too large for the destination media or file system."}, 
		{/*DE_SRC_IS_CDROM			*/0x86		, "The source is a read-only CD-ROM, possibly unformatted."}, 
		{/*DE_SRC_IS_DVD			*/0x87		, "The source is a read-only DVD, possibly unformatted."}, 
		{/*DE_SRC_IS_CDRECORD		*/0x88		, "The source is a writable CD-ROM, possibly unformatted."}, 
		{/*DE_ERROR_MAX				*/0xB7		, "MAX_PATH was exceeded during the operation."}, 
		{/*							*/0x402		, "An unknown error occurred. This is typically due to an invalid path in the source or destination. This error does not occur on Windows Vista and later."}, 
		{/*ERRORONDEST				*/0x10000	, "An unspecified error occurred on the destination."}, 
		{/*DE_ROOTDIR | ERRORONDEST	*/0x10074	, "Destination is a root directory and cannot be renamed."}, 
	};
	append(todo);
}
