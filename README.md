# UBC Sailbot – Rudder Control Subsystem
UBC Sailbot is an engineering design team at the University of British Columbia focused on building small-to-medium autonomous sailboats of collecting oceanic and atmospheric data in the Pacific Ocean with the hopes of integrating our technology with the fields of climate research and sustainability. Our current project is POLARIS, which is a 3-meter fully autonomous sailboat. More details can be found [here](https://www.ubcsailbot.org/discover-polaris).

As part of UBC Sailbot’s **Drive (DRV) subteam**, I have been responsible for developing the embedded hardware and firmware for the **rudder control subsystem** of an autonomous sailboat designed for open-ocean navigation. My role combines hardware design and embedded programming.


## Hardware Implementation

### Motor Controller
The motor controller we selected required a **0–5 V analog input** to control speed and a separate **digital direction signal**. Since the STM32 Nucleo board outputs a maximum of 3.3 V, I designed a simple signal conditioning circuit using op-amps:

- Motor Speed: We initially planned to use a non-inverting amplifier to scale the STM32’s 0–3.3 V DAC output up to 5 V. However, testing showed that a full 3.3 V signal already saturated the motor controller’s speed input. As a result, we opted for a voltage follower instead, preserving signal integrity without unnecessary gain.
- Motor Direction: A comparator circuit generates the required 0/5 V digital signal for direction control. A GPIO pin on the STM32 sets the desired direction (high or low), which is fed into an LM311 comparator. The LM311 compares the 3.3 V input against a fixed reference using a 5 V supply. Its open-collector output, pulled up to 5 V, cleanly shifts the logic level to meet the motor controller’s 5 V requirement.

#### Motor Control Schematic

This schematic shows the analog and digital signal conditioning circuits used to interface the STM32 with the motor controller. It includes the DAC scaling stage and the comparator used to drive the direction pin.

<p align="center">
  <img width="574" alt="Motor_Schematic" src="https://github.com/user-attachments/assets/453c6d5b-dc5c-494b-9a7c-b075dcc98294" />
</p>

#### Encoder Interface Schematic

This schematic captures the UART-based interface with the absolute encoder using an RS-485 transceiver for noise resilience and protocol compatibility with MODBUS.

<p align="center">
  <img width="541" alt="Encoder_Schematic" src="https://github.com/user-attachments/assets/014afe0d-5d83-40bb-973f-6bd3780a1d69" />
</p>


#### RUDR Control PCB Layout

The completed PCB layout for the rudder control board ("RUDR") integrates motor drive circuitry, encoder interface, and CAN transceivers into a compact design suitable for mounting within the boat.

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

**Note:** The actual source code for the rudder control firmware is not included in this repository as it is still undergoing development, validation, and internal review. To protect against the premature release of incomplete or unverified control logic, only high-level descriptions, architecture diagrams, and representative pseudocode are provided in the file **pseudocode.c**. This ensures that the design intent and control structure are communicated clearly without exposing unstable or incomplete implementations.

### Encoder Firmware
The encoder communicates via MODBUS over UARTThe MODBUS protocol layer was implemented by other members of the team, and I worked closely with them to integrate it into my firmware as a reusable encoder driver. While the low-level MODBUS communication code is not published here, the structure and integration logic are documented to illustrate how the encoder fits into the overall system. Once testing is finalized, we may consider sharing selected firmware modules or representative pseudocode snippets.

Below is a block diagram illustrating the overall hardware and firmware design of the rudder control subsystem:

<p align="center">
  <img width="865" alt="Block Diagram" src="https://github.com/user-attachments/assets/6eb97e8b-2fde-428a-9df5-7170de5df493" />
</p>


### Closed-Loop PI Controller

The diagram below provides a closer look at the **closed-loop PI control structure**, showing how angle feedback, motor drive, and control logic are interconnected:

<p align="center">
  <img width="1039" alt="Closed-Loop PID" src="https://github.com/user-attachments/assets/f029621a-d185-4a7c-afb2-47f16a94de04" />
</p>

## Validation & Testing

Validation began with basic hardware bring-up to ensure all components performed according to design specifications. After assembling the PCB, continuity tests and power checks were conducted to verify clean power delivery, correct signal routing, and safe operation of all analog and digital subsystems.

Once the hardware was confirmed functional, we moved to **dryland testing** using a **3D-printed rudder jig**. This allowed us to simulate mechanical rotation and validate closed-loop control behavior in a safe, repeatable environment. The jig mounted the encoder coaxially with a mock rudder shaft, enabling us to visualize rudder movement and assess the responsiveness of the PI controller under step and ramp inputs.

Key testing milestones included:
- Verifying that the DAC output correctly scaled to the expected analog voltage range (0–3.3 V) and saturated the motor controller as intended
- Confirming that the comparator circuit produced stable 0/5 V direction signals
- Testing the encoder communication via MODBUS over UART, and validating real-time angle readings under movement
- Evaluating the performance of the PI controller, tuning gains to minimize overshoot and steady-state error across a range of setpoints

All test results, scope captures, and tuning iterations were documented in **Confluence** to support reproducibility and team-wide visibility. The current firmware and hardware configuration are now stable and prepared for integration into the full boat system, with on-water trials scheduled next.

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
- Revise the current PCB for final launch
