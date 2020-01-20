#include "idt.h"
#include "io.h"
#include "cmos.h"
#include "screen.h"

//declear global variable and store in cmos register values  
int second;
int minute;
int hours;
int week;
int day;
int month;
int year;
int century;

/*
 *feching cmos registers values 
 *cmos values store in variable
*/
void data_ret(){
	outb(0x70,0x00);
	second = inb(0x71);

	outb(0x70,0x02);
	minute = inb(0x71);
	 
	outb(0x70,0x04);
	hours = inb(0x71);

	outb(0x70,0x06);
  	week = inb(0x71);
   
    outb(0x70,0x07);
  	day = inb(0x71);
   
    outb(0x70,0x08);
	month = inb(0x71);
   
    outb(0x70,0x09);
  	year = inb(0x71);

	outb(0x70,0x32);
  	century = inb(0x71);      
}

/* 
 *cmos_int_enable() => enable cmos interrupt 
 * 
*/
void cmos_int_enable(){
	cli();
	outb(0x70,0x8b);
	//0x8b cmos register in 7th value (0 to 1) change
	unsigned short b = inb(0x71) | 0x40;
	
	outb(0x70,0x8b);
	outb(0x71, b); 
	
	sti();
}

/*
 *rtc_data_print() => global variable value print on screen
 *
*/

void rtc_data_print(){
    putc(58, 1,  ((char)hours >> 4) + '0',   0xb9 );
	putc(59, 1,  ((char)hours & 0x0f) + '0',   0xb9 );
	putc(60, 1,  ':',  0xb1 );
	
	putc(61, 1,  ((char)minute >> 4) + '0',  0xb9 );
	putc(62, 1,  ((char)minute & 0x0f) + '0',  0xb9 );
	putc(63, 1,  ':',  0xb1 );
	
	putc(64, 1,  ((char)second >> 4) + '0',  0xb9 );
	putc(65, 1,  ((char)second & 0x0f) + '0',  0xb9 );
	putc(66, 1,  ' ',  0xb1 );

	putc(67, 1,  ((char)day >> 4) + '0',     0xb9 );
	putc(68, 1,  ((char)day & 0x0f) + '0',     0xb9 );
	putc(69, 1,  '/',  0xb1 );

	putc(70, 1,  ((char)month >> 4) + '0',   0xb9 );
	putc(71, 1,  ((char)month & 0x0f) + '0',   0xb9 );
	putc(72, 1,  '/',  0xb1 );

	putc(73, 1,  ((char)century >> 4) + '0', 0xb9 );
	putc(74, 1,  ((char)century & 0x0f) + '0', 0xb9 );
	putc(76, 1,  ((char)year & 0x0f) + '0',    0xb9 );
	putc(75, 1,  ((char)year >> 4) + '0',    0xb9 );
	
}

//cmos starting register 
int rtc_count = 0;
__attribute__ ((naked))
void rtc_isr(){
	cli();
	asm volatile ("pusha \n\t");
	rtc_count++;
	
	/*
	 *intization if condition
	 * 
	 *
	*/
	 
	if(rtc_count >= 1024){
		rtc_count = 0;
		data_ret();
		rtc_data_print();
	}
	
	outb(0x70,0xC);
	inb(0x71);
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
	asm volatile ("popa \n\t");
	//rtc_isr() function are end here....
	asm volatile ("iret \n\t");
}	



