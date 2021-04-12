/* (C) 2021 by Alexander Chernokrylov <CodeDesign2763@gmail.com>
 *
 * This program is free software;
 * you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by
 * the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

/* MWE для исследования
 * странного поведения стенда EasyAVR6 при работе
 * с внешними прерываниями */ 
	 
/* Суть программы:
 * 1. Зажечь на стенде EasyAVR6 светодиоды,
 *	соответствующие PA3-PA7
 * 2. При нажатии на кнопку PD2, соотв. INT0
 *	светодиоды для PA3-PA7 должны погаснуть */
	 
/* Cуть странного поведения:
 * При нажатии на кнопку, соответствующую PD2 
 * cветодиоды для порта A должны гаснуть, но
 * при испытании на стенде этого не происходит
 *
 * При добавлении в код обработчика прерывания INT0
 * команды _delay_ms было замечено, что светодиоды гаснут
 * лишь на время ее работы 
 * 
 * Проблему удалось решить лишь при помощи дополнительной 
 * глобальной переменной:
 * Ее значение изменялось в обработчике прерываний,
 * а в основном цикле значение PORTA менялось по условию 
 * в зависимости от ее значения */

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect) {
	/* Гасим светодиоды для порта А */
	PORTA=0x00;
}

int main(void)
{
    /* Джампер J13 может быть в двух положениях:
	 * VCC и GND
	 * В положении VCC вывод PD2 должен быть подтянут к 0,
	 * а при нажатии на кнопку на него должно подаваться VCC
	 * через резистор 220 Ом 
	 * 
	 * В положении GND вывод PD2 должен быть подтянут к 1,
	 * что легко сделать подав 1 на соответствующий разряд
	 * PORTD, при нуле на разряде в DDRD. Тогда кнопка должна
	 * замыкать вывод PD2 на GND через резистор 220 Ом */
	
	/* Код для джампера J13 в положении GND */
	
	/* Начальная инициализация портов в/в */
	/* Порт А в режим вывода */
	DDRA=0xff;
	/* Зажигаем светодиоды для порта А */
	PORTA=0b11110000;
	
	/* Порт D в режим ввода */
	DDRD=0x00;
	/* Подтянем все пины к 1 */
	PORTD=0xFF;
	/* На всякий случай проверим,
	 * чтобы не был отключен режим подтягивания */
	SFIOR |= (0 << PUD) ;
	
	/* Настройка прерываний */
	/* Разрешение прерываний с векторами INT0 и INT1 */
	GICR |= (1 << INT1) | (1 << INT0);
	
	/* Срабатывание INT0 по спаду импульса */
	MCUCR |= (1 << ISC11) | (0 << ISC10);
		
	/* Глобальное разрешение прерываний в SREG */
	sei();
	
    while (1) 
    {
	}
}
