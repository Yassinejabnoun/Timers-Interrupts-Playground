#include "mbed.h"
#include "main.h"
#include "breadboard.h"
#include "CPwmGenerator1.h"
#include "CPwmGenerator2.h"
#include "CPwmGenerator3.h"
#include "CPwmGenerator4.h"
#include "CExponentialFilter.h"
#include "CShiftRegisterOutputExtender.h"
#include "CSevenSegmentDisplay.h"
#include "CPolledTimer.h"
#include "CDebouncer.h"


// Legen Sie die globalen Objekte, die die Peripheriekomponenten
// repräsentieren, hier an (vor den Funktionen).

// DigitalOut led1(LED1);

BusIn keys(BB_BTN_0, BB_BTN_1, BB_BTN_2, BB_BTN_3,
BB_BTN_4, BB_BTN_5, BB_BTN_6, BB_BTN_7);

AnalogIn poti(BB_POTI, 3.3);

DigitalOut ssdSerialData(SSD_SER);
DigitalOut ssdShiftRegisterClock(SSD_SCLK);
DigitalOut ssdBufferRegisterClock(SSD_RCLK);

DigitalOut ssdDigit0(SSD_DIGIT_0, OpenDrainNoPull);
DigitalOut ssdDigit1(SSD_DIGIT_1, OpenDrainNoPull);
DigitalOut ssdDigit2(SSD_DIGIT_2, OpenDrainNoPull);
DigitalOut ssdDigit3(SSD_DIGIT_3, OpenDrainNoPull);

/**
 * Realisiert mit CPwmGenerator1 auf PC6 ein PWM-Signal. Zusätzlich
 * werden die Bits 15-10 des Zählerstands auf den LEDs
 * ausgegeben.
 */
void task1() {
	// Die LEDs werden flexibler verwendet als in den vorherigen Laboren
	// und daher nicht als globales Objekt definiert sondern nach Bedarf
	// in den verschiedenen Tasks.
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);

	CPwmGenerator1 fixedGenerator;

	while (true) {
		// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
		leds = (TIM3->CNT >> 10) & 0x3f;
	}
}


/**
 * Realisiert mit CPwmGenerator1 auf PC6 ein PWM-Signal. Zusätzlich
 * werden die Bits 15-10 des Zählerstands auf den LEDs
 * ausgegeben.
 * Der Tastgrad des PWM-Generators wird über der Potentiometer angepasst werden,
 * die Spannung wird gemessen und über den exponentiellen Glättungsfilter
 * geglättet .
 *
 */
void task2() {
	// Die LEDs werden definiert
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);

	// Ein Objekt vom Typ CPwmGenerator2 wird erzeugt
	CPwmGenerator2 variableGenerator;

	// Filter für Messwerte wird erzeugt
	CExponentialFilter filter(0.05);
	while (true) {
    	// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
    	leds = (TIM3->CNT >> 10) & 0x3f;

    	// Den Messwert vom poti wird im Filter geschoben
    	filter<<(poti.read());

    	// Tastgrad auf das berechnete Prozent setzten
    	variableGenerator.setDutyCycle(filter*100);

	}
}


/**
 * Realisiert mit CPwmGenerator2 auf PC6 ein PWM-Signal. mit dem wird die
 *  Helligkeitssteuerung von Led6 gesteuert.
 * Der Tastgrad des PWM-Generators wird über der Potentiometer ,
 * die Spannung wird gemessen und über den exponentiellen Glättungsfilter
 * geglättet .
 * Die Periodendauer wird als Parameter in μs angegeben (von 0 bis 65536 μs ) .
 */
void task3() {
	// Die LEDs werden definiert.
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);

	CPwmGenerator3 variableGenerator(20000);
	CExponentialFilter filter(0.05);
	while (true) {
    	// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
    	leds = (TIM3->CNT >> 10) & 0x3f;

    	// Den Messwert vom poti wird im Filter geschoben
    	filter<<(poti.read());

    	// Tastgrad auf das berechnete Prozent setzten
    	variableGenerator.setDutyCycle(filter*100);
	}
}

/**
 * Realisiert mit CPwmGenerator2 auf PC6 ein PWM-Signal. mit dem wird die
 *  Helligkeitssteuerung von Led6 gesteuert.
 * Der Tastgrad des PWM-Generators wird über der Potentiometer ,
 * die Spannung wird gemessen und über den exponentiellen Glättungsfilter
 * geglättet .
 * Die Periodendauer wird als Parameter in μs angegeben (von 0 bis 65536 μs )
 * das PWM-Signal wird per Interrupt auf LED7 ausgegeben..
 */
void task4() {
	// Die LEDs werden definiert
	BusOut leds(BB_LED_0, BB_LED_1, BB_LED_2, BB_LED_3, BB_LED_4, BB_LED_5);

	// Ein Objekt vom Typ CPwmGenerator3 wird erzeugt
	CPwmGenerator4 variableGenerator(20000);

	// Filter für Messwerte wird erzeugt
	CExponentialFilter filter(0.05);

	// das PWM-Signal wird auf LED7 ausgegeben.
	DigitalOut pwmOut(BB_LED_7);
	variableGenerator.setPwmOut(&pwmOut);
	while (true) {
		// Duty Cycle wird aus dem  Potentiometer gerechnet und gesetzt
		variableGenerator.setDutyCycle(
				(uint8_t) ((filter << poti.read()) * 100));
		// Bits 15-10 von CNT auf LEDs 5-0 anzeigen.
		leds = (TIM3->CNT >> 10) & 0x3f;
	}
}


/**
 * die letzten vier Stellen meiner Matriklelnummer werden auf
 * den ssd angezeigt
 * die Steuerung von SSD-Digits per Interrupts getestet.
 */
void task5() {
	// lokales Objekt zur Steuerung der Segmenten
	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);

	// Erzeugung für SSD wird vorbereitet
	DigitalOut *digitalOuts[4] = { &ssdDigit3, &ssdDigit2, &ssdDigit1,
			&ssdDigit0 };
	CSevenSegmentDisplay ssd(&ssdSegments, digitalOuts);

	// Led 0 defienieren
	DigitalOut led0(BB_LED_0);

	// die letzten vier Stellen meiner Matriklelnummer
	//werden auf SSD dargestellt
	ssd = 9510;

	// Helligkeit wird eingestellt
	ssd.setBrightness(5);

	while (true) {
		// Ausführung von Interrupt Handlern verhindern und LED ausschalten.
		__disable_irq();
		led0 = 0;

		// Auf einen Interrupt warten. Wenn er auftritt wird die CPU wieder
		// aktiviert, der Interrupt Handler wird aber noch nicht ausgeführt.
		// Der Interrupt "steht aus" ("is pending").
		__WFI();

		// LED einschalten und die Ausführung von Interrupt Handlern wieder
		// zulassen. Der Handler wird damit sofort nach __enable_irq()
		// aufgerufen.
		led0 = 1;
		__enable_irq();
	}
}

/**
 * Legt ein Objekt für die Sieben-Segment-Anzeige an und erlaubt mit einer
 * Eintastensteuerung mit Button 7 das Dimmen der Sieben-Segment-Anzeige.
 */
void task6(){
	// lokales Objekt zur Steuerung der Segmenten
	CShiftRegisterOutputExtender ssdSegments(&ssdSerialData,
			&ssdShiftRegisterClock, &ssdBufferRegisterClock);

	// Erzeugung für SSD wird vorbereitet
	DigitalOut *digitalOuts[4] = { &ssdDigit3, &ssdDigit2, &ssdDigit1,
			&ssdDigit0 };

	CSevenSegmentDisplay ssd(&ssdSegments, digitalOuts);
	ssd = 9510;
	CDebouncer debouncedKeys(&keys,200);
	ssd.setBrightness(100);
	CPolledTimer timer(50);
	while (true) {
		// überprüfen , ob die Heilligkeit maximal ist
		if (ssd.getBrightness() == 100) {
			// Solange die Helligkeit ungleich 5 und die linke taster gedrückt
			while (ssd.getBrightness() != 5 && debouncedKeys & 0x80) {
				// jede 50 ms die Helligkeit um 1 erniedrigen
				if (timer.timeReached()) {
					ssd.setBrightness(ssd.getBrightness() - 1);
				}
			}
		}
		//max Helligkeit noch nicht erreicht
		else {
			// Solange die Helligkeit ungleich 100 und die linke taster gedrückt
			while (debouncedKeys & 0x80) {
				while (ssd.getBrightness() != 100) {
					// jede 50 die Helligkeit um 1 erhöhen
					if (timer.timeReached()) {
						ssd.setBrightness(ssd.getBrightness() + 1);
					}
				}
			}
		}

	}
}


/**
 * Funktion, die der Timer 3 IRQ Handler aufrufen soll. Ermöglicht
 * die Konfiguration unterschiedlicher effektiver IRQ Handler je
 * nach Nutzung des Timers.
 *
 * Die aufzurufende Funktion muss im Rahmen der Konfiguration des
 * Interrupts zugewiesen werden.
 */
function<void()> activeTIM3_IRQHandler;

/**
 * Interrupt-Handler für Timer 3. Der Handler ruft die Funktion auf,
 * die der globalen Variablen activeTIM3_IRQHandler zugewiesen wurde.
 */
extern "C" void TIM3_IRQHandler() {
	activeTIM3_IRQHandler();
}


/**
 * Haupt- oder Einstiegsfunktion des Programms. Wird beim Start nach der
 * Initialisierung der globalen Variablen (bei Objekten: Aufruf des
 * Konstruktors) ausgeführt.
 */
int main() {

	keys.mode(PullDown);

	// Aufruf der Funktion, die die Lösung der gerade bearbeiteten Aufgabe
	// implementiert.

	//task1();
	//task2();
	//task3();
	//task4();
	//task5();
	task6();

	while (true) {
	}
}
