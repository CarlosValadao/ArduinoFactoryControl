#ifndef DHT_H_INCLUDED
#define DHT_H_INCLUDED
/*
||
||  Filename:	 		DHT.h
||  Title: 			    DHTxx Driver
||  Author: 			Efthymios Koktsidis
||	Email:				efthymios.ks@gmail.com
||  Compiler:		 	AVR-GCC
||	Description:
||	This library can drive DHT11 and DHT22 sensors.
||
*/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//------ Headers ------//
#include <inttypes.h>
#include <util/delay.h>
#include <avr/io.h>

#include "IO_Macros.h"
#include "DHT_Settings.h"

//----------------------//

//----- Auxiliary data -------------------//
#define DHT11 1
#define DHT22 2
#define DHT_ReadInterval 1500

#define __DHT_Delay_Setup 2000

enum DHT_Status_t
{
    DHT_Ok,
    DHT_Error_Humidity,
    DHT_Error_Temperature,
    DHT_Error_Checksum,
    DHT_Error_Timeout
};
//-----------------------------------------//

//----- Prototypes---------------------------//
void DHT_Setup();
enum DHT_Status_t DHT_GetStatus();
enum DHT_Status_t DHT_ReadRaw(uint8_t Data[4]);
enum DHT_Status_t DHT_GetTemperature(uint8_t *Temperature);
enum DHT_Status_t DHT_GetHumidity(uint8_t *Humidity);
enum DHT_Status_t DHT_Read(uint8_t *Temperature, uint8_t *Humidity);
double DHT_CelsiusToFahrenheit(double Temperature);
double DHT_CelsiusToKelvin(double Temperature);
//-------------------------------------------//
#endif