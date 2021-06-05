//*****************************************************************************
#ifndef LCD_LIB
#define LCD_LIB

#include <inttypes.h>
#include <util/delay.h>
//Uncomment this if LCD 4 bit interface is used
//******************************************
#define LCD_4bit
//***********************************************

#define F_CPU 8000000UL // „астота микроконтроллера

#define LCP PORTC	//define MCU port connected to LCD control pins
#define LCDR DDRC	//define MCU direction register for port connected to LCD control pins

#define LCD_RS	0 	//define MCU pin connected to LCD RS
#define LCD_RW	6 	//define MCU pin connected to LCD R/W
#define LCD_E	1	//define MCU pin connected to LCD E

#define LDP PORTC	//define MCU port connected to LCD data pins
#define LDDR DDRC	//define MCU direction register for port connected to LCD data pins

#define LCD_D4	2	//define MCU pin connected to LCD D3
#define LCD_D5	3	//define MCU pin connected to LCD D4
#define LCD_D6	4	//define MCU pin connected to LCD D5
#define LCD_D7	5	//define MCU pin connected to LCD D6



#define LCD_CLR             0	//DD0: clear display
#define LCD_HOME            1	//DD1: return to home position
#define LCD_ENTRY_MODE      2	//DD2: set entry mode
#define LCD_ENTRY_INC       1	//DD1: increment
#define LCD_ENTRY_SHIFT     0	//DD2: shift
#define LCD_ON_CTRL         3	//DD3: turn lcd/cursor on
#define LCD_ON_DISPLAY      2	//DD2: turn display on
#define LCD_ON_CURSOR       1	//DD1: turn cursor on
#define LCD_ON_BLINK        0	//DD0: blinking cursor
#define LCD_MOVE            4	//DD4: move cursor/display
#define LCD_MOVE_DISP       3	//DD3: move display (0-> move cursor)
#define LCD_MOVE_RIGHT      2	//DD2: move right (0-> left)
#define LCD_FUNCTION        5	//DD5: function set
#define LCD_FUNCTION_8BIT   4	//DD4: set 8BIT mode (0->4BIT mode)
#define LCD_FUNCTION_2LINES 3	//DD3: two lines (0->one line)
#define LCD_FUNCTION_10DOTS 2	//DD2: 5x10 font (0->5x7 font)
#define LCD_CGRAM           6	//DD6: set CG RAM address
#define LCD_DDRAM           7	//DD7: set DD RAM address
// reading:
#define LCD_BUSY            7	//DD7: LCD is busy
#define LCD_LINES			2	//visible lines 2 строки 
#define LCD_LINE_LENGTH		16	//line length (in characters) 16 символов
// cursor position to DDRAM mapping
#define LCD_LINE0_DDRAMADDR		0x00
#define LCD_LINE1_DDRAMADDR		0x40
#define LCD_LINE2_DDRAMADDR		0x14
#define LCD_LINE3_DDRAMADDR		0x54
// progress bar defines
#define PROGRESSPIXELS_PER_CHAR	6


void lcd_dat(uint8_t);		//вывод одного символа в текущую позицию
void lcd_com(uint8_t);	//посылка команды в LCD
void lcd_init(void);			//Initializes LCD
void lcd_clr(void);				//Clears LCD
void lcd_home(void);			//переводит курсор в начало
void lcd_string(char*, char);	//вывод строки указанной длинны в текущую позицию
void lcd_gotoxy(uint8_t, uint8_t);	//перемещает курсор в указанную позицию XY
void copy_string_to_lcd(const uint8_t*, uint8_t, uint8_t);// вывод строки из флеш-пам€ти в указанную позицию диспле€ x,y
void lcd_definechar(const uint8_t *,uint8_t);//записывает пользовательское изображение символа в пам€ть диспле€
void lcd_shift_right(uint8_t);	//shift by n characters Right
void lcd_shift_left(uint8_t);	//shift by n characters Left
void lcd_cursor_on(void);		//включает курсор подчеркивание
void lcd_cursor_blink(void);	//включает мигающий курсор
void lcd_cursor_off(void);		//выключает курсор
void lcd_blank(void);			//отключает изображение, но не очищает
void lcd_visible(void);			//включает изображение
void lcd_cursor_left(uint8_t);	//перемещает курсор на указанное число символов влево
void lcd_cursor_right(uint8_t);	//перемещает курсор на указанное число символов вправо
void lcd_progress_bar(uint8_t progress, uint8_t maxprogress, uint8_t length);//позвол€ет выводить на дисплей динамическую шкалу
void lcd_num_to_str(uint16_t value, uint8_t nDigit);// позвол€ет выводить на дисплей переменную до 4 разр€дов

#endif

