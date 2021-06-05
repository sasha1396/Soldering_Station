
#include "ADC.h"
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>


unsigned int adc_device(unsigned char ADC_channel) // ����� ��������, �� � ��������� ��������� ����� ������ ���, ����� ������ �� ����������
{
	ADMUX = ADC_channel | (ADMUX & 0xF0); // ������������� �����, ��� ���� ���������� ��� �������� ��������� �������, �� ������� ������ ��������
	
	//�������� ��� ������������ �������� ����������
	_delay_us(10);
	
	//�������� �������������� (ADSC = 1)
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1 << ADIF)) == 0 ); //����, ���� ��� �������� �������������� (ADIF = 0)
	ADCSRA |= (1 << ADIF); //������������� ADIF
	return ADCW; //ADCW - �������� ADCH � ADCL
}

void init_adc(void)
{
	PORTC = 0b00000000; // �����
	DDRC = 0b00000000; // �����
	
	// ADC
	ADMUX |= ((0 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0));
	ADCSRA |= ((1 << ADEN) | (0 << ADSC) | (0 << ADATE) | (0 << ADIF) | (0 << ADIE) | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0));
}

