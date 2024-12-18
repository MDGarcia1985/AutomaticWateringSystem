Here’s the `README.md` formatted specifically for Markdown:

---

# **Automatic Watering System**

## **Overview**
The Automatic Watering System is an Arduino-based project that automates plant watering based on environmental conditions. It uses a stepper motor to direct water flow between two plants, a pump to deliver water, and a DHT11 sensor to measure temperature and humidity. An LCD display provides real-time data, and the system enters a low-power mode when idle.

---

## **Features**
- **DHT11 Sensor**: Measures temperature and humidity.
- **Stepper Motor**: Directs water flow to Plant A or Plant B.
- **Pump Control**: Activates the pump to water plants.
- **LCD Display**: Shows temperature, humidity, and system status.
- **Low-Power Mode**: Utilizes AVR watchdog sleep mode for energy efficiency.
- **Component Testing**: Verifies the stepper motor and pump functionality at the start of each loop.

---

## **Hardware Components**
1. **Arduino Board** (e.g., Arduino Uno)
2. **DHT11 Sensor**: Measures temperature and humidity.
3. **Stepper Motor**: Controls the valve for directing water flow.
4. **ULN2003 Stepper Motor Driver**: Drives the stepper motor.
5. **3-6V Water Pump**: Delivers water to plants.
6. **Transistor (PN2222)**: Controls the pump via Arduino.
7. **LCD Display (16x2)**: Displays system data.
8. **Power Supply**: 5V-9V DC for the Arduino and components.

---

## **Pin Configuration**
| Component         | Arduino Pin  |
|-------------------|--------------|
| Stepper Motor IN1 | 6            |
| Stepper Motor IN2 | 10           |
| Stepper Motor IN3 | A1           |
| Stepper Motor IN4 | A2           |
| Pump Control      | 9            |
| DHT11 Sensor      | 7            |
| LCD RS            | 12           |
| LCD Enable        | 11           |
| LCD D4            | 5            |
| LCD D5            | 4            |
| LCD D6            | 3            |
| LCD D7            | 2            |

---

## **How It Works**
1. **Initialization**:
   - Initializes all components, ensuring the pump is off and the system is ready.

2. **Component Testing**:
   - At the start of each loop, the system tests the stepper motor and pump.

3. **Data Monitoring**:
   - The DHT11 sensor measures temperature and humidity.
   - Readings are displayed on the LCD and logged to the Serial Monitor.

4. **Watering Logic**:
   - If the humidity is below 50% and the temperature is above 15°C, the system waters the plants:
     - Stepper motor moves to Plant A position, and the pump runs for 5 seconds.
     - Stepper motor moves to Plant B position, and the pump runs for 5 seconds.

5. **Low-Power Mode**:
   - After completing tasks, the system enters sleep mode for 10 minutes to conserve power.

---

## **Setup Instructions**
1. **Wiring**:
   - Connect components as per the pin configuration table.
   - Ensure all power and ground connections are secure.

2. **Upload Code**:
   - Open the `.ino` file in the Arduino IDE.
   - Select the correct board and port in the **Tools** menu.
   - Upload the code to your Arduino.

3. **Testing**:
   - The system will test components (stepper motor and pump) at the start of each loop.
   - Check the Serial Monitor for debugging output.

4. **Operation**:
   - The system measures temperature and humidity continuously.
   - Watering occurs based on the configured thresholds.

---

## **Customization**
- **Thresholds**:
  - Adjust temperature and humidity thresholds in the loop:
    ```cpp
    if (hum < 50 && temp > 15) { ... }
    ```

- **Stepper Motor Positions**:
  - Modify positions for Plant A and Plant B:
    ```cpp
    moveStepperTo(0);   // Plant A position
    moveStepperTo(100); // Plant B position
    ```

- **Sleep Duration**:
  - Adjust the sleep duration:
    ```cpp
    goToSleep(600000); // Sleep for 10 minutes
    ```

---

## **Known Limitations**
- **DHT11 Sensor Accuracy**:
  - DHT11 has limited accuracy. Consider upgrading to DHT22 for better precision.
- **Pump Power**:
  - Ensure the pump's power requirements are compatible with the transistor.
- **Stepper Motor Calibration**:
  - Verify and calibrate stepper motor steps for precise plant targeting.

---

## **Future Improvements**
- Support for additional plants.
- Add EEPROM storage to save state during power loss.
- Integrate a real-time clock (RTC) for scheduled watering.
- Implement WiFi connectivity for remote monitoring.

---

## **License**
This project is licensed under the MIT License. Feel free to use, modify, and distribute the code.

---
