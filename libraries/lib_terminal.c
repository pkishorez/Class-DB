/**
 * @file lib_terminal.c
 * @author Kishore
 * @date June 27, 2015, 10:09 PM
 * 
 * @brief Adds terminal related functionality.
 */

#include "headers/headers.h"
#include "headers/lib_utilities.h"
#include "headers/lib_terminal.h"


static struct termios save;

/**
 * Saves the state of terminal which could be restored
 * using restore function
 */
void terminal_save()
{
	assert(tcgetattr(STDIN_FILENO, &save) != -1);
}

/**
 * Restores the terminal session to the one saved before.
 */
void terminal_restore()
{
	assert(tcsetattr(STDIN_FILENO, TCSANOW, &save) != -1);
}

/**
 * Disables echo of strings on screen to key strokes.
 */
void terminal_disable_echo()
{
	struct termios tp;
	assert(tcgetattr(STDIN_FILENO, &tp) != -1);
	
	tp.c_lflag &= ~ECHO;

	assert(tcsetattr(STDIN_FILENO, TCSANOW, &tp) != -1);
}

/**
 * Enable echo of strings on screen to key strokes.
 */
void terminal_enable_echo()
{
	struct termios tp;
	assert(tcgetattr(STDIN_FILENO, &tp) != -1);

	tp.c_lflag |= ECHO;

	assert(tcsetattr(STDIN_FILENO, TCSANOW, &tp) != -1);
}

/**
 * Set non canonical mode of terminal which disables
 * buffering of input and provides input real time.
 */
void terminal_set_ncanonical()
{
	struct termios tp;
	
	assert(tcgetattr(STDIN_FILENO, &tp) != -1);
	tp.c_lflag &= ~ICANON;
	
	tp.c_cc[VMIN] = 1;
	tp.c_cc[VTIME] = 0;
	assert(tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) != -1);
}