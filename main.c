#include "Types.h"
#include "LCD.h"
#include "Keybad.h"
#include <util/delay.h>
#define F_CPU 8000000
/* 
	Setting values to determine which operation needed
    Based on keypad button pressed 
*/
enum{
	plus=10,
	minus,
	multi,
	clear,
	equal,
	division,
	syntaxError,
	mathError

};
#define non 0
/*
	Setting initial values for counters, operators and reminder
*/	
s8 firstOperator=non,secondOperator=non,operandCnt=0,status=1,prevNum=0,cursor=0,reminder=0,initial=0,outResult=0;
s32 firstOperand=0,secondOperand=0;
/*  
	Function to turn negative numbers into positive
*/
s32 toPos(s32 num){
	if(num<0){
		num*=-1;
	}
	return num;
}
/* 
	This function do the following:
		-Turns string numbers entered into integers.
		-Handles float numbers.
		-Get reminder of float numbers.
*/
void connectNum(s8 digit){
	/*
		check if last data entered is operand not operator
	*/
	if(prevNum){
		/* 
			check if operands number are even so that we concatenate 
			the previous operand digit with the current digit entered
		*/
		if(operandCnt%2==0){
			if(!initial){
				firstOperand=0;
				initial=1;
			}
			firstOperand=(firstOperand*10)+digit;
			firstOperator=non;
		}
		
		else{
			if(!initial){
				secondOperand=0;
				initial=1;
			}
			secondOperand=(secondOperand*10)+digit;
			secondOperator=non;
		}
	}
	/*
		Check if any operands exist then calculate last two operands
		based on sign entered.
	*/
	else if(operandCnt>=1){
		/* 
			In this algorithms used here i use two variables for operator and two 
			variables for operands in order to preserve the last operand/operator
			and the current operand/operator
		*/
		if(firstOperator!=non){
			switch(firstOperator){
				case plus:
				secondOperand+=firstOperand;
				break;
				case minus:
				secondOperand=firstOperand-secondOperand;
				break;
				case multi:
				reminder=(reminder*toPos(secondOperand));
				secondOperand*=firstOperand;
				secondOperand+=(reminder/100);
				reminder=reminder%100;
				break;
				case division:
				if(secondOperand==0){
					status=mathError;
				}
				reminder=((toPos(firstOperand)*100)/toPos(secondOperand))%100;
				secondOperand=firstOperand/secondOperand;
				break;
			}
		}
		else if(secondOperator!=non){
			switch(secondOperator){
				case plus:
				firstOperand+=secondOperand;
				break;
				case minus:
				firstOperand=secondOperand-firstOperand;
				break;
				case multi:
				reminder=(reminder*toPos(firstOperand));
				firstOperand*=secondOperand;
				firstOperand+=(reminder/100);
				reminder=reminder%100;
				break;
				case division:
				if(firstOperand==0){
					status=mathError;
				}
				reminder=((toPos(secondOperand)*100)/toPos(firstOperand))%100;
				firstOperand=secondOperand/firstOperand;
				break;
			}
		}
	}
}
/* 
	This function implemented specially to process continuous operators entered
	Ex. 5+--+-5=0
*/
void calculate(s8 operator){
	if(operator==equal)
		return ;
	if(operandCnt%2!=0){

		if((operator==minus&&firstOperator==minus)||(operator==plus&&firstOperator==plus)){
			firstOperator=plus;
		}
		else if((operator==minus&&firstOperator==plus)||(operator==plus&&firstOperator==minus)){
			firstOperator=minus;
		}
		else if(firstOperator==non){
			firstOperator=operator;
		}
		else{
			status=syntaxError;
		}
	}
	else{

		if((operator==minus&&secondOperator==minus)||(operator==plus&&secondOperator==plus)){
			secondOperator=plus;
		}
		else if((operator==minus&&secondOperator==plus)||(operator==plus&&secondOperator==minus)){
			secondOperator=minus;
		}
		else if(secondOperator==non){
			secondOperator=operator;
		}
		else{
			status=syntaxError;
		}
	}
}


int main(void){
	/*
		Set initial configurations for LCD
	*/
	LCD_vidInit();
	/*
		Set initial configurations for Keypad
	*/
	Keybad_vidInit();
	while(1){
		s8 i,shownNum;
		for(i=0;i<16;i++){
			/*
				Check which button on keypad is pressed and set
				shownNum by its value
			*/
			if(Keybad_u8SetButtonStatus(i)==1){
				shownNum=0;
				switch(i){
					case 0:
					shownNum=1;
					break;
					case 1:
					shownNum=2;
					break;
					case 2:
					shownNum=3;
					break;
					case 3:
					shownNum=plus;
					break;
					case 4:
					shownNum=4;
					break;
					case 5:
					shownNum=5;
					break;
					case 6:
					shownNum=6;
					break;
					case 7:
					shownNum=minus;
					break;
					case 8:
					shownNum=7;
					break;
					case 9:
					shownNum=8;
					break;
					case 10:
					shownNum=9;
					break;
					case 11:
					shownNum=multi;
					break;
					case 12:
					shownNum=clear;
					break;
					case 13:
					shownNum=0;
					break;
					case 14:
					shownNum=equal;
					break;
					case 15:
					shownNum=division;
					break;

				}
				cursor++;
				/*
					If first line of LCD is complete shift cells
				*/
				if(cursor>16){
					LCD_vidWriteCommand(0b00011000);
				}
				/* 
					check if any calculations are called after result is out then show "Ans" 
					Ex. 5+5=10 when making another operations it like multiplication it becomes:
					Ans *   which Ans here mean 10 (result of first calculation).
				*/
				if(outResult){
					LCD_vidWriteCommand(0b00000001);
					LCD_vidWriteString("Ans");
					outResult=0;
				}
				/*
					check if any error occurred then initialize variable again
				*/
				else if(status==syntaxError||status==mathError){
					LCD_vidWriteCommand(0b00000001);
					firstOperator=non,secondOperator=non,operandCnt=0,status=1,prevNum=0,cursor=0,reminder=0,firstOperand=0,secondOperand=0;
				}
				/*
					Check if numerical operand is entered then concatenate digits of number
				*/
				if(shownNum<10){
					LCD_vidWriteNumber(shownNum);
					prevNum=1;
					connectNum(shownNum);
				}
				/*
					Otherwise it's operator then we determine the operator to use
					after precessing the continuous operators if any.
				*/
				else{

					if(prevNum){
						prevNum=0;
						initial=0;
						connectNum(shownNum);
						operandCnt++;
					}
					calculate(shownNum);
					/* 
						Here we show entered button on LCD
					*/
					switch(shownNum){
						case plus:
						LCD_vidWriteString("+");
						break;
						case minus:
						LCD_vidWriteString("-");
						break;
						case multi:
						LCD_vidWriteString("*");
						break;
						case division:
						LCD_vidWriteString("/");
						break;
						case equal:
						LCD_vidWriteString("=");
						LCD_vidSetPosition(1,0);
						/*
							Here we check if cells of first line is complete so we can shift them
						*/
						while(cursor>16){
							LCD_vidWriteCommand(0b00011100);
							cursor--;
						}
						/*
							Check if syntax error occurred and printing it
						*/
						if(status==syntaxError){
							LCD_vidWriteString("Syntax Error");
						}
						/*
							Check if math error occurred and printing it
						*/
						else if(status==mathError){
							LCD_vidWriteString("Math Error");
						}
						/*
							if no errors occurred then start printing the result 
						*/
						else{
							outResult=1;
							/* 
								if operands count is even then result is preserved in secondOperand variable
								as mentioned before
							*/
							if(operandCnt%2==0){
								/* 
									Check if the result is negative then print minus sign
								*/
								if(secondOperand<0){
									LCD_vidWriteString("-");
									LCD_vidWriteNumber(secondOperand*-1);
								}
								else{
									LCD_vidWriteNumber(secondOperand);
								}

							}
							/* 
								if operands count is odd then result is preserved in firstOperand variable
							*/
							else{
								if(firstOperand<0){
									LCD_vidWriteString("-");
									LCD_vidWriteNumber(firstOperand*-1);
								}
								else{
									LCD_vidWriteNumber(firstOperand);
								}

							}
							/* 
							Check if reminder exist (float) then we print it 
							*/
							if(reminder!=0){
								LCD_vidWriteString(".");
								LCD_vidWriteNumber(reminder);
							}
						}
						break;
						case clear:
						LCD_vidWriteCommand(0b00000001);
						firstOperator=non,secondOperator=non,operandCnt=0,status=1,prevNum=0,cursor=0,reminder=0,firstOperand=0,secondOperand=0;
						break;
					}
				}
			}
		}
	}
}

