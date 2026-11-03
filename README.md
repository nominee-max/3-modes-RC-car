# 3-Modes RC Car

An Arduino-based RC car platform with multiple interchangeable driving modes:

1. **Line following** - Uses left and right line sensors to follow a track.
2. **Obstacle avoidance** - Uses an ultrasonic distance sensor mounted on a servo to detect obstacles and choose a clearer direction.
3. **Hand-gesture control** - Uses an MPU6050 motion sensor and nRF24L01 radios to send tilt commands to the car wirelessly.

The repository includes the firmware for each mode and KiCad design files for the line-follower and hand-gesture-control hardware.

## Project Description

This project explores several ways to control the same two-wheel drive robot chassis. Each mode is implemented as a separate Arduino sketch so it can be tested and uploaded independently.

### Implemented features

- Two-motor differential drive using an L298N-style motor driver.
- Line-following logic based on two digital sensors.
- Ultrasonic ranging with servo-assisted left/right scanning.
- Wireless control over nRF24L01 modules.
- MPU6050-based tilt input using the Digital Motion Processor (DMP).
- Motor stop behavior when the hand-gesture radio signal is lost.
- KiCad schematics and PCB layouts for supporting hardware.

### Repository structure

```text
Firmware/
├── controller_radio/
├── hand_gesture_radio/
│   ├── Reciver_handgesture/
│   └── Transmitter_HandGesture/
├── line_following/
└── obstacle_avoider/

Hardware/
├── Line Follower/
└── hand gesture control/
```

### Important hardware notes

- The firmware targets Arduino-compatible boards and assumes the pin assignments in each sketch.
- Motor direction and enable pins differ between the autonomous and hand-gesture sketches. Confirm wiring against the selected sketch before powering the car.
- The hand-gesture transmitter and receiver must use the same nRF24L01 address and radio data rate.
- The transmitter requires an MPU6050, and both radio endpoints require an nRF24L01 module.
- Use a suitable external motor power supply and common ground. Do not power motors directly from an Arduino GPIO pin.

## Getting Started

### Prerequisites

Install the Arduino IDE and the libraries required by the mode you want to run:

- `Servo` (included with the Arduino IDE) for obstacle avoidance.
- `SPI` and `Wire` (included with the Arduino IDE) for radio and I2C communication.
- `RF24` for the nRF24L01 transmitter and receiver.
- `I2Cdev` and `MPU6050` for the hand-gesture transmitter.

You will also need:

- An Arduino-compatible controller board.
- A two-motor robot chassis and motor driver.
- A USB cable for uploading sketches.
- Mode-specific sensors and modules.
- A separate, appropriate supply for the motors and a shared ground.

### 1. Clone and open the project

```bash
git clone https://github.com/nominee-max/3-modes-RC-car.git
cd 3-modes-RC-car
```

Open the sketch for the desired mode in the Arduino IDE.

### 2. Wire the selected mode

Use the pin definitions at the top of each sketch as the source of truth. The currently implemented mappings include:

#### Line following

The line-following sketch uses:

- Motor control pins: `2`, `3`, `4`, and `7`
- Right line sensor: `A0`
- Left line sensor: `A1`

Upload `Firmware/line_following/line_following.ino`, then place both sensors over the track and adjust sensor logic or motor wiring if the car turns in the wrong direction.

#### Obstacle avoidance

The obstacle-avoidance sketch uses:

- Motor enable pins: `5` and `6`
- Motor direction pins: `8`, `9`, `10`, and `11`
- Ultrasonic trigger: `A5`
- Ultrasonic echo: `A4`
- Scanning servo: `3`

Upload `Firmware/obstacle_avoider/obstacle_avoider.ino`. The car drives forward until an obstacle is detected within approximately 40 cm, reverses briefly, scans both sides, and turns toward the side with more clearance.

#### Hand-gesture control

Upload the two sketches to separate boards:

1. Upload `Firmware/hand_gesture_radio/Transmitter_HandGesture/Transmitter_HandGesture.ino` to the MPU6050 transmitter.
2. Upload `Firmware/hand_gesture_radio/Reciver_handgesture/Reciver_handgesture.ino` to the car receiver.
3. Install the RF24, I2Cdev, and MPU6050 libraries before compiling.
4. Open the serial monitor at `115200` baud to inspect initialization and control data.

The transmitter maps MPU6050 pitch and roll to X/Y radio values. The receiver interprets those values as forward, reverse, left, right, or stop commands. If no packet is received for 500 ms, the receiver stops the motors.

### 3. Upload and test safely

1. Test with the drive wheels off the ground.
2. Verify that the motors stop at startup and when radio communication is interrupted.
3. Check motor direction before placing the car on a track.
4. Disconnect USB power before changing wiring.
5. Test one firmware mode at a time; do not upload autonomous and radio-control sketches to the same controller expecting them to run together.

## Roadmap

- [ ] Add a single mode-selection interface instead of requiring a separate firmware upload.
- [ ] Document complete wiring diagrams and connector labels for every mode.
- [ ] Add a shared motor-control library to reduce duplicated drive logic.
- [ ] Add configurable motor speed, sensor thresholds, and obstacle distance settings.
- [ ] Improve line-following behavior with proportional or PID steering.
- [ ] Add radio packet validation and clearer transmitter/receiver failsafe states.
- [ ] Add battery voltage monitoring and low-battery protection.
- [ ] Add calibration procedures for the MPU6050, line sensors, and ultrasonic sensor.
- [ ] Add build and upload instructions for supported Arduino boards.
- [ ] Add automated checks or hardware-in-the-loop tests where practical.

## General README Template

The following outline can be reused for future project documentation:

```markdown
# Project Name

Short summary of what the project does and who it is for.

## Project Description

Explain the problem, the main features, and the current implementation status.

## Features

- Feature one
- Feature two
- Feature three

## Repository Structure

Describe the important folders and files.

## Requirements

List hardware, software, dependencies, and supported platforms.

## Getting Started

### Installation

Show how to clone the repository and install dependencies.

### Configuration

Document required settings, wiring, environment variables, or calibration.

### Running

Provide the commands or steps needed to run, flash, or use the project.

## Usage

Include examples, operating instructions, and expected behavior.

## Testing

Explain how to run tests or perform a hardware verification procedure.

## Roadmap

- [ ] Planned improvement
- [ ] Planned improvement

## Contributing

Explain how to report issues, propose changes, and submit pull requests.

## License

State the project license.
```

