#include "io.h"
#include "idt.h"

//create a basic structure who arrange and manage every entery in idt table
struct idt_entry {
	unsigned short low;
	unsigned short sel;
	unsigned char res;
	unsigned char type;
	unsigned short high;
} __attribute__ ((packed)); // __attribute__ ((packed)) => Telling the compiler to not optimize and generate the structure as it is. 
struct idt_entry idt_e;

// Below is our IDT table
// Every element/entry in it is 8 bytes long
// For details, Check "idt_entry" structure definition.
//reserved 2048 byte memory size
struct idt_entry idt[256];

// IDTR => 48 bits data which will be provided to processor
// Processor will store this 48 bits in its internal storage/registers
// This will be used by processessor in determining the address of idt table and size of it.
struct idt_r {
	unsigned short size;
	unsigned int addr;
}  __attribute__ ((packed));
struct idt_r idtr;
	
/**
  * This function will create IDT table.
  * By default, common_isr() will be used in each entry.
  * The ISR address could be updated in future by calling @attach_isr_addr(int isr2, int int_num) function.
  */
void create_idt(){
     //long long entry;
     int isr_addr = (int)&common_isr;
     long long isr_addr_low = isr_addr & (int)0xffff;
     long long isr_addr_high = isr_addr >> 16;
     
     idt_e.low = (short)isr_addr_low;
     idt_e.high = (short)isr_addr_high;
     idt_e.res = (char) 0;
     idt_e.type = (char) 0x8e;
     idt_e.sel = (short) 0x8;
     //entry =   isr_addr_low | (long long)0x08 << 16 | (long long)0x00 << 32 | (long long)0x8e << 40 | isr_addr_high << 48;    
       
     int x = 0;
     while (x < 256){
	      idt[x] = idt_e;
	      x++;
	}
	
	// Updating IDTR
	// Setting size and address of IDT table in IDTR
	// 256 (entries) * 8 (bytes per entry) = 2048 Bytes
	idtr.size = (short)2048;
	idtr.addr = (unsigned int)(&idt); 
	//idtr = (short)sizeof(idt) | (int)idt << 16;
	
	// Disabling hardware interrupts
	cli();
	// Telling the processor to use our IDT table
	attach_idt_table();
	// Enabling hardware interrupts
	sti();
}

//Tells processor to use our own IDT table for interrupt servicing.
void attach_idt_table(){
	asm volatile ("lidt %0 \n"::[idtr]"m"(idtr));
}

/**
  * function to attach isr address to interrupt number
  *	@isr2 ISR Address
  * @int_num interrupt number
  */
void attach_isr_addr(int isr2, int int_num){
	 //isr address are partible in high and low byte and store in variable 	
	 short isr_addr_low = isr2 & (int)0xffff;
     short isr_addr_high = isr2 >> 16;
     
     //isr address low and high byte store in idt_table 
     idt[int_num].low = isr_addr_low;
     idt[int_num].high = isr_addr_high;
     
}

/**
  * Common ISR (Interrupt handler) for all interrupts
  */
__attribute__ ((naked)) // Telling the compiler to not optimize this function and generate the assemble used inside as it is.
void common_isr(){
	// Disabling hardware interrupts and pushing all registers to stack.
	asm volatile ("cli \n\t" "pusha \n\t");
		
	// Sending acknowledgement to Master and Slave PIC Chip.
	outb(0x20, 0x20);
	outb(0xa0, 0x20);
	
	// Popping all registers from stack
	// and, generating iret instruction to tell the processor that we successfully handled the interrupt request.
	// Processor will return from here and continue executing whatever program was active before this interrupt occurred.
	asm volatile (
					"popa \n\t"
					"iret \n\t"
					);
}

