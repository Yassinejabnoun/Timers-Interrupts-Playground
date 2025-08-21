/*
 * CPwmGenerator3.cpp
 *
 *  Created on: 22.01.2022
 *      Author: jabno
 */

#include "CPwmGenerator3.h"

CPwmGenerator3::CPwmGenerator3(uint16_t period) {
	// Timer mit Takt versorgen
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// Verwendete Peripheriegeräte konfigurieren.
	configureGpioPin();
	configureTimer(period);
	setDutyCycle(0);
}

void CPwmGenerator3::configureGpioPin() {
	// GPIO-Pin PC6 auf alternate function 2 konfigurieren.
	MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODE6, 2 << GPIO_MODER_MODE6_Pos);
	MODIFY_REG(GPIOC->AFR[0], GPIO_AFRL_AFRL6,
			GPIO_AF2_TIM3 << GPIO_AFRL_AFSEL6_Pos);

}

void CPwmGenerator3::configureTimer(uint16_t period) {

	// Prescaler auf berechneten Wert für 1 MHz output setzen.
	// Die Periodendauer ist variabel einstellbar
	TIM3->PSC = 84 - 1;
	TIM3->ARR = period - 1;

	// Compare register 1 auf 10 * 2048 setzen
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

void CPwmGenerator3::setDutyCycle(uint8_t percent) {
	// Überprüfen ob der eingegbene Parameter größer 100 ist
	if (percent > 100) {
		// wenn es der Fall ist , wird 100 übernommen .
		percent = 100;
	}
	// Das Compare Register auf dem gewünschten Taktgrad setzen , aus dem ARR
	// glesene Wert
	TIM3->CCR1 = ((TIM3->ARR + 1) * percent) / 100;

}
