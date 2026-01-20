# Smart Drone Hangar

## 1. Introduction
This project implements an embedded system for an automated drone hangar. The system consists of two main subsystems communicating via serial line:
1.  **Drone Hangar (Arduino):** Manages sensors, actuators, and control logic.
2.  **Drone Remote Unit (PC):** A GUI for remote control and telemetry monitoring.

## 2. System Architecture (Drone Hangar)
The Arduino code follows a **Task-Based Architecture** with a **Cooperative Scheduler**.

The scheduler executes tasks based on a **50ms** base tick:

* **SensingTask (Period: 50ms):** Acquires data from sensors (Sonar, Temperature, PIR) and updates the shared `Context`.
* **LogicTask (Period: 100ms):** Implements the **Finite State Machine (FSM)**. It controls the actuators (Servo, LEDs, LCD) based on the system state and sensor data.
* **CommTask (Period: 200ms):** Handles asynchronous serial communication with the PC (receiving commands, sending telemetry).

## 3. Finite State Machine (FSM)
The FSM (implemented in `LogicTask`) manages the system behavior through the following states:

* **INSIDE:** Initial state. Door closed, waiting for takeoff command.
* **TAKEOFF:** Door open, LED L2 blinking. Transitions to *DroneOut* when distance > D1.
* **DRONEOUT:** Drone is outside. Door closed. Waiting for landing command and PIR detection.
* **LANDING:** Door open, LED L2 blinking. Transitions to *Inside* when distance < D2.
* **PREALARM:** Triggered by `Temp1`. Suspends new operations but **allows ongoing takeoff/landing to complete**.
* **ALARM:** Triggered by `Temp2` or timeout. **Immediately closes the door** for safety, activates LED L3 (Red), and locks the system until the RESET button is pressed.

```mermaid
stateDiagram-v2
    [*] --> INSIDE

    state "INSIDE" as INSIDE
    state "TAKE OFF" as TAKEOFF
    state "DRONE OUT" as DRONEOUT
    state "LANDING" as LANDING
    state "PRE-ALARM" as PREALARM
    state "ALARM" as ALARM

    INSIDE: Door Closed, L1 ON
    INSIDE: Display "DRONE INSIDE"

    TAKEOFF: Door Open, L2 Blink
    TAKEOFF: Display "TAKE OFF"

    DRONEOUT: Door Closed
    DRONEOUT: Display "DRONE OUT"

    LANDING: Door Open, L2 Blink
    LANDING: Display "LANDING"

    PREALARM: L2 Blink (if moving)
    PREALARM: Allow move completion

    ALARM: Door Closed, L3 ON
    ALARM: Display "ALARM"

    INSIDE --> TAKEOFF: CMD:TAKEOFF
    TAKEOFF --> DRONEOUT: Dist > D1 (for T1)
    DRONEOUT --> LANDING: CMD:LAND & Motion
    LANDING --> INSIDE: Dist < D2 (for T2)

    INSIDE --> PREALARM: Temp >= Temp1 (for T3)
    TAKEOFF --> PREALARM: Temp >= Temp1 (for T3)
    LANDING --> PREALARM: Temp >= Temp1 (for T3)

    PREALARM --> INSIDE: Temp < Temp1
    PREALARM --> TAKEOFF: Temp < Temp1
    PREALARM --> LANDING: Temp < Temp1

    PREALARM --> ALARM: Time > T4
    ALARM --> INSIDE: RESET (if prev=INSIDE)
    ALARM --> DRONEOUT: RESET (if prev=OUT)

```

## 4. Drone Remote Unit (PC Subsystem)
The PC application is developed in **Java** using **Swing** for the GUI and **JSSC** for Serial Communication. It follows the **Observer pattern** to separate the UI (`DRUView`) from the logic (`DRUController`). It allows operators to send Takeoff/Land commands and view real-time telemetry (State, Distance, Temp, Motion).