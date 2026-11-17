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

refer to the schematics under appropiate sections to get wiring setup
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
