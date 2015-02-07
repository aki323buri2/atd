//atd.macro.h
#ifndef __ATD_MACRO_H__
#define __ATD_MACRO_H__
#include "atd.h"

#define EMPTY 					(-1)
#define BETWEEN(a, b, c)		((a) <= (b) && (b) <= (c))

#define CRLF					"\r\n"
#define TAB						"\t"
#define SPACE					" "
#define WSPACE					"　"

#define EXCLAMATION_MARK		"!"
#define QUESTION_MARK			"?"
#define SINGLE_QUOTATION_MARK	"'"
#define DOUBLE_QUOTATION_MARK	"\""
#define NUMERICAL_SIGN			"#"//、HASH_MARK
#define DOLLARS_SIGN			"$"
#define YEN_SIGN				"\\"
#define PERCENT					"%"
#define AMPERSAND				"&"
#define LPAREN					"("//、LPARENTHESIS
#define RPAREN					")"//、RPARENTHESIS
#define CARET					"＾"//、CIRCUMFLEX
#define TILDE					"~"
#define VERTICAL_BAR			"|"
#define AT_SIGN					"@"
#define BACK_APOSTROPHE			"`"
#define LBRACKET				"["
#define RBRACKET				"]"
#define LBRACE					"{"
#define RBRACE					"}"
#define ASTERISK				"*"
#define SEMICOLON				";"
#define COLON					":"
#define COMMA					","
#define PERIOD					"."//、DOT、POINT
#define LESS_THAN_SIGN			"<"
#define GRATER_THAN_SIGN		">"
#define SLASH					"/"
#define LOW_LINE				"_"//、UNDERSCORE
#define EQUAL					"="
#define MINUS_SIGN				"-"//、HYPHEN、DASH
#define PLUS_SIGN				"+"

#define EXCLAMATION			EXCLAMATION_MARK		//"!"
#define QUESTION			QUESTION_MARK			//"?"
#define SINGLE_QUOTATION	SINGLE_QUOTATION_MARK	//"'"
#define DOUBLE_QUOTATION	DOUBLE_QUOTATION_MARK	//"\""
#define NUMERICAL			NUMERICAL_SIGN			//"#"//、HASH_MARK
#define DOLLARS				DOLLARS_SIGN			//"$"
#define YEN					YEN_SIGN				//"\\"
#define AT					AT_SIGN					//"@"
#define LESS_THAN			LESS_THAN_SIGN			//"<"
#define GRATER_THAN			GRATER_THAN_SIGN		//">"
#define MINUS				MINUS_SIGN				//"-"//、HYPHEN、DASH
#define PLUS				PLUS_SIGN				//"+"

#define HASH_MARK		NUMERICAL_SIGN	//"#"
#define LPARENTHESIS	LPAREN			//"("
#define RPARENTHESIS	RPAREN			//")"
#define CIRCUMFLEX		CARET			//"＾"
#define DOT				PERIOD			//"."、POINT
#define UNDERSCORE		LOW_LINE		//"_"
#define HYPHEN			MINUS_SIGN		//"-"、DASH
#define DASH			HYPHEN			//"-"

#endif//__ATD_MACRO_H__