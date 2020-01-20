#include "io.h"

/*
 *inb() function => in argument declear 
 *port => port read byte
 *
*/
unsigned short inb(unsigned short port){
		unsigned short out;
		asm volatile (
			"inb %1, %0 \n\t"
			: "=ra"(out) : "id"(port)
		);
		return out;
}

/*
 *outb() function => in argument declear
 *@port => port number
 *@val => write bytes
*/
void outb(unsigned short port, unsigned short val){
	asm volatile (
		"outb %0, %1 \n\t" 
		::"ra"(val), "id"(port)
	);			
}

/*
 *inw() function => in argument declear
 *port => port read value
 *
*/
unsigned short inw(unsigned short port){
		unsigned short out;
		asm volatile (
			//inw(read) port_number, value
			"inw %1, %0 \n\t"
			: "=ra"(out) : "id"(port)
		);
		return out;
}

/*
 *outw() function => in argument declear
 *@port => port number
 *@val => write bytes
*/
void outw(unsigned short port, unsigned short val){
	asm volatile (
		//out(write) @value, @port_number
		"outw %0, %1 \n\t" 
		::"ra"(val), "id"(port)
	);			
}

//cli() => telling proccessor software and hardware interrupt are close/disable/off
void cli(){
	asm volatile ("cli \n\t");
}

//sti() => telling proccessor software and hardware interrupt are enable/start/intization
void sti(){
	asm volatile ("sti \n\t");
}


