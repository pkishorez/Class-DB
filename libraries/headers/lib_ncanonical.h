/**
 * @file ncanonical.h
 * @author Kishore
 * @date January 5, 2016, 3:15 PM 
 */

#include "lib_types.h"

void ncanonical_emptybuffer();
char* ncanonical_getbuffer();
void ncanonical_append(char *txt, int movecursor);
void ncanonical_insert(char *txt, int movecursor);
void ncanonical_put_cursor_end();
keystroke_t ncanonical_read(int fd);
keystroke_t ncanonical_read_save(int fd, int position);
keystroke_t ncanonical_read_save_print(int fd);
void ncanonical_printkey(keystroke_t key);
void ncanonical_print(void);
void ncanonical_prompt(void);
void ncanonical_buffer_stats();