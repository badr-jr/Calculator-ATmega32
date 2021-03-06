/*
 * LCD.h
 *
 *  Created on: Aug 23, 2019
 *      Author: Badr
 */

#ifndef LCD_H_
#define LCD_H_

#define LCD_Clear 0b00000001
void LCD_vidInit(void);
void LCD_vidWriteData(u8 data);
void LCD_vidWriteCommand(u8 command);
void LCD_vidWriteString(u8 *str);
void LCD_vidWriteNumber(s32 num);
void LCD_vidSetPosition(u8 row,u8 col);

#endif /* LCD_H_ */
