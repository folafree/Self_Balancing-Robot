
# Introduction

This project fulfills the final project requirement for 
- ECE 528 Robotics and Embedded Systems

By:
- Sherifat Solebo

## Project Goal
- Design a self-balancing robot that keeps a platform level
  - Maintain a 0&deg; angle while the base is tilted or disturbed
- Apply embedded systems concepts on an Arduino based platform
  - Read pitch and roll from an MPU6050 IMU over the I2C bus


## Video Demonstration
- Demo: [Link](https://www.youtube.com/shorts/pJ3YCWHPwqo)

## Self Balancing Robot
![Robot Picture 1](./Images/self_leveling_robot.jpg)

## Background and Methodology

Implemented in Arduino C/C++, this project has achieved the following:

- Design and build a **two-axis self-balancing robot** to keep a platform level while the base is tilted or disturbed
  - Use a pan-tilt servo bracket to provide independent **pitch** and **roll** motion
  - Top plate sized for a standard cup and mounted so the center of mass is near the middle of the platform
 
- Integrated an **MPU6050 IMU** with the Arduino over the **I2C bus**
  
- Calibrates IMU offsets at startup (platform must be kept still during calibration)

- IMU angles are read using mpu.getAngleX()/getAngleY() (AngleX → pitch, AngleY → roll)
  - Uses a simple **low-pass filter** on the IMU angles to smooth measurements
    - `angle_filt = angle_filt + α(angle_raw - angle_filt)`

- Error is computed as: `error = setpoint - filtered_angle`
  
- Implemented a **Proportional control algorithm** to stabilize the platform
   - Target setpoints are `0° pitch` and `0° roll`
   - Two proportional controllers (pitch and roll): `u = Kp * error`
   - Includes a small deadband around `0°` to reduce jitter and noise-induced motion
   - Servo outputs are centered and constrained to safe limits to prevent over-rotation/binding
   - Servo command per axis `Cmd = center + u`

- Integrated the complete system on a single prototype platform

## Block Diagram
![ Block Diagram 1](./Images/SBRdiagram.png)

## Parts
| Part | QTY | Manufacturer |
| --- | --- | --- |
| Arduino Uno | 1 | Arduino |
| MPU6050 | 1 | Adafruit |
| Servo Motor | 2 | N/A |
| Pan-Tilt kit | 1 | N/A |

## Pinout
|MPU 6050  and Servo Motor | Arduino Uno Pin | 
| --- | --- |
| MPU- Vin | 3.3V |
| MPU- GND | GND |
| MPU- SCL | A5 |
| MPU- SDA | A4 | 
| Servo 1- VCC | 5V |
| Servo 1- GND | GND |
| Servo 1- PWM Input | 9 |
| Servo 2- VCC | 5V |
| Servo 2- GND | GND |
| Servo 2- PWM Input | 10 |


