/*
 * DIO.h
 *
 *  Created on: Aug 17, 2019
 *      Author: Badr
 */

#ifndef DIO_H_
#define DIO_H_
enum{
	A,
	B,
	C,
	D

};
void DIO_vidSetPortDirection(u8 portNumber,u8 value);
void DIO_vidSetPortValue(u8 portNumber,u8 value);
u8 DIO_u8ReadPortValue(u8 portNumber);
void DIO_vidSetPinDirection(u8 portNumber,u8 pin,u8 value);
void DIO_vidSetPinValue(u8 portNumber,u8 pin,u8 value);
u8 DIO_u8ReadPinValue(u8 portNumber,u8 pin);

#endif /* DIO_H_ */
