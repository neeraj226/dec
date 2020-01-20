extern void common_isr();
extern void create_idt();
extern void attach_isr_addr(int isr2, int int_num);
extern void attach_idt_table();
