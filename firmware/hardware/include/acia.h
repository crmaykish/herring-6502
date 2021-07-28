#ifndef ACIA_H
#define ACIA_H

extern void acia_init();

extern char getc();
extern void __fastcall__ putc(char);
extern void __fastcall__ print(char *);

#endif
