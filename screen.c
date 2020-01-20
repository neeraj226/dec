#include "screen.h"

/** 
  * prints() => in declear argumant
  * int h => column number (screen position)
  * int line => row number (screen position)
  * char ch => value 
  * char color => value color 
*/
  void prints(int h, int line, char* str, char color){
	short *vga = (short *)(0xb8000 + (--h*2) + (--line*80*2));
	while(*(str) != 0){
		*(vga++) = color << 8 | *(str++);
	}
}


/** 
  * putc() => in declear argumant
  * int h => column number (screen position)
  * int line => row number (screen position)
  * char ch => value 
  * char color => value color 
*/
void putc(int h, int line, char ch, char color){
	short *vga = (short *)(0xb8000 + (--h*2) + (--line*80*2));
	*vga = color << 8 | ch;
}
