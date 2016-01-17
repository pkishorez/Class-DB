/**
 * @file lib_terminal.h
 * @author Kishore
 * @date June 27, 2015, 10:10 PM 
 */

#include "lib_types.h"
void terminal_save();
void terminal_restore();
void terminal_disable_echo();
void terminal_enable_echo();

void terminal_set_ncanonical();

void ansi_fg_bg(int fg, int bg);
void ansi_default();

inline void ansi_dim();
inline void ansi_bold();
inline void ansi_uline();
inline void ansi_reverse();
inline void ansi_cursor_hide();
inline void ansi_clear_line();

inline void ansi_cursor_right(int offset);
inline void ansi_cursor_left(int offset);
inline void ansi_cursor_up(int offset);
inline void ansi_cursor_down(int offset);


#define ansi_bold_text(...)\
	if (1){\
		ansi_bold();\
		printf(__VA_ARGS__);\
		ansi_default();\
	}

#define ansi_uline_text(...)\
	if (1){\
		ansi_uline();\
		printf(__VA_ARGS__);\
		ansi_default();\
	}

#define ansi_uline_bold_text(...)\
	{\
		ansi_bold();\
		ansi_uline();\
		printf(__VA_ARGS__);\
		ansi_default();\
	}

#define ansi_fg_bg_text(fg, bg, ...)\
	if (1){\
		ansi_fg_bg(fg,bg);\
		printf( __VA_ARGS__ );\
		ansi_default();\
		fflush(stdout);\
	}

#define ansi_fg_bg_text(fg, bg, ...)\
	if (1){\
		ansi_fg_bg(fg,bg);\
		printf( __VA_ARGS__ );\
		ansi_default();\
		fflush(stdout);\
	}


#define ansi_fg_bg_text_ncursor(fg, bg, ...)\
	if (1){\
		ansi_fg_bg(fg,bg);\
		ansi_cursor_left(printf( __VA_ARGS__ ));\
		ansi_default();\
		fflush(stdout);\
	}

#define ansi_fg_bg_textln(a, b, ...)\
	if (1){\
		ansi_fg_bg(a,b);\
		printf( __VA_ARGS__ );\
		ansi_default();\
		printf("\n");\
		fflush(stdout);\
	}

#define ansi_error(...)\
	if (1){\
		ansi_bold();\
		ansi_fg_bg_text(C_RED, C_DEFAULT, "Error : %s:%d\n    ", __FILE__, __LINE__);\
		ansi_fg_bg_text(C_CYAN, C_DEFAULT,__VA_ARGS__);\
		fflush(stdout);\
	}

#define ansi_redtext(...)\
	if (1){\
		ansi_fg_bg_text(C_RED, C_DEFAULT, __VA_ARGS__);\
		fflush(stdout);\
	}

#define ansi_success(...)\
	if (1){\
		ansi_bold();\
		ansi_fg_bg_text(C_GREEN, C_DEFAULT, "\nSuccess : " __VA_ARGS__);\
		fflush(stdout);\
	}
