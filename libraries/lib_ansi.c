/**
 * @file lib_ansi.c
 * @author Kishore
 * @date September 28, 2015, 10:12 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/headers.h"
#include "headers/lib_terminal.h"

/**************************************************************
 * Ansi related functions goes here. The following functions  *
 * are only print related functions where printing a sequence *
 * brings about a change in the way the terminal behaves.     *
 **************************************************************/

void ansi_default()
{
	printf("\033[0m");
}


inline void ansi_fg_bg(int fg, int bg)
{
	printf("\033[3%dm\033[4%dm", fg, bg);
}




inline void ansi_dim()
{
	printf("\033[2m");
}


inline void ansi_uline()
{
	printf("\033[4m");
}


inline void ansi_bold()
{
	printf("\033[1m");
}


inline void ansi_reverse()
{
	printf("\033[7m");
}


inline void ansi_cursor_hide()
{
	printf("\033[?25l");
}


inline void ansi_cursor_show()
{
	printf("\033[?25h");
}


inline void ansi_cursor_save()
{
	printf("\033[s");
}


inline void ansi_cursor_restore()
{
	printf("\033[u");
}

inline void ansi_clear_line()
{
	printf("\033[J");
}

inline void ansi_screen_save()
{
	printf("\033[?47h");
}


inline void ansi_screen_restore()
{
	printf("\033[?47l");
}

inline void ansi_cursor_down(int offset)
{
	printf("\033[%dB", offset);
}

inline void ansi_cursor_up(int offset)
{
	printf("\033[%dA", offset);
}

inline void ansi_cursor_left(int offset)
{
	if (offset>0)
		printf("\033[%dD", offset);
}

inline void ansi_cursor_right(int offset)
{
	printf("\033[%dC", offset);
}

inline void ansi_text(int fg_color, int bg_color, int bold, int underline, int nline, char *text)
{
	ansi_default();
	ansi_fg_bg(fg_color, bg_color);
	if (bold)
		ansi_bold();
	if (underline)
		ansi_uline();
	printf("%s", text);
	ansi_default();
	if (nline)
		printf("\n");
}