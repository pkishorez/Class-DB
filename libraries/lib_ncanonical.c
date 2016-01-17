/**
 * @file lib_ncanonical.c
 * @author Kishore
 * @date January 5, 2016, 3:13 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/headers.h"
#include "headers/lib_ncanonical.h"
#include "headers/lib_terminal.h"


static struct{
	char ch[1024*10];
	int len;
	int pos;
}buffer = {
	.len = 0,
	.pos = 0
};

#define HISTORY_LIMIT 10
#define HISTORY_TEXT_SIZE 256
char history[HISTORY_LIMIT][HISTORY_TEXT_SIZE];
int history_index = 0;

static int up_down_left_right(int key);
static int up_down_left_right(int key)
{
	switch(key){
		case 65:
			return CHAR_UP;
		case 66:
			return CHAR_DOWN;
		case 67:
			return CHAR_RIGHT;
		case 68:
			return CHAR_LEFT;
		default:
			return 0;
	}
}

/**
 * Emties the buffer used for storing string entered
 * in non canonical mode.
 */
void ncanonical_emptybuffer()
{
	buffer.ch[0] = '\0';
	buffer.len = 0;
	buffer.pos = 0;
}

/**
 * Gets the buffer used for storing string entered
 * in non canonical mode.
 * @returns string.
 */
char* ncanonical_getbuffer()
{
	return buffer.ch;
}

/**
 * Used to place the cursor at the end of line.
 */
void ncanonical_put_cursor_end()
{
	buffer.pos = buffer.len;
	ncanonical_print();
}

/**
 * Appends the string to the current command mode.
 * @param txt : The string that needs to be appended.
 * @param movecursor : boolean value which decides whether to move
 * cursor along the text or remain in same position.
 */
void ncanonical_append(char *txt, int movecursor)
{
	int len, i;
	for (len=0; txt[len]!='\0' && isprint(txt[len]); len++);
	
	for (i=0; i<len; i++)
		buffer.ch[buffer.len + i] = txt[i];
	buffer.len += len;
	if (movecursor)
		buffer.pos = buffer.len;
	ncanonical_print();
}

/**
 * Inserts string in command mode at the place of cursor.
 * @param txt : Text that needs to be inserted at cursor position.
 * @param movecursor : boolean value to decide whether to move
 * cursor or not along the txt.
 */
void ncanonical_insert(char *txt, int movecursor)
{
	int len, i;
	for (len=0; txt[len]!='\0' && isprint(txt[len]); len++);
	
	for (i=buffer.len; i>=buffer.pos; i--)
		buffer.ch[i+len]=buffer.ch[i];
	for (i=0; i<len; i++)
		buffer.ch[buffer.pos + i] = txt[i];
	buffer.len += len;
	
	if (movecursor)buffer.pos += len;
	ncanonical_print();
}


/**
 * Used to read the character from file descriptor in ncanonical mode.
 * @param fd : file descriptor from which to read.
 * @param position : whether to move the cursor position if 
 * arrow keys are pressed.
 * @returns keystroke_t structure that defines the key pressed.
 */
keystroke_t ncanonical_read_save(int fd, int position)
{
	if (buffer.len==0)
		ansi_cursor_save();
	keystroke_t key = ncanonical_read(fd);
	if (key.ch==CHAR_LEFT && position){
		if (buffer.pos!=0)
			buffer.pos-=1;
	}
	else if (key.ch==CHAR_RIGHT && position){
		if (buffer.pos!=buffer.len)
			buffer.pos+=1;
	}
	else if (key.ch==CHAR_UP && position){
		// To implement history.
	}
	else if (key.ch==CHAR_HOME){
		buffer.pos = 0;
	}
	else if (key.ch==CHAR_END){
		buffer.pos = buffer.len;
	}
	else if (key.ch==CHAR_BACKSPACE && position){
		if (buffer.pos!=0){
			buffer.pos-=1;
			int i;
			for (i=buffer.pos; i<buffer.len; i++){
				buffer.ch[i]=buffer.ch[i+1];
			}
			buffer.len--;
		}
	}
	else if (key.ch==CHAR_DELETE && position){
		if (buffer.pos<buffer.len){
			int i;
			for (i=buffer.pos; i<buffer.len; i++)
				buffer.ch[i] = buffer.ch[i+1];
			buffer.len--;
		}
	}
	else if (isprint(key.ch)){
		int i;
		for (i=buffer.len-1; i>=buffer.pos; i--){
			buffer.ch[i+1] = buffer.ch[i];
		}
		buffer.ch[buffer.pos++] = key.ch;
		buffer.len++;
		buffer.ch[buffer.len] = '\0';
	}
	return key;
}


/**
 * Prints the prompt used in command mode.
 */
void ncanonical_prompt(void)
{
	ansi_bold_text("> ");
	ansi_cursor_save();
	ncanonical_print();
}

/**
 * Prints the string entered in ncanonical mode so far.
 */
void ncanonical_print(void)
{
	ansi_cursor_restore();
	ansi_clear_line();
	int i;
	//printf("BUFFER LENGTH : %d, pos : %d\n", buffer.len, buffer.pos);
	for (i=0; i<buffer.len; i++)
		printf("%c", buffer.ch[i]);
	ansi_cursor_left(buffer.len - buffer.pos);
	fflush(stdout);
}

/**
 * Reads the key pressed, save in buffer and prints the same.
 * @param fd : file descriptor where key input is stored.
 * @returns keystroke_t structure that defines the key pressed.
 */
keystroke_t ncanonical_read_save_print(int fd)
{
	keystroke_t key = ncanonical_read_save(fd, 1);
	ncanonical_print();
	return key;
}


/**
 * Reads the key pressed in ncanonical mode.
 * @param fd : file descriptor where key press is read.
 * @returns keystroke_t structure that defines the key pressed.
 */
keystroke_t ncanonical_read(int fd)
{
	char buf[10];
	int r = read(fd, buf, 10);
	keystroke_t key = {
		.ch        = 0,
		.ctrl      = 0,
		.isspecial = 0,
		.shift     = 0,
		.alt       = 0
	};
	
	switch(r)
	{
		case 1 : {
			key.ch = buf[0];
			switch(key.ch){
				case CHAR_TAB :
				case CHAR_ENTER :
				case CHAR_BACKSPACE:
				case CHAR_ESCAPE:{
					key.isspecial = 1;
					break;
				}
			}
			return key;
			break;
		}
		case 3 : {
			switch(buf[0])
			{
				case CHAR_ESCAPE:{
					if(buf[1]==91){
						key.isspecial = 1;
						key.ch = up_down_left_right(buf[2]);
						return key;
					}
					else if (buf[1]==79){
						key.isspecial = 1;
						switch(buf[2]){
							case 72:
								key.ch = CHAR_HOME;
								return key;
							case 70:
								key.ch = CHAR_END;
								return key;
							case 81:
								key.ch = CHAR_F2;
								return key;
							case 82:
								key.ch = CHAR_F3;
							default:
								return key;
						}
					}
					break;
				}
			}
			break;
		}
		case 4:{
			if (buf[0]==27 && buf[1]==91 && buf[2]==51 && buf[3]==126){
				key.isspecial = 1;
				key.ch = CHAR_DELETE;
			}
			break;
		}
		case 6:{
			if (buf[0]==27 && buf[1]==91 && buf[2]==49 && buf[3]==59 && buf[4]==53){
				key.ctrl = 1;
				key.isspecial = 1;
				key.ch = up_down_left_right(buf[5]);
				return key;
			}
			if (buf[0]==27 && buf[1]==91 && buf[2]==49 && buf[3]==59 && buf[4]==50){
				key.shift = 1;
				key.isspecial = 1;
				key.ch = up_down_left_right(buf[5]);
				return key;
			}
			if (buf[0]==27 && buf[1]==91 && buf[2]==49 && buf[3]==59 && buf[4]==51){
				key.alt = 1;
				key.isspecial = 1;
				key.ch = up_down_left_right(buf[5]);
				return key;
			}
			if (buf[0]==27 && buf[1]==91 && buf[2]==49 && buf[3]==59 && buf[4]==52){
				key.shift = 1;
				key.alt = 1;
				key.isspecial = 1;
				key.ch = up_down_left_right(buf[5]);
				return key;
			}
			if (buf[0]==27 && buf[1]==91 && buf[2]==49 && buf[3]==59 && buf[4]==54){
				key.shift = 1;
				key.ctrl = 1;
				key.isspecial = 1;
				key.ch = up_down_left_right(buf[5]);
				return key;
			}
		}
	}
	return key;
}


void ncanonical_printkey(keystroke_t key)
{
	if (key.ctrl)
		printf("CTRL + ");
	if (key.shift)
		printf("SHIFT + ");
	if (key.alt)
		printf("ALT + ");
	ansi_bold();
	if (key.isspecial){
		if (key.ch==CHAR_BACKSPACE)
			printf("BACKSPACE");
		else if (key.ch==CHAR_DOWN)
			printf("DOWN");
		else if (key.ch==CHAR_END)
			printf("END");
		else if (key.ch==CHAR_ENTER)
			printf("ENTER");
		else if (key.ch==CHAR_ESCAPE)
			printf("ESCAPE");
		else if (key.ch==CHAR_HOME)
			printf("HOME");
		else if (key.ch==CHAR_LEFT)
			printf("LEFT");
		else if (key.ch==CHAR_RIGHT)
			printf("RIGHT");
		else if (key.ch==CHAR_TAB)
			printf("TAB");
		else if (key.ch==CHAR_UP)
			printf("UP");
		else if (key.ch==CHAR_F2)
			printf("F2");
		else if (key.ch==CHAR_F3)
			printf("F3");
		else
			printf("UNKNOWN");
	}
	else{
		if (key.ch==0)
			printf("UNKNOWN");
		else
			printf("%c", key.ch);
	}
	ansi_default();
	fflush(stdout);
}