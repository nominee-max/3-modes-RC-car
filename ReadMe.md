# 3-Modes RC Car

An Arduino robot car that can drive itself or be driven wirelessly. The same two-wheel chassis runs in different modes. You pick a mode by wiring its sensors and uploading its sketch.

| Mode | How it drives | Main parts |
|------|---------------|------------|
| Line follower | Follows a dark line on the floor | 2 × IR sensors |
| Obstacle avoider | Drives forward, scans left and right when something is close, then turns toward the clearer side | HC-SR04 ultrasonic sensor on a servo |
| Hand gesture | Tilt your hand to steer the car | MPU6050 + 2 × nRF24L01 radios |
| Joystick *(in progress)* | Steer with a joystick remote | Joystick + 2 × nRF24L01 radios |

---

## Repository layout

```text
Firmware/                         Arduino sketches (.ino)
├── line_following/               Line follower mode
├── obstacle_avoider/             Obstacle avoider mode
├── hand_gesture_radio/
│   ├── Transmitter_HandGesture/  Runs on the glove/hand controller
│   └── Reciver_handgesture/      Runs on the car
└── controller_radio/
    ├── transmitter_joystick/     Runs on the joystick remote
    └── reciever_car/             Runs on the car

Hardware/                         KiCad schematics and PCB layouts
├── Line Follower/
├── ObstacleAvoider/
└── hand gesture control/         Transmitter and receiver schematics
```

---

## What you need

**Every mode**
- Arduino Uno or Nano (or another compatible board)
- 2-wheel-drive robot chassis with 2 DC motors
- L298N motor driver
- Battery pack for the motors (separate from the Arduino's USB power)
- Jumper wires and a USB cable

**Extra parts per mode**
- **Line follower:** 2 × IR line sensor modules
- **Obstacle avoider:** HC-SR04 ultrasonic sensor and a small servo (SG90)
- **Hand gesture:** a second Arduino, an MPU6050 motion sensor, 2 × nRF24L01 radio modules
- **Joystick:** a second Arduino, an analog joystick module, 2 × nRF24L01 radio modules

**Software**
- [Arduino IDE](https://www.arduino.cc/en/software)
- Libraries (install from *Sketch → Include Library → Manage Libraries*):
  - `RF24` by TMRh20, for the radio modes
  - `I2Cdev` and `MPU6050` by Jeff Rowberg, for the hand-gesture transmitter
  - `Servo`, `SPI` and `Wire` come with the Arduino IDE

---

## Quick start

1. **Clone the repo**
   ```bash
   git clone https://github.com/nominee-max/3-modes-RC-car.git
   ```
2. **Choose a mode** and open its `.ino` file from `Firmware/` in the Arduino IDE.
3. **Wire it up** using the pin tables below. The `#define` lines at the top of each sketch are the final word on pins.
4. **Select your board and port** under *Tools*, then click **Upload**.
5. **Power the motors** from the battery pack and place the car down.

For the radio modes, upload the **transmitter** sketch to the controller's Arduino and the **receiver** sketch to the car's Arduino.

---

## Wiring

### Motor driver (shared by all modes)

| L298N pin | Arduino pin |
|-----------|-------------|
| IN1 | D2 |
| IN2 | D3 |
| IN3 | D4 |
| IN4 | D7 |
| ENA | D5 (PWM speed) |
| ENB | D6 (PWM speed) |
| GND | GND (must be shared with the battery) |

> The line follower sketch only uses IN1 to IN4. Leave the ENA/ENB jumpers on the L298N so the motors run at full speed.

### Line follower

| Part | Arduino pin |
|------|-------------|
| Right IR sensor OUT | A0 |
| Left IR sensor OUT | A1 |

**How it works**

| Right | Left | Action |
|-------|------|--------|
| 1 | 1 | Forward |
| 1 | 0 | Turn right |
| 0 | 1 | Turn left |
| 0 | 0 | Stop |

If your sensors give the opposite values on your line or floor, flip the logic in `loop()` or adjust the sensors' sensitivity screws.

### Obstacle avoider

| Part | Arduino pin |
|------|-------------|
| HC-SR04 TRIG | A5 |
| HC-SR04 ECHO | A4 |
| Servo signal | D3 *(see Known issues)* |

The servo faces forward at 115°, looks right at 50° and left at 170°. When something is closer than **40 cm**, the car stops, backs up, checks both sides and turns toward the side with more room.

### Hand gesture (and joystick) radio

Both Arduinos connect to an nRF24L01 the same way:

| nRF24L01 pin | Arduino Uno/Nano pin |
|--------------|----------------------|
| VCC | **3.3V** (not 5V) |
| GND | GND |
| CE | D8 |
| CSN | D9 |
| SCK | D13 |
| MOSI | D11 |
| MISO | D12 |

Transmitter only: MPU6050 to the Arduino:

| MPU6050 pin | Arduino pin |
|-------------|-------------|
| VCC | 5V (or 3.3V) |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

**Controls** (hand gesture mode)

| Hand movement | Car |
|---------------|-----|
| Tilt forward | Forward |
| Tilt back | Backward |
| Tilt right | Turn right |
| Tilt left | Turn left |
| Hold level | Stop |

The car stops on its own if it hasn't heard from the transmitter for **500 ms**.

> **First run:** the transmitter waits for you to type any character in the Serial Monitor (115200 baud) before it calibrates the MPU6050. Keep it still and flat during calibration. To skip this wait, comment out `#define PRINT_DEBUG` in the transmitter sketch.

---

## Hardware files

The `Hardware/` folder has KiCad projects for each mode. Open the `.kicad_pro` file in [KiCad](https://www.kicad.org/) to view the schematic and PCB layout.

- `Line Follower/`: car wiring and IR sensor boards
- `ObstacleAvoider/`: car wiring with the HC-SR04 (custom symbol included)
- `hand gesture control/`: separate `Transmitter` and `reciever` schematics

---

## Tips and troubleshooting

- **Motors don't spin:** check that the battery ground and the Arduino ground are connected.
- **Car goes backward or spins the wrong way:** swap the two wires of that motor on the L298N.
- **Radio not working:** power the nRF24L01 from 3.3V and add a 10 µF capacitor across its VCC and GND. Make sure both sketches use the same address (`0xF9E8F0F0E1`) and data rate (250 kbps).
- **"Radio not detected" in the Serial Monitor:** recheck the CE/CSN and SPI wires.
- **MPU6050 connection failed:** check SDA/SCL and that the module is powered.
- **Debug output:** each sketch prints sensor values to the Serial Monitor. Match the baud rate: 9600 for the line follower, 115200 for the radio sketches.

---

## Known issues

- **Obstacle avoider:** the sketch uses old pin names (`enablePinA`, `RightMotorForward`, and others) that aren't defined, so it won't compile yet. The servo is also on D3, which is the same pin as IN2.
- **Radio receivers:** `loop()` sets the motors to full forward and resets `lastRecvTime` on every pass. Because of this, the car can creep forward and the 500 ms signal-loss stop never triggers.
- **Joystick transmitter:** currently a copy of the hand-gesture (MPU6050) transmitter. The joystick reading code still needs to be written.

---

## Future plans

- Mount all sensors at once and switch modes without rewiring or re-uploading.
- Finish the joystick remote.
- Add a project gallery with photos and videos.
