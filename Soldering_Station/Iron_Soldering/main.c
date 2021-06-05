/*
* Iron_Soldering.c
*
* Created: 04.10.2019 23:10:59
* Author : User
*/
#define F_CPU 8000000UL // Частота микроконтроллера

#include <util/delay.h>
#include <avr/io.h>
#include "lcd_lib.h"
#include "ADC.h"
#include "PWM.h"
#include "interrupts.h"

//Кнопка и энкодер
#define KEY_A  PIND&(1 << PD3)
#define KEY_B  PIND&(1 << PD4)
#define KEY_C  PINB&(1 << PB0) // кнопка

//Переключатель света
#define GREEN_LIGHT   PD1 //
#define RED_LIGHT    PD2 //

#define LIGHT_SWITCH    PD2 // 3 порт

#define SWITCH_LOAD     0x06 // 1 и 2 порт
#define SWITCH_SPEAKER  PD5 // 4 порт

unsigned char KEY_A_previous; // переменная для хранения предыдущего состояния кнопки 1
unsigned char KEY_B_previous; // переменная для хранения предыдущего состояния кнопки 2
unsigned char KEY_C_previous; // переменная для хранения предыдущего состояния кнопки 3

unsigned char isPressedKey_a;
unsigned char isPressedKey_b;

unsigned char KEY_A_current; // переменная для хранения текущего состояния кнопки 1
unsigned char KEY_B_current; // переменная для хранения текущего состояния кнопки 2
unsigned char KEY_C_current; // переменная для хранения текущего состояния кнопки 3

char first = 1;
signed char state = 0, count, x, y = 0;
int j = 0, k = 0 , t = 0, t_dr = 0;
unsigned short temp_sol, temp_dr, temper = 0, usttemp = 0, usttemp2 = 0, PwmTemp;
char a;
char button;
unsigned short ticks = 0;
unsigned short temp_dr_cur;
signed short temp_sol_cur;


// функция для отображения изменения параметров в настройках
void lcd_settings (char num)
{
	switch (num)
	{
		case 0:
		lcd_gotoxy(4,0);
		lcd_string("SETTINGS", 8);
		lcd_gotoxy(4,1);
		lcd_string("Voice:", 6);
		lcd_gotoxy(10,1);
		//		lcd_string(Result(VOICE), 3);
		break;
		
		case 1:
		lcd_gotoxy(4,0);
		lcd_string("SETTINGS", 8);
		lcd_gotoxy(4,1);
		lcd_string("Light:", 6);
		lcd_gotoxy(10,1);
		//		lcd_string(Result(LIGHT), 3);
		break;
		
		case 2:
		lcd_gotoxy(4,0);
		lcd_string("SETTINGS", 8);
		//ftoa(I_SET,s,2); // Значения float переводим в строку и заносим в массив s
		//sprintf(string,"Current:%s",s); // Переносим форматированный текст в массив string
		//lcd_gotoxy(3,1);
		//lcd_string(string,9);   // На LCD
		break;
		
		case 3:
		lcd_gotoxy(4,0);
		lcd_string("SETTINGS", 8);
		lcd_gotoxy(4,1);
		lcd_string("?????: OFF", 10);
		break;
		
		default:
		break;
	}
}


short encoder ()
{
	signed short value = 0;
	KEY_A_current = KEY_A;
	KEY_B_current = KEY_B;
	if ((KEY_A_previous != 0) && (KEY_A_current == 0) && (KEY_B_current != 0)) // Если при падении A на B 1, то крутим туда
	{
		state -= 1;
	}
	if ((KEY_A_previous != 0) && (KEY_A_current == 0) && (KEY_B_current == 0)) // // Если при падении A на B 0, то крутим обратно
	{
		state += 1;
	}
	
	if ((KEY_A_previous != 0) && state != 0)
	{
		
		if (state == 1 && KEY_B_current == 0 || state == -1 && KEY_B_current != 0 )
		{
			value += state;
			state = 0;
		}
	}
	KEY_A_previous = KEY_A_current;
	value += state;
	state = 0;
	return value;
}

void but (void)
{
	KEY_C_current = KEY_C;
	if ((KEY_C_previous != 0) && (KEY_C_current == 0))
	{
		if (KEY_C_current == 0)
		{
			count++;
			if (count > 100)
			{
				
			}
		}
		button = 1;
	}
	KEY_C_previous = KEY_C_current;
	return button;
}

// Выход в настройки
void MenuSettings (void)
{
	_delay_ms(100);
	lcd_clr();
	lcd_gotoxy(5,0);
	lcd_string("Settings",8); // На LCD
	_delay_ms(500);
	lcd_clr();
	///////__________Здесь_делаю_меню___________////////

	while(a <= 2) // Крутимся в цикле по этому условию
	{
		x += encoder();
		if (button)// Если нажали кнопку 3, то можно задавать значения
		{
			lcd_settings(x);
		}
		a = 1;// Поставили флаг и крутимся в цикле
		lcd_settings(x); // Выводим название текущей настройки меню
		
		// перемещаемся вверх
		if (0 > x)
		{
			x = 3;
		}
		// перемещаемся вниз
		if (x > 3)
		{
			x = 0;
		}
		lcd_clr();

		//_____________________________КОНЕЦ_МЕНЮ___________________________________//
	}
}

unsigned short Mes_T (void)
{
	unsigned short T_SUM = 0;
	for (int j = 0; j < 100; j++) // 500 выборок
	{
		T_SUM +=  adc_device(7);
	}
	temp_sol_cur = (T_SUM/100); // вычисляем падение напряжение на шунте усиливаю с помощью ОУ
	return temp_sol_cur;
}

// функция работы устройства
void display (void)
{
	lcd_gotoxy (0,0);
	lcd_string("Solder:",7);
	lcd_gotoxy (7,0);
	lcd_num_to_str(usttemp,3);
	
	lcd_gotoxy (11,0);
	lcd_string("T:",2);
	if (temp_sol_cur < 0)
	{
		lcd_gotoxy (13,0);
		lcd_num_to_str(0,4);
	}
	else
	{
		lcd_gotoxy (13,0);
		lcd_num_to_str(temp_sol_cur,3);
	}
	
	lcd_gotoxy (0,1);
	lcd_string("Dryer:",6);
	lcd_gotoxy(6,1);
	lcd_num_to_str(OCR1B,3);
	
	lcd_gotoxy (11,1);
	lcd_string("T:",2);
	lcd_gotoxy (13,1);
	lcd_num_to_str(temp_dr_cur,3);
	
	//lcd_gotoxy(10,1);
	//lcd_string("FAN",3);
	//lcd_gotoxy (13,1);
	//lcd_num_to_str(OCR0A,3);
}

void D10 (void)
{
	if ((usttemp - temp_sol_cur) < 5 & (usttemp - temp_sol_cur) > 1)
	{
		OCR1A = 400;
	}
	else
	{
		OCR1A = 0;
	}
}

void D100 (void)
{
	if ((usttemp - temp_sol_cur) < 50 & (usttemp - temp_sol_cur) > 10)
	{
		OCR1A = 500;
	}
	else
	{
		D10 ();
	}
}

void D200 (void)
{
	if ((usttemp - temp_sol_cur) < 250 & (usttemp - temp_sol_cur) > 50)
	{
		OCR1A = 600;
	}
	else
	{
		D100 ();
	}
}

void D300 (void)
{
	if ((usttemp - temp_sol_cur) < 350 & (usttemp - temp_sol_cur) > 250)
	{
		OCR1A = 700;
	}
	else
	{
		D200 ();
	}
}

void D400 (void)
{
	if ((usttemp - temp_sol_cur) < 480 & (usttemp - temp_sol_cur) > 200)
	{
		OCR1A = 800;
	}
	else
	{
		D300 ();
	}
}

void PWR (void)
{
	if (temp_sol_cur < usttemp)
	{
		D400 ();
	}
	else
	{
		OCR1A = 0;
	}

}
void soil(void)
{
	PWR();
	temp_sol_cur = adc_device(7);
	if (temp_sol_cur > 50)
	{
		temp_sol_cur -= 50;
	}
	usttemp  += (encoder() * 50);
	// сигнал о том что достигнута установленная температура
	if ((temp_sol_cur == usttemp) & (y == 1))
	{
		PORTD |= (1 << RED_LIGHT);
	//	sound_generator(3);
		y = 0;
	}
		// флаг, если изменили температуру на новую, чтобы сообщить о достижении новой температуры
		else if ((usttemp - temp_sol_cur) > 20)
		{
			y = 1;
			PORTD &= ~(1 << RED_LIGHT);
		}

	
	if (temp_sol_cur > 490)
	{
	OCR1A = 0;
	}
	
	if (usttemp > 490)
	{
		usttemp = 0;
	}
}

void work (void)
{	// Тут сразу идёт управление паяльником
	display();
	soil();
	//PWR();
	//temp_sol += adc_device(7);
	//t++;
	//if (t == 15)
	//{
	//temp_sol_cur = temp_sol / 15;
	//temp_sol = 0;
	//t = 0;
	//}
	//usttemp  += (encoder() * 50);
	temper += adc_device(6);
	t_dr++;
	if (t_dr == 20)
	{
		temp_dr_cur = (temper / 20) - 143;
		temper = 0;
		t_dr = 0;
	}

	if (temp_dr_cur < 50)
	{
		SwitchPwmFan(0);
	}
	else
	{
		SwitchPwmFan(1);
		OCR0A = 255;
	}
	but();
	// Если нажали кнопку, то переходим на управление феном
	// Далее при нажатии переходим на управление накалом спирали фена
	if (button)
	{
		button = 0;
		a = 1;
		SwitchPwmSolder(0);
		SwitchPwmFan(1);
		while (a < 2)
		{
			OCR0A  += encoder()*5;
			but();
				if (temp_dr_cur > 200)
				{
					OCR0A = 255;
				}
			display();
			if (button)
			{
				button = 0;
				a = 1;
				SwitchPwmDryer(1);
				t_dr = 0;
				temper = 0;
				while (a < 2)
				{
					temper += (adc_device(6)) - 113;
					t_dr++;
					if (t_dr == 20)
					{
						temp_dr_cur = (temper / 20);
						temper = 0;
						t_dr = 0;
					}
					

					//temp_dr_cur = temper / 100;
					OCR1B  += encoder() * 50;
					but();
					display();
					if (OCR0A < 240)
					{
						SwitchPwmDryer(0);
					}
					if (OCR1B > 500)
					{
						OCR1B = 0;
					}
						if (button)
						{
							SwitchPwmDryer(0);
							button = 0;
							a = 3;
							PORTD &= ~(1 << RED_LIGHT);
							SwitchPwmSolder(1);
							if (temp_dr_cur < 40)
							{
								SwitchPwmFan(0);
							}
							else
							{
								SwitchPwmFan(1);
								OCR0A = 255;
							}
						}
					}
				}
			}
		}
	
}

int main(void)
{
	init_pwm();
	init_adc();
	
	PORTD |= ((1 << PD0) | (1 << PD3) | (1 << PD4));
	DDRD |= 0xFF;
	DDRD &= ~(1 << PD3);
	DDRD &= ~(1 << PD4);
	
	DDRB &= ~(1 << PB0);
	lcd_init();
	lcd_clr();
	_delay_ms(100);
	PORTD |= (1 << GREEN_LIGHT);
	//sound_generator(1);
	SwitchPwmSolder(1);
	SwitchPwmFan(1);
	y = 1;
	while (1)
	{
		work();
		//soil();
	}
}
