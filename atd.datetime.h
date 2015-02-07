//atd.datetime.h
#ifndef __ATD_DATETIME_H__
#define __ATD_DATETIME_H__
#include "atd.h"
namespace atd {;
namespace root {;
//====================================================
//= atd::root::::datetime
//====================================================
struct datetime : public object, public SYSTEMTIME
{
	short &year			;//WORD wYear;
	short &month		;//WORD wMonth;
	short &dayofweek	;//WORD wDayOfWeek;
	short &day			;//WORD wDay;
	short &hour			;//WORD wHour;
	short &minute		;//WORD wMinute;
	short &second		;//WORD wSecond;
	short &milliseconds	;//WORD wMilliseconds;
	datetime(
		  short year		= 0
		, short month		= 0
		, short dayofweek	= 0
		, short day			= 0
		, short hour		= 0
		, short minute		= 0
		, short second		= 0
		, short milliseconds= 0
	);
};
}//namespace base
//====================================================
//= atd::datetime
//====================================================
struct datetime : public root::datetime
{
	datetime();
	datetime(const SYSTEMTIME &that);
	datetime(const FILETIME &that);
	datetime(const struct tm &that);
	datetime(
		  short year
		, short month
		, short day
		, short hour		= 0
		, short minute		= 0
		, short second		= 0
		, short milliseconds= 0
	);
	datetime &copy(const SYSTEMTIME &that);
	datetime &copy(const FILETIME &that);
	datetime &copy(const struct tm &that);
	datetime &adjust();
	datetime &mktime();
	datetime &localize();
	FILETIME  filetime() const;
	struct tm tm() const;
	datetime date() const;
	datetime time() const;
	string strftime(const string &format = "%Y-%m-%d %H:%M:%S") const;
	/*
	%a	現在のロケールにおける曜日の省略名。
	%A	現在のロケールにおける曜日の完全な名前。
	%b	現在のロケールにおける月の省略名。
	%B	現在のロケールにおける月の完全な名前。
	%c	現在のロケールにおいて一般的な日付・時刻の表記。
	%C	世紀 (西暦年の上 2 桁)。 (SU)
	%d	月内通算日 (10 進数表記) (01-31)。
	%D	%m/%d/%y と等価。(うえっ、アメリカ専用だ。アメリカ以外の国では %d/%m/%y の方が一般的だ。紛らわしいので、使用すべきではない。) (SU)
	%e	%d と同様に月内通算日を 10 進数で表現するが、 1 桁の場合 10 の位にゼロを置かずスペースを置く。(SU)
	%E	別形式を使用する際の修飾子。下記参照。 (SU)
	%F	%Y-%m-%d と等価 (ISO 8601 形式の日付フォーマット)。 (C99)
	%G	ISO 8601 週単位表記の年 (week-based year; 「注意」の節を参照)。 世紀も 10 進数で表す。 ISO 週番号 (%V を参照) に対応した 4 桁の西暦年。 これは基本的には %Y と同じ形式だが、ISO 週数が前年や翌年になる 場合にはその年が使用される点が異なる。(TZ)
	%g	%G と同様。但し、世紀を含まず下 2 桁のみを表示 (00-99)。 (TZ)
	%h	%b と等価 (SU)
	%H	24 時間表記での時 (hour)。 (00-23)
	%I	12 時間表記での時 (hour)。 (01-12)
	%j	年の初めから通算の日数。 (001-366)
	%k	24 時間表記での時 (0-23)。 1 桁の場合には前にゼロでなくスペースが置かれる。 (%H も参照) (TZ)
	%l	12 時間表記での時 (0-12)。 1 桁の場合には前にゼロでなくスペースが置かれる。 (%I も参照) (TZ)
	%m	月 (10 進数表記)。 (01-12)
	%M	分 (10 進数表記) (00-59)
	%n	改行。 (SU)
	%O	別形式を使用する際の修飾子。下記参照。 (SU)
	%p	現在のロケールにおける「午前」「午後」に相当する文字列。 英語の場合には "AM" または "PM" となる。 正午は「午後」、真夜中は「午前」として扱われる。
	%P	%p と同様であるが小文字が使用される。 英語の場合には "am" や "pm" となる。(GNU)
	%r	午前・午後形式での時刻。 POSIX ロケールでは %I:%M:%S %p と等価である。(SU)
	%R	24 時間表記での時刻、秒は表示しない (%H:%M)。 秒を含んだものは以下の %T を参照すること。(SU)
	%s	紀元 (Epoch; 1970-01-01 00:00:00 +0000 (UTC)) からの秒数。 (TZ)
	%S	秒 (10 進数表記) (00-60) (時々ある閏秒に対応するため、値の範囲は 60 までとなっている)
	%t	タブ文字 (SU)
	%T	24 時間表記の時間 (%H:%M:%S) (SU)
	%u	週の何番目の日 (10 進数表記) か。月曜日を 1 とする (1-7)。 %w も参照。(SU)
	%U	年の初めからの通算の週番号 (10 進数表記) (00-53)。 その年の最初の日曜日を、第 1 週の始まりとして計算する。 %V と %W も参照すること。
	%V	ISO 8601 形式での年の始めからの週番号 (「注意」の節を参照)。 10 進数表記で、01 から 53 の値となる。週番号は、 新しい年が少なくとも 4 日以上含まれる最初の週を 1 として計算する。 %U と %W も参照のこと。(SU)
	%w	週の何番目の日 (10 進数表記) か。日曜日を 0 とする。(0-6)。 %u も参照。(SU)
	%W	年の初めからの通算の週番号 (10 進数表記) (00-53)。 その年の最初の月曜日を、第 1 週の始まりとして計算する。
	%x	現在のロケールで一般的な日付表記。時刻は含まない。
	%X	現在のロケールで一般的な時刻表記。日付は含まない。
	%y	西暦の下2桁 (世紀部分を含まない年) (00-99)。
	%Y	世紀部分を含めた ( 4 桁の) 西暦年。
	%z	+hhmm や -hhmm の形式のタイムゾーン (UTC へのオフセット時間)。(SU)
	%Z	タイムゾーン名または省略名。
	%+	date(1) 形式での日時。(TZ) (glibc2 ではサポートされていない)
	%%	'%' 文字。
	*/
	string dayofweek_jp() const;
	string yyyymmdd(const string &glue = "") const;
	string   yymmdd(const string &glue = "") const;
	string   hhmmss(const string &glue = "") const;
	string format(const string &format, bool short_year = false, bool dayofweek = false) const;
	string format_of_time(const string &format) const;
};
}//namespace atd
#endif//__ATD_DATETIME_H__