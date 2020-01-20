extern unsigned short inb(unsigned short port);
extern void outb(unsigned short port, unsigned short val);
extern unsigned short inw(unsigned short port);
extern void outw(unsigned short port, unsigned short val);

extern void cli();
extern void sti();
extern void iret();
extern void push();
extern void pop();
