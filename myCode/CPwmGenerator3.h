/*
 * CPwmGenerator3.h
 *
 *  Created on: 22.01.2022
 *      Author: jabno
 */

#ifndef CPWMGENERATOR3_H_
#define CPWMGENERATOR3_H_
#include "mbed.h"
/**
 * Modelliert unter Nutzung des Timer 3 einen Generator für ein
 * PWM-Signal auf PC6.
 * Die Periodendauer des PWM-Signals wird über die methode configureTimer
 * eingegeben.
 * Das Ein-/ausverhältnis ist einstellbar von 0 % bis 100% .
 */
class CPwmGenerator3 {
private:
	/**
	 * Konfiguriert PC6 als Alternate Function-Ausgang und selektiert
	 * den von Timer 3/Compare Register 1 generierten Wert als das
	 * auszugebende Signal.
	 */
	void configureGpioPin();

	/**
	 * Konfiguriert den Timer 3 inklusive des Compare Register 1 so,
	 * dass ein Signal wie in der Klassenbeschreibung dargestellt
	 * erzeugt wird.
	 *
	 * @param period : Periodendauer in μs
	 */
	void configureTimer(uint16_t period);

public:
	/**
	 * Konstruktor der Klasse
	 * Konfiguriert die Hardware durch Aufrufen der privaten Methoden .
	 *
	 * @param period : Periodendauer in μs
	 */
	CPwmGenerator3(uint16_t period);

	/**
	 * Den Tastgrad in prozent einstellen. Der Wert des Arguments muss
	 * zwischen 0 und 100 liegen, Werte größer 100 werden wie 100 behandelt.
	 *
	 * @param percent : der Tastgrad in Prozent .
	 */
	void setDutyCycle(uint8_t percent);
};
#endif /* CPWMGENERATOR3_H_ */
