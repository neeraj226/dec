# This is an MBR bootloader
# Bios will load the first 512 bytes of generated binary code of this file.
# This file contains code of first and second stage bootloader.
# a

BITS 16
org 0x7c00
       
start:
    mov si,msg
    call print 
    
    mov ah,2
    mov al, 40
    mov cx, 2
    mov dx, 0
    mov bx, (0x7c00 + 512)
    int 0x13
           
    lgdt [GDTR]
    mov eax,cr0
    or eax,1
    mov cr0,eax
    
    cli
    jmp 0x8:PMODE

      
print:
      mov ah,0Eh
      mov al,[si]
      int 10h
      add si, 1
      cmp al,0
      jne print 
      ret
      

   msg db 'welcome to real mode.'
   
; -------
; Global Descriptor Table
; -------

GDT_START:
; Null Descriptor
     dd 0
     dd 0    
; Code Descriptot
	dw 0xFFFF
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0
; Data  Descriptor
	dw 0xFFFF		; Segment Limit 0-15 bits
	dw 0			; Base Address 0-15 bits
	db 0			; Base Address 16-23 bits
	db 10010010b	; Access Flags - Type, S, DPL, P
	db 11001111b	; High 4 - Flags, Low 4 - Segment Limit 16-20
	db 0			; Base Address - 24 - 31 bits
GDT_END:
; -------------
; GDT Register - To be loaded in Processor
; -------------

GDTR:
	dw GDT_END - GDT_START - 1
	dd GDT_START
  
times 510-($-$$) db 0
dw 0xaa55
    
BITS 32

idt:
    times 2048 db 0
    
PMODE:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov bp, 0xffff
	mov sp, bp
	
	mov edi, 0xb8000
	mov byte [edi],'a'
	mov byte [edi+1],0xb9
	
	mov eax,ISR 
	mov bx,ax
	shr eax,16
	mov dx,ax

	mov edi, 0
	call fill_idt
	
	lidt [IDT_R]
	;sti
	
	int 32
	
	mov eax,[0x7c00 + 3072 + 0x18]
	lea ebx, [0x7c00 + 3072 + eax]
	call ebx
	
	mov edi, 0xb8010
	mov byte [edi],'z'
	mov byte [edi+1],0xb9
	
    jmp a 
a:
  jmp a 

fill_idt:
	mov word [idt + edi],bx
	mov word [idt + edi + 2],0x8
	mov byte [idt + edi + 4],0
	mov byte [idt + edi + 5],0x8e
	mov word [idt + edi + 6],dx
    
    add edi,8
    cmp edi,2048
    jl fill_idt
    ret
     
IDT_R:
     dw 2048
     dd idt            

ISR:
	cli
	pusha
    mov edi,0xb8000
    mov byte [edi],'p'
    mov byte [edi+1],0xb1
    
    mov al, 0x20
    out 0x20, al
    out 0xa0, al
    popa
    iret  
    

  
               
