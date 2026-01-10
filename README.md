# STM32 Kinematics Engine

**A deterministic, real-time velocity estimator for athletic performance, built on Bare-Metal C++.**
Prototype Engine:
<img width="1288" height="957" alt="physics engine" src="https://github.com/user-attachments/assets/6495552f-4974-4653-8e4f-f2b3ed4990cf" />

## The Objective
My goal is to build a "Velocity Based Training" (VBT) unit—a small embedded device that straps to a barbell to measure "explosiveness" rather than just raw strength.

In weight training, moving a bar at **0.8 m/s** builds power, while **0.5 m/s** builds strength. The problem is, I can't feel that difference, and commercial VBT tools are incredibly expensive. This project is my solution: a custom "Kinematics Engine" that calculates velocity in real-time and provides instant feedback (Green LED = Fast / Red LED = Slow).

## The Hardware
* **Microcontroller:** STM32 Nucleo-F411RE (ARM Cortex-M4)
* **Sensor:** MPU6050 (6-Axis Accelerometer & Gyroscope)
* **Interface:** UART (for Telemetry) & GPIO (for visual feedback)

## The "Bare Metal" C++ Approach
I chose to write this firmware in **C++** using a "bare metal" approach. While STM32 usually defaults to C, I converted the project to C++ to leverage **Object-Oriented Programming (OOP)** without sacrificing performance.

### Why C++ for `main.cpp`?
Most embedded tutorials stick to procedural C with global variables. I wanted to apply software engineering principles to hardware:
1.  **Encapsulation:** The physics logic is wrapped in a `PhysicsEngine` class. This keeps the state (current velocity, previous timestamp, integration errors) contained within the object, preventing the "global variable spaghetti" common in firmware.
2.  **Driver Abstraction:** The `MPU6050` driver is a class that accepts an I2C handle. This makes the code reusable and cleaner to read (`sensor.getAcceleration()`).
3.  **Low-Level Control:** Despite using Classes, I am still bypassing heavy libraries to manipulate hardware registers directly (e.g., writing to `RCC->AHB1ENR` to control clock gating).

## System Architecture
The system functions as a real-time data pipeline:
1.  **Input:** The MPU6050 sensor provides raw acceleration data via I2C.
2.  **Processing:** A hardware timer triggers an interrupt (aiming for 100Hz-1kHz) to ensure deterministic timing.
3.  **Math:** The `PhysicsEngine` class performs numerical integration (Euler method) to convert Acceleration (m/s²) → Velocity (m/s).
4.  **Feedback:**
    * **Visual:** The Green LED toggles if the rep was "Explosive" (>1.0 m/s).
    * **Telemetry:** Live velocity data is streamed via UART to a laptop console for analysis.

## What I Learned (So Far)
* **The C++ Linker "Name Mangling" Trap:** I learned that C++ compilers rename functions to support overloading. This broke the STM32 startup file which looks for `main`. I had to use `extern "C"` to force the compiler to treat the entry point as standard C.
* **Register-Level Control:** I moved away from `HAL_GPIO_WritePin` to directly manipulating the **ODR** (Output Data Register) and **MODER** (Mode Register). This taught me exactly how the silicon controls electricity at the bit level.
* **Physics in Firmware:** Implementing a physics engine on a chip requires handling "Drift." I learned that even tiny sensor errors accumulate over time during integration, requiring "Deadzone" logic to ignore static noise.

## Project Status
* [x] Bare Metal GPIO & Clock Configuration
* [x] UART Telemetry Driver
* [x] C++ Project Conversion & Linker Setup
* [x] Physics Engine Implementation (Integration Logic)
* [ ] MPU6050 I2C Driver (Skeleton Complete, awaiting hardware integration)
* [ ] DMA Implementation (Planned for efficiency)

## How to Build
This project is built using **STM32CubeIDE**.
1.  Clone the repo.
2.  Import as an "Existing STM32 Project."
3.  Ensure your build settings allow `float` printing (`-u _printf_float` in linker flags).
4.  Build & Run on NUCLEO-F411RE.
