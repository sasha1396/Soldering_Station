/*
 * interrupts.c
 *
 * Created: 06.10.2019 13:18:35
 *  Author: User
 */ 

#include "interrupts.h"

void move_interrupts (void)
{
	MCUCR |= (1 << IVSEL);
	//MCUCR  = (0 << IVCE);
	PCICR |= (1 << PCIE2);
	PCIFR |= (1 << PCIF2);
	PCMSK2 |= ((1 << PCINT20) | (1 << PCINT19));
}


