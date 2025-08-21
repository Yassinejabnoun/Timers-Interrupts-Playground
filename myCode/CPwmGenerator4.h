/*
 * CPwmGenerator4.h
 *
 *  Created on: 23.01.2022
 *      Author: jabno
 */

#ifndef CPWMGENERATOR4_H_
#define CPWMGENERATOR4_H_
#include "mbed.h"
#include "main.h"
/**
 *Modelliert unter Nutzung des Timer 3 ein Interrupt,
 * um die Helligkeit der LED6 an PC6 zu steuern. . Die Periode ist mit einer
 * Ausgangsfrequenz des Prescalers von 1 MHz und einem Counter-Modulo
 * von 1 μs bis 65536 μs variable eingestellt .
 * Das Ein-/ausverhältnis ist einstellbar von 0 % bis 100% .
 */
class CPwmGenerator4 {
private:
	DigitalOut *m_pwmOut;

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

	/**
	 * Schaltet die benötigten Funktionen für die Interrupt-Verarbeitung ein
	 * und legt die handleInterrupt Klassenmethode als Interrupt-Handler fest.
	 */
	void configureInterrupt();

	/**
	 * Behandelt den Interrupt des Timer3, sodass Ausgang beim
	 * Update-Interrupt die Led eingeschaltet  und beim
	 * Compare-Interrupt auf 0 gesetzt wird.
	 * Setzt außerdem die Interrupt-Fläge zurück.
	 */
	void handleInterrupt();


public:
	/**
	 * Konstruktor der Klasse
	 * Ein neues Objekt erzeugen , und der Timer bzw der GPIO-Pin konfigurieren.
	 *
	 * @param period : Periodendauer in μs
	 */
	CPwmGenerator4(uint16_t period);

	/**
	 * Den Tastgrad in prozent einstellen. Der Wert des Arguments muss
	 * zwischen 0 und 100 liegen, Werte größer 100 werden wie 100 behandelt.
	 *
	 * @param percent : der Tastgrad in Prozent .
	 */
	void setDutyCycle(uint8_t percent);

	/**
	 * Legt den Pin fest, auf dem das PWM-Signal ausgegeben werden soll.
	 *
	 * @param pin legt der Pin fest, auf dem das PWM-Signal ausgegeben
	 * werden soll.
	 */
	void setPwmOut(DigitalOut *pin);
};

#endif /* CPWMGENERATOR4_H_ */
