/*
 * ATmega32A_ADC.c
 *
 * Created: 9/15/2020 3:44:16 AM
 *  Author: Khaled
 */ 



#include "ATmega32A_ADC.h"
#include "Microcontroller_config.h"


#define ADC_Enable     0x80
#define Clear_Channel  0xE0

/* ADC Registers addresses */
#define  ADCSRA			((reg8 *)(0x06 + IO_OFFSET))
#define  ADMUX			((reg8 *)(0x07 + IO_OFFSET))
#define  ADCL			((reg8 *)(0x04 + IO_OFFSET))
#define  ADCH			((reg8 *)(0x05 + IO_OFFSET))
/*To read all data from Registers by 2 byte step pointer*/
#define  ADC_DATA		(*(vuint16_t *) (0x04 + IO_OFFSET))

/* SFIOR Register is defined in Microconteroller_Config.h as it is related more than one peripheral */

#define  ADC_VECTOR     _VECTOR(16)

void (*ADC_Callback)(uint16_t)= NULL;

ISR(ADC_VECTOR)
{
	if (ADC_Callback != NULL)
	{
		/* Pass the ADC_DATA to the function in the next layer to read the data */
		ADC_Callback(ADC_DATA);
	}
}


void ADC_init(ADC_ReferenceSelect  Reference, ADC_ConvMode Conversion, ADC_PrescalerSelect F_ADC, ADC_Interrupt InteruptMode)
{
	ADMUX->allRegister = Reference ;
	/* Select the ADC Freq, Enable the EDC, Select conversion mode and Interrupt state */
	ADCSRA->allRegister = F_ADC | ADC_Enable | (Conversion & 0x00FF) | InteruptMode ;
}

uint16_t ADC_Read_Polling(ADC_Channel Channel, ADC_Resolution Resolution)
{
	uint16_t Result=0;
	switch (Resolution)
	{
		case ADC_10_Bits:
		/*Right adjust Result*/
		ADMUX->bits.b5=LOW;
		break;
		case ADC_8_Bits:
		/*Left adjust Result to read 8 bit*/
		ADMUX->bits.b5=HIGH;
		break;
		default:
		break;
	}
	/*Clear Previous Channel Selection and select new channel*/
	ADMUX->allRegister &= Clear_Channel;
	ADMUX->allRegister |=Channel;
	/*start conversion*/
	ADCSRA->bits.b6=HIGH;
	/* Wait till the conversion ended by checking the interrupt flag*/
	while(ADCSRA->bits.b4==LOW)
	{
		/*stuck*/
	}
	/* After conversion ended we must clear interrupt flag..ADFI is cleared by writing 1 to it*/
	ADCSRA->bits.b4=1;
	
	/* Switch again after conversion to return data */
	switch(Resolution)
	{
		case ADC_10_Bits:
		Result = ADC_DATA;
		break;
		case ADC_8_Bits:
		Result = ADCH->allRegister;
		break;
		default:
		break;
		
	}
	return Result;
}

void ADC_Read_Interrupt( ADC_Channel Channel, ADC_Resolution Resolution)
{
	switch (Resolution)
	{
		case ADC_10_Bits:
		/*Right adjust Result*/
		ADMUX->bits.b5=LOW;
		break;
		case ADC_8_Bits:
		/*Left adjust Result to read 8 bit*/
		ADMUX->bits.b5=HIGH;
		break;
		default:
		break;
	}
	/*Clear Previous Channel Selection and select new channel*/
	ADMUX->allRegister &= Clear_Channel;
	ADMUX->allRegister |=Channel;
	/* start conversion then the interrupt will occur */
	ADCSRA->bits.b6=HIGH;
}

float ADC_Read_Voltage(uint16_t ADC_Code , ADC_Channel Channel, float Gain)
{
	float ADC_Result = 0.0;
	switch (Channel)
	{
		/*Single Ended cases*/
		case ADC0:
		case ADC1:
		case ADC2:
		case ADC3:
		case ADC4:
		case ADC5:
		case ADC6:
		case ADC7:
		ADC_Result =(ADC_Code*5.0)/1024.0;
		break;
		default:
		/* Differential Channels */
		ADC_Result =(ADC_Code*5.0)/(512.0 * Gain);
		break;
	}
	return ADC_Result;
}

void ADC_Disable(void)
{
	ADCSRA->bits.b7=LOW;
}