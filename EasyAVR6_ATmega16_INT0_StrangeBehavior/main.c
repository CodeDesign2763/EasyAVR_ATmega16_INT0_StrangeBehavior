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

/* MWE ��� ������������
 * ��������� ��������� ������ EasyAVR6 ��� ������
 * � �������� ������������ */ 
	 
/* ���� ���������:
 * 1. ������ �� ������ EasyAVR6 ����������,
 *	��������������� PA3-PA7
 * 2. ��� ������� �� ������ PD2, �����. INT0
 *	���������� ��� PA3-PA7 ������ ��������� */
	 
/* C��� ��������� ���������:
 * ��� ������� �� ������, ��������������� PD2 
 * c��������� ��� ����� A ������ �������, ��
 * ��� ��������� �� ������ ����� �� ����������
 *
 * ��� ���������� � ��� ����������� ���������� INT0
 * ������� _delay_ms ���� ��������, ��� ���������� ������
 * ���� �� ����� �� ������ 
 * 
 * �������� ������� ������ ���� ��� ������ �������������� 
 * ���������� ����������:
 * �� �������� ���������� � ����������� ����������,
 * � � �������� ����� �������� PORTA �������� �� ������� 
 * � ����������� �� �� �������� */

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(INT0_vect) {
	/* ����� ���������� ��� ����� � */
	PORTA=0x00;
}

int main(void)
{
    /* ������� J13 ����� ���� � ���� ����������:
	 * VCC � GND
	 * � ��������� VCC ����� PD2 ������ ���� �������� � 0,
	 * � ��� ������� �� ������ �� ���� ������ ���������� VCC
	 * ����� �������� 220 �� 
	 * 
	 * � ��������� GND ����� PD2 ������ ���� �������� � 1,
	 * ��� ����� ������� ����� 1 �� ��������������� ������
	 * PORTD, ��� ���� �� ������� � DDRD. ����� ������ ������
	 * �������� ����� PD2 �� GND ����� �������� 220 �� */
	
	/* ��� ��� �������� J13 � ��������� GND */
	
	/* ��������� ������������� ������ �/� */
	/* ���� � � ����� ������ */
	DDRA=0xff;
	/* �������� ���������� ��� ����� � */
	PORTA=0b11110000;
	
	/* ���� D � ����� ����� */
	DDRD=0x00;
	/* �������� ��� ���� � 1 */
	PORTD=0xFF;
	/* �� ������ ������ ��������,
	 * ����� �� ��� �������� ����� ������������ */
	SFIOR |= (0 << PUD) ;
	
	/* ��������� ���������� */
	/* ���������� ���������� � ��������� INT0 � INT1 */
	GICR |= (1 << INT1) | (1 << INT0);
	
	/* ������������ INT0 �� ����� �������� */
	MCUCR |= (1 << ISC11) | (0 << ISC10);
		
	/* ���������� ���������� ���������� � SREG */
	sei();
	
    while (1) 
    {
	}
}
