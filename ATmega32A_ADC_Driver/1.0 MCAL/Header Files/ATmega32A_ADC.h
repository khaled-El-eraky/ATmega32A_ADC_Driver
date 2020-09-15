/*
 * ATmega32A_ADC.h
 *
 * Created: 9/15/2020 3:43:35 AM
 *  Author: Khaled
 */ 


#ifndef ATMEGA32A_ADC_H_
#define ATMEGA32A_ADC_H_


#include "StdTypes.h"
 /* Assigning initial values for all enumerations the in configuring the Registers >>OR not Assign  */
	typedef enum
	{
		/* AREF and Internal Vref turned off */
		AREF=0x00,
		/* AVCC with external capacitor at AREF pin */
		AVCC=0x40,
		/* Internal 2.56V Voltage Reference with external capacitor at AREF pin */
		INTERNAL_2_56v=0xC0,
	}ADC_ReferenceSelect;
	
	typedef enum
	{
		/* if the data is 10 bit so, store it Right adjusted by clear the ADLAR to read normally ADCL then ADCH */
		ADC_10_Bits=0x00,
		/* if the data is 8 bit so, store it Left adjusted by setting the ADLAR to read just ADCH */
		ADC_8_Bits=0x20,
	}ADC_Resolution;
	
	/* Enumeration for the table 84. in data sheet to select the channel as the data in table is ordered */
	typedef enum
	{
		ADC0,ADC1,ADC2,ADC3,
		ADC4,ADC5,ADC6,ADC7,
		ADC_DIFF_PADC0_NADC0_G10,
		ADC_DIFF_PADC1_NADC0_G10,
		ADC_DIFF_PADC0_NADC0_G200,
		ADC_DIFF_PADC1_NADC0_G200,
		ADC_DIFF_PADC2_NADC2_G10,
		ADC_DIFF_PADC3_NADC2_G10,
		ADC_DIFF_PADC2_NADC2_G200,
		ADC_DIFF_PADC3_NADC2_G200,
		ADC_DIFF_PADC0_NADC1_G1,
		ADC_DIFF_PADC1_NADC1_G1,
		ADC_DIFF_PADC2_NADC1_G1,
		ADC_DIFF_PADC3_NADC1_G1,
		ADC_DIFF_PADC4_NADC1_G1,
		ADC_DIFF_PADC5_NADC1_G1,
		ADC_DIFF_PADC6_NADC1_G1,
		ADC_DIFF_PADC7_NADC1_G1,
		ADC_DIFF_PADC0_NADC2_G1,
		ADC_DIFF_PADC1_NADC2_G1,
		ADC_DIFF_PADC2_NADC2_G1,
		ADC_DIFF_PADC3_NADC2_G1,
		ADC_DIFF_PADC4_NADC2_G1,
		ADC_DIFF_PADC5_NADC2_G1,
		ADC_BANDGAP_1_22v,
		ADC_GND_0v,
	}ADC_Channel;
	
	/* 16 bit enum LSByte for ADCSRA Register the MSByte is for SFIOR Reg */
	typedef enum
	{
		ADC_SingleConv = 0x0000,
		ADC_AutoTrig_FreeRuning = 0x0020,
		ADC_AutoTrig_AnalogComparator = 0x2020,
		ADC_AutoTrig_EXTINT0 = 0x4020 ,
		ADC_AutoTrig_TIMER0_CompareMatch = 0x6020,
		ADC_AutoTrig_TIMER0_OverFlow = 0x8020,
		ADC_AutoTrig_TIMER1_CompareMatch_B = 0xA020,
		ADC_AutoTrig_TIMER1_OverFlow = 0xC020,
		ADC_AutoTrig_TIMER1_CaptureEvent = 0xE020,
	}ADC_ConvMode;
	
	typedef enum 
	{
		ADC_FCPU_BY2 = 0x01,
		ADC_FCPU_BY4 = 0x02,
		ADC_FCPU_BY8 = 0x03,
		ADC_FCPU_BY16 = 0x04,
		ADC_FCPU_BY32 = 0x05,
		ADC_FCPU_BY64 = 0x06,
		ADC_FCPU_BY128 = 0x07,	
	}ADC_PrescalerSelect;
	
	typedef enum
	{
		ADC_InterruptDisable=0x00,
		ADC_InterruptEnable=0x08,
	}ADC_Interrupt;
	
	/*Pointer to function for the interrupt callback*/
	extern void (*ADC_Callback)(uint16_t);
	
	/* Function for initializing the ADC 
	   >>Para>> voltage REF, Conversion mode, Precaled Freq and interrupt state */
	void ADC_init(ADC_ReferenceSelect  Reference, ADC_ConvMode Conversion, ADC_PrescalerSelect F_ADC, ADC_Interrupt InteruptMode);
	
	/* Function for read data Polling_based 
	   >>para>>(The channel and Resolution) 
	   >>return>>(the ADC_Data) */				
	uint16_t ADC_Read_Polling(ADC_Channel    Channel, ADC_Resolution Resolution);
							
	/* Function for read data Interrupt_based >>para>>(The channel and Resolution)>>
	   does not return as by the end of conversion it will interrupt automatically */						
	void ADC_Read_Interrupt( ADC_Channel    Channel, ADC_Resolution Resolution);
								
	/* Function to disable the ADC */				
	void ADC_Disable(void);
	
	/*Function to read voltage on of ADC Pins*/
	float ADC_Read_Voltage(uint16_t ADC_Code , ADC_Channel Channel, float Gain);



#endif /* ATMEGA32A_ADC_H_ */