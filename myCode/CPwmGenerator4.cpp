/*
 * CPwmGenerator4.cpp
 *
 *  Created on: 23.01.2022
 *      Author: jabno
 */

#include "CPwmGenerator4.h"

CPwmGenerator4::CPwmGenerator4(uint16_t period) {
	// Timer mit Takt versorgen
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// Verwendete Peripheriegeräte konfigurieren.
	configureGpioPin();
	configureTimer(period);
	setDutyCycle(0);
}

void CPwmGenerator4::configureGpioPin() {
	// GPIO-Pin PC6 auf alternate function 2 konfigurieren.
	MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, 2 << GPIO_MODER_MODE6_Pos);
	MODIFY_REG(GPIOC->AFR[0], GPIO_AFRL_AFRL6,
			GPIO_AF2_TIM3 << GPIO_AFRL_AFSEL6_Pos);

}

void CPwmGenerator4::configureTimer(uint16_t period) {

	// Prescaler auf berechneten Wert für 1 MHz output setzen.
	// Die Periodendauer ist variabel einstellbar
	TIM3->PSC = 84 - 1;
	TIM3->ARR = period - 1;

	// Compare register 1 auf 10 * 2048 setzen .
	TIM3->CCR1 = 10 << 11;

	// PWM1 konfigurieren und Ausgabe des OC1REF einschalten.
	MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_OC1M, 6<< TIM_CCMR1_OC1M_Pos);

	//Compare 1 output enable Bit wird eingesetzt
	SET_BIT(TIM3->CCER, TIM_CCER_CC1E);

	// Update-Event zum Aktualisieren der gepufferten Register erzwingen
	SET_BIT(TIM3->EGR, TIM_EGR_UG);

	// UpdateInterruptFlag wird gelöscht
	CLEAR_BIT(TIM3->SR, TIM_SR_UIF);

	// Timer wird danach gestartet
	SET_BIT(TIM3->CR1, TIM_CR1_CEN);
}


void CPwmGenerator4::handleInterrupt() {
	// beim Update Event wird das eingestellte Output geschaltet
	if (READ_BIT(TIM3->SR, TIM_SR_UIF)) {

		// Update Flag wird zurückgesetzt
		CLEAR_BIT(TIM3->SR, TIM_SR_UIF);

		//  Led auf High pegel gelegt
		*m_pwmOut = 1;
	}

	// wenn Flag für CRR1 eingesetzt ist
	if (READ_BIT(TIM3->SR, TIM_SR_CC1IF)) {
		// Flag wird zurückgesetzt
		CLEAR_BIT(TIM3->SR, TIM_SR_CC1IF);

		// Das eingestellte Output wird ausgeschaltet
		*m_pwmOut = 0;
	}
}

void CPwmGenerator4::configureInterrupt() {

	// Klassenmethode als Interrupt-Handler einstellen
	activeTIM3_IRQHandler = bind(&CPwmGenerator4::handleInterrupt, this);

	// Interrupt am Nested Vectored Interrupt Controller wird eingeschaltet
	__NVIC_EnableIRQ(TIM3_IRQn);

	// TimerInterrupt für Updates  wird eingeschaltet
	SET_BIT(TIM3->DIER, TIM_DIER_UIE);

	// TimerInterrupt für CaptureCompare1 wird eingeschaltet
	SET_BIT(TIM3->DIER, TIM_DIER_CC1IE);
}

void CPwmGenerator4::setDutyCycle(uint8_t percent) {
	// Überprüfen ob der eingegbene Parameter größer 100 ist
	if (percent > 100) {
		// wenn es der Fall ist , wird 100 übernommen .
		percent = 100;
	}
	// Das Compare Register auf dem gewünschten Taktgrad setzen , aus dem ARR
	// glesene Wert
	TIM3->CCR1 = ((TIM3->ARR + 1) * percent) / 100;
}

void CPwmGenerator4::setPwmOut(DigitalOut *pin) {
	m_pwmOut = pin;
}
