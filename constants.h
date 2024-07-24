#ifndef __AVR_ATmega328P__
//#define __AVR_ATmega328P__
#endif
#define F_CPU 16000000UL  
#define BAUD 250000UL
#define MYUBRR F_CPU/16/BAUD-1