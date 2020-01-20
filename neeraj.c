#include "io.h"
#include "screen.h"
#include "idt.h"
#include "cmos.h"

void show_welcome();
void show_error();
void kbd();
void timer();
void _start(){
	show_welcome();
	show_error();
	create_idt();
    
    //cli();
	attach_isr_addr((int)kbd, 9);
	attach_isr_addr((int)timer, 8);
	attach_isr_addr((int)rtc_isr, 0x70); 
	cmos_int_enable();
	
	//sti();
	for(;;);
}

//msg[] variable in store value print on screen.
void show_welcome(){
	char msg[] = "Welcome to c";
	prints(30, 1, msg, 0xb9);
}

//telling to compilar when program execute than all errer's and fault and warning show on screen.
void show_error(){
	char msg[] = "error";
	prints(30, 2, msg, 0xb1);
}

//Telling the compiler to not optimize this function and generate the assemble used inside as it is.     
__attribute__ ((naked))
void kbd(){
	//keyboard device interrupt stop
	asm volatile ("cli \n\t" "pusha \n\t");
	
	*(short *)0xb8000 = 0xb934;
	
	unsigned short c = (inb(0x60) & 0x0f) + '0';
	*(short *)0xb8020 = 0xb9 << 8 | c;
	
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
	
	asm volatile (
					"popa \n\t"
					"iret \n\t"
					);
}

__attribute__ ((naked))
void timer(){
	asm volatile ("cli \n\t" "pusha \n\t");
	
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
	
	asm volatile (
					"popa \n\t"
					"iret \n\t"
					);
}






