# UBC Sailbot – Rudder Control Subsystem

As part of UBC Sailbot’s **Drive (DRV) subteam**, I have been responsible for developing the embedded hardware and firmware for the **rudder control subsystem** of an autonomous sailboat designed for open-ocean navigation. My role combines hardware design and embedded programming.


## Hardware Implementation

### Motor Controller
The motor controller we selected required a **0–5 V analog input** to control speed and a separate **digital direction signal**. Since the STM32 Nucleo board outputs a maximum of 3.3 V, I designed a simple signal conditioning circuit using op-amps:

- Motor Speed: We initially planned to use a non-inverting amplifier to scale the STM32’s 0–3.3 V DAC output up to 5 V. However, testing showed that a full 3.3 V signal already saturated the motor controller’s speed input. As a result, we opted for a voltage follower instead, preserving signal integrity without unnecessary gain.
- Motor Direction: A comparator circuit generates the required 0/5 V digital signal for direction control. A GPIO pin on the STM32 sets the desired direction (high or low), which is fed into an LM311 comparator. The LM311 compares the 3.3 V input against a fixed reference using a 5 V supply. Its open-collector output, pulled up to 5 V, cleanly shifts the logic level to meet the motor controller’s 5 V requirement.

<p align="center">
  <img width="574" alt="Motor_Schematic" src="https://github.com/user-attachments/assets/453c6d5b-dc5c-494b-9a7c-b075dcc98294" />
</p>

<p align="center">
  <img width="541" alt="Encoder_Schematic" src="https://github.com/user-attachments/assets/014afe0d-5d83-40bb-973f-6bd3780a1d69" />
</p>

<p align="center">
  <img width="449" alt="RUDR_PCB" src="https://github.com/user-attachments/assets/957852b4-f12a-496d-b785-04155c8a6157" />
</p>

### Encoder

The rudder’s angular position was measured using an **absolute encoder by BRITER**. 

## Firmware Development

### Motor Controller Firmware

The rudder firmware is centered around a **Proportional-Integral (PI) controller** implemented in C on the STM32 Nucleo platform. A PI controller is ideal for rudder control because it effectively balances two key objectives:
- The **proportional term** responds immediately to angular error, applying correction proportional to the deviation.
- The **integral term** accumulates residual error over time, eliminating steady-state offsets caused by environmental forces like wind or water resistance.

Together, these characteristics enable the rudder to **converge accurately and smoothly** to a commanded angle without excessive overshoot or oscillation—critical for stable autonomous heading control.

The controller runs at **50 Hz**, matching the update rate of the absolute encoder. It produces a **signed control signal**:
- The **magnitude** determines motor speed, output via the STM32's DAC.
- The **sign** sets direction, passed to a GPIO that feeds into a comparator stage to drive the motor controller's direction pin.

The firmware also integrates with the team's CAN bus library, subscribing to rudder setpoints from the navigation node and publishing current angle and status for telemetry. 

### Encoder Firmware
- The encoder communicates via MODBUS over UARTThe MODBUS protocol layer was implemented by other members of the team, and I worked closely with them to integrate it into my firmware as a reusable encoder driver.

<p align="center">
  <img width="1039" alt="Closed-Loop PID" src="https://github.com/user-attachments/assets/f029621a-d185-4a7c-afb2-47f16a94de04" />
  <img width="865" alt="Block Diagram" src="https://github.com/user-attachments/assets/6eb97e8b-2fde-428a-9df5-7170de5df493" />
</p>


## Validation Approach
First thin is to check the hardware and see if it works as intended.
First line of testing for the PID is dry-land testing. The team created a 3D-printed jig that allows the encoder to be mounted unto the motor shaft. Before integrating CAN, we can manually set an error and run the PID
Using Confluence and Github, we can track firmware versions, gain sets, and mechanical responses — each run logged with scope captures and encoder plots via serial monitor.

Below is a quick snippet of the PI controller in action:


https://github.com/user-attachments/assets/0d4bbd11-4ae1-4a0a-9f7d-93dab6fbd5bc


## Project Status

The rudder control hardware has been finalized and enclosed in a form factor suitable for direct integration into the boat. The PI controller is currently in the process of iterative tuning and is actively being validated over CAN communication. Final on-water trials are scheduled for late July 2025 following deck-level wiring. 

<p align="center">
  <img alt="RUDR Enclosure" src= "https://github.com/user-attachments/assets/2432d283-9607-4a43-b7ce-042da658d3d5" />
</p>

Next steps include:
- Performing in-water tuning under wave and wind disturbances
- Integrating with the navigation planner for autonomous heading control
