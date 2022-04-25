/*
 * ADC.c
 *
 *  Created on: Oct 5, 2019
 *      Author: Badr
 */
#include "Types.h"
#include "Registers.h"
#include "Macros.h"

void ADC_vidInit(void){
	ClrBit(ADMUX,0);
	ClrBit(ADMUX,1);
	ClrBit(ADMUX,2);		// single conversion channel 0
	ClrBit(ADMUX,3);
	ClrBit(ADMUX,4);

	ClrBit(ADMUX,6);		// AREF internal
	ClrBit(ADMUX,7);

	ClrBit(ADMUX,5);		// Right Adjustment

	SetBit(ADCSRA,0);
	SetBit(ADCSRA,1);		// prescaler 128
	SetBit(ADCSRA,2);

	SetBit(ADCSRA,7);		// enable ADC

	SetBit(ADCSRA,6);		// start conversion
}

u8 ADC_u8GetResult(u16 *result){
	u8 status;
	if(GetBit(ADCSRA,4)==1){
		*result=ADCLow & 0b0000001111111111;
		SetBit(ADCSRA,4); 	// clear flag
		SetBit(ADCSRA,6);	// start conversion
		status=1;
	}
	else{
		status=0;
	}
	return status;
}
