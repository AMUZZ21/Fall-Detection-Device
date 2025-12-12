# Fall Detection Device

An Arduino-based fall/accident detection system that monitors sudden impacts using an **MPU6050 accelerometer**, tracks location via GPS, and sends real-time alerts using a GSM module.

---

## Features

- **Impact Detection:** Uses **MPU6050 accelerometer** to detect falls or sudden impacts.
- **GPS Tracking:** Captures latitude and longitude coordinates for accurate location tracking.
- **Alert System:** Sends SMS with Google Maps link and automatically makes a call to a predefined number.
- **Visual & Audio Alerts:** LED and buzzer activate when a fall is detected.

---

## Hardware Required

- Arduino Uno or Nano
- **MPU6050** accelerometer
- **GPS Module** (e.g., NEO-6M)
- **SIM800L GSM Module**
- LED and Buzzer
- Connecting wires and breadboard

---

## Pin Configuration

| Component      | Pin         |
|----------------|------------|
| LED            | 8          |
| Buzzer         | 9          |
| SIM800L RX     | 2          |
| SIM800L TX     | 3          |
| GPS RX         | 5          |
| GPS TX         | 6          |
| MPU6050 SDA    | A4 (Arduino Uno/Nano) |
| MPU6050 SCL    | A5 (Arduino Uno/Nano) |

---

## Software

- Arduino IDE
- Libraries:
  - `Wire.h` (for I2C communication with **MPU6050**)
  - `MPU6050.h` (for accelerometer)
  - `SoftwareSerial.h` (for GSM and GPS modules)

---

## Usage

1. Connect the hardware components as per the pin configuration.
2. Install required Arduino libraries.
3. Open `FallDetectionDevice.ino` in Arduino IDE.
4. Update the `phoneNumber` variable with the number to receive alerts.
5. Upload the code to the Arduino.
6. Monitor the Serial console for GPS and acceleration data.

---

## How It Works

1. The **MPU6050** continuously monitors acceleration.
2. If the total acceleration exceeds the **impact threshold (1.4g)**, a fall is detected.
3. The GPS module fetches the current coordinates.
4. The system sends an SMS with a Google Maps link to the predefined phone number.
5. SIM800L also makes a call to alert immediately.
6. LED and buzzer provide instant visual and audio notification.

---

## Notes

- Adjust `impactThreshold` based on testing and sensitivity needs.
- Ensure the GSM module has adequate signal coverage.
- GPS may take some time to get a fix, especially indoors.

---

## License

This project is open-source and available under the MIT License. See the [LICENSE](LICENSE) file for details.
