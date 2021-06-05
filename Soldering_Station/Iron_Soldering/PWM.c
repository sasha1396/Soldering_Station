/*
 * PWM.c
 *
 * Created: 01.12.2018 21:24:14
 * Author : Sasha
 */ 
#include "PWM.h"
#include <avr/io.h>
#include <util/delay.h>

char i = 0;

// ���/���� ��� A0 ���������� ����
void SwitchPwmFan(char value)
{
	if (value)
	{
		TCCR0A |= (1 << COM0A1); // ����� B �������
		OCR0A = 0; //
	}
	else
	TCCR0A &= ~(1 << COM0A1); // �� ���������� ��� ���
}

// ���/���� ��� A1 ��������
void SwitchPwmSolder(char value)
{
	if (value)
	{
		TCCR1A |= (1 << COM1A1); // ����� B �������
		OCR1A = 0; //
	}
	else
	TCCR1A &= ~(1 << COM1A1); // �� ���������� ��� ���
}

// ���/���� ��� �1 ���
void SwitchPwmDryer(char value) 
{
	if (value)
	{
		TCCR1A |= (1 << COM1B1); // ����� B �������
        OCR1B = 0; //
	}
	else
	TCCR1A &= ~(1 << COM1B1); // �� ���������� ��� ���
}


void sound_generator(char n)// n - ����������� ������, t - ������������ ������� (1 - ��� 100 ��)
{
	TCCR0A |= (1 << COM0B1); // �������� ���
	while(i < n)
	{
		TCCR0A |= (1 << COM0B1);
		OCR0B = 0x7F;// 75% 150
		_delay_ms(100); // ������������
		OCR0B = 0x00;
		TCCR0A &= ~(1 << COM0B1);
		_delay_ms(5); // �����
		i++;
	}
	TCCR0A &= ~(1 << COM0B1); // ��������� ���
	i = 0;// �������� �������
}

void init_pwm(void)// ������������� ������ ��� ���
{
	// ����������� ������� ���� �� �����, �� �� ������ �� �� ��� 1
//    PORTB = 0b00000000;
	DDRB = 0b00000110;
//	PORTD = 0b00000000;
	DDRD = 0b01100000;
	
	// ��� 0-�� ������
	TCCR0A |= ((0 << COM0A1) | (0 << COM0A0) | (0 << COM0B1) | (0 << COM0B0) | (1 << WGM01) | (1 << WGM00)); // FAST PWM
	TCCR0B |= ((0 << FOC0A)  | (0 << FOC0B) | (0 << WGM02) | (0 << CS02) | (1 << CS01) | (1 << CS00));
	//OCR0A = 0x7D; // ��� ��������
	//OCR0B = 0xDC; // �� 50% ����������
	
	// ��� ������� ������  COM1A1 (COM1B1) - �������� ��� /// COM1A0 (COM1B0) - ����������� ������
	TCCR1A |= ((0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0)  | (1 << WGM11) | (1 << WGM10)); 
	TCCR1B |= ((0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (1 << CS10));
	TCCR1C |= ((0 << FOC1A)  | (0 << FOC1B));
	
	// ��� ������� ������
		
	/*CS22 CS21 CS20	Description
		0	0	0		No clock source (Timer/Counter stopped)
		0	0	1		clkT2S/(No prescaling)
		0	1	0		clkT2S/8 (From prescaler)
		0	1	1		clkT2S/32 (From prescaler)
		1	0	0		clkT2S/64 (From prescaler)
		1	0	1		clkT2S/128 (From prescaler)
		1	1	0		clkT2S/256 (From prescaler)
		1	1	1		clkT2S/1024 (From prescaler)*/
		
	// Fpwm = Fclk/(N*510) (������ ���� 1-3��� ��� �������� ((0 << CS22) | (1 << CS21) | (0 << CS20))) F = 1960 ��
	//TCCR2A |= ((0 << FOC1A) | (0 << COM2A1) | (0 << COM2A0) | (1 << WGM20) | (0 << WGM21) | (0 << CS22) | (1 << CS21) | (0 << CS20));
	//TIMSK0 |= ((0 << TOIE2) | (0 << OCIE1A) | (0 << OCIE1B) | (0 << TOIE1) | (0 << TOIE0));
	//OCR2A = 0x7F; // 50% ����������� ����������
}

