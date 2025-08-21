/*
 * CPwmGenerator2.h
 *
 *  Created on: 22.01.2022
 *      Author: jabno
 */

#ifndef CPWMGENERATOR2_H_
#define CPWMGENERATOR2_H_
#include "mbed.h"
/**
 * Modelliert unter Nutzung des Timer 3 einen Generator für ein
 * PWM-Signal auf PC6. Die Periode ist mit einer Ausgangsfrequenz des
 * Prescalers von ca. 2048 Hz und einem Counter-Modulo von 16*2048 fest
 * auf 16 s eingestellt.Das Ein-/ausverhältnis ist einstellbar von 0 %
 * bis 100% .
 */
class CPwmGenerator2 {
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
	 */
	void configureTimer();

public:
	/**
	 * Konstruktor der Klasse
	 * Konfiguriert die Hardware durch Aufrufen den privaten Methoden .
	 */
	CPwmGenerator2();

	/**
	 * Den Taktgrad in prozent einstellen. Der Wert des Arguments muss
	 * zwischen 0 und 100 liegen, Werte größer 100 werden wie 100 behandelt.
	 *
	 * @param percent : der Taktgrad in Prozent .
	 */
	void setDutyCycle(uint8_t percent);
};

#endif /* CPWMGENERATOR2_H_ */
