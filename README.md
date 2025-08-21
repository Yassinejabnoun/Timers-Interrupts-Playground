# Timers & Interrupts Playground ⏱️⚡

This project showcases **STM32 timer** usage for **hardware PWM**, **interrupt-driven PWM**,
and **timer-based 7-segment multiplexing**. It also demonstrates **brightness control** using the
compare register and **low-power operation** via `__WFI()` (Wait-For-Interrupt).

Originally inspired by a lab exercise, extended and documented here as a standalone project.

---

## 🎯 Features

- **Hardware PWM (TIM3)** with a clean class design and adjustable **duty cycle (0–100%)**.
- **Configurable period** (e.g., 20 ms) for flicker-free LED dimming.
- **Interrupt-driven PWM**: generate PWM entirely via timer interrupts and a GPIO pin.
- **Timer-driven 7-segment multiplexing (TIM10)**: update digits from a periodic timer IRQ.
- **Brightness control** per digit via compare interrupt (CCR vs. ARR).
- **Low-power loop** using `__WFI()` to wait for interrupts.

---

## 🛠 Hardware Setup

- STM32 Nucleo board (e.g., STM32F4xx)
- Breadboard:
  - 4-digit 7-segment (common anode) + driver transistors
  - LEDs / GPIO lines for inspection
  - Potentiometer as ADC source (for duty scaling)
- Optional: logic analyzer (e.g., PulseView) for timing verification

---

## 💻 Software Setup

- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)
- Mbed-style project layout
- (Optional) PulseView / Saleae for measurements

---

## 📂 Project Structure
TO DO

---

## 📸 Showcase

---

## 🚀 Getting Started

1. Clone the repository
   ```bash
   git clone https://github.com/Yassinejabnoun/Timers-Interrupts-Playground.git
   cd Timers-Interrupts-Playground
2. Import into STM32CubeIDE.
3. Wire the hardware as in docs/circuit.png.
4. Build & flash.

---

## 🎥 Demonstration

(Add your YouTube link here – thumbnail will be generated automatically)

---

## 📜 License

MIT License – free to use for personal or professional projects.


