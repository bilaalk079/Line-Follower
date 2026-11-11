# Line Following Robot

An Arduino-based autonomous line-following robot that uses three infrared (IR) sensors and differential motor speed control to detect and follow a track.

The robot interprets sensor readings to determine its position relative to the line and adjusts the speed of its left and right motors accordingly.

## Overview

The robot uses a three-sensor configuration consisting of:
- Left IR sensor
- Right IR sensor
- Middle IR sensor

By combining the readings from these sensors, the robot determines whether it is moving straight, drifting towards either side, or approaching a sharp turn.

Motor adjustments are handled through PWM signals, allowing the robot to vary the speed of each motor independently.

## Hardware Components

| Component | Description |
|---|---|
| Arduino | Main microcontroller |
| 3 × IR Sensors | Line detection |
| 2 × DC Motors | Differential drive |
| Motor Driver | Motor direction and speed control |
| Robot Chassis | Mechanical structure |
| Battery | Power supply |

## Pin Configuration

### Motor Connections

| Arduino Pin | Function |
|---|---|
| D3 | Right motor forward |
| D5 | Right motor backward |
| D6 | Left motor forward |
| D9 | Left motor backward |

### IR Sensor Connections

| Arduino Pin | Sensor |
|---|---|
| D7 | Left IR sensor |
| D4 | Right IR sensor |
| D12 | Middle IR sensor |

## How It Works

### 1. Sensor-Based Line Detection

The robot continuously reads the digital outputs of its three IR sensors.

The sensor readings are interpreted using a predefined error mapping.

| Left | Right | Middle | Interpretation | Error |
|---|---|---|---|---|
| 0 | 0 | 1 | Straight | 0 |
| 1 | 0 | 1 | Slight left | -2 |
| 1 | 0 | 0 | Far left | -4 |
| 0 | 1 | 1 | Slight right | +2 |
| 0 | 1 | 0 | Far right | +4 |
| 0 | 0 | 0 | Line lost | Previous error |
| 1 | 1 | 0 | Ambiguous detection | Previous error |
| 1 | 1 | 1 | Stop | 10 |

*Note: These readings assume the sensors output HIGH (1) when detecting the line and LOW (0) otherwise.*

### 2. Error-Based Motor Control

The robot uses the calculated error to determine the appropriate motor speeds.

A proportional gain (Kp) determines how aggressively the robot responds to deviations from the line.

Current control parameters:

```cpp
const int baseSpeed = 150;
int Kp = 8;
```

The correction is calculated using:

```cpp
int correction = abs(Kp * error);
```

The resulting correction is applied differently to the left and right motors depending on the direction of deviation.

### 3. Differential Steering

The robot adjusts its motor speeds to correct its direction.

- **Straight movement:** Both motors operate at the base speed.
- **Left correction:** The left motor receives a higher speed while the right motor is reduced.
- **Right correction:** The right motor receives a higher speed while the left motor is reduced.

The current implementation uses different speed reductions for the inner and outer wheels.

```cpp
int outerBase = baseSpeed - (baseSpeed * 0.2);
int innerBase = baseSpeed - (baseSpeed * 0.9);
```

PWM values are constrained between 0 and 255 to prevent invalid motor speed commands.

## Sharp-Corner Handling and Hardware Constraints

One of the challenges encountered during development was maintaining reliable line tracking through sharp corners.

Due to hardware limitations and the turning characteristics of the robot, the inner motor's speed was significantly reduced during directional corrections. However, the resulting PWM values were sometimes too low for the motor to overcome static friction and initiate movement.

This caused the inner motor to appear completely stationary during certain turns, effectively producing a pivot-like turning behaviour.

Although this was not an explicitly implemented motor-stopping mechanism, the resulting behaviour helped the robot navigate sharper deviations without consistently overshooting the track.

This highlighted an important practical consideration in DC motor control: a motor's commanded PWM value does not necessarily translate into physical movement, particularly below its minimum starting threshold.

## Code Structure

The Arduino sketch is organised into the following functions:

| Function | Responsibility |
|---|---|
| `setup()` | Initialises motor and sensor pins |
| `calculateError()` | Interprets sensor readings and determines deviation |
| `driveMotors()` | Applies PWM signals to the motors |
| `stopMotors()` | Stops both motors |
| `loop()` | Continuously reads sensors, calculates corrections, and controls movement |

## Current Limitations

- The robot relies on predefined sensor combinations rather than continuous position measurement.
- The proportional controller uses a fixed gain.
- There is no integral or derivative component.
- Lost-line recovery relies on the previous error value.
- Sharp turns can require deliberate motor stoppage due to hardware limitations.
- Performance depends on sensor positioning, track contrast, motor characteristics, and battery voltage.


## Conclusion

This project explores the fundamentals of autonomous mobile robotics through infrared sensing, differential drive, PWM motor control, and error-based steering.

Beyond getting the robot to follow a line, the project provided practical experience in sensor integration, motor control, debugging, and adapting control strategies to real-world hardware limitations.

**The objective was not simply to make a robot move, but to understand how sensing, control logic, and physical hardware work together to achieve autonomous navigation.**
