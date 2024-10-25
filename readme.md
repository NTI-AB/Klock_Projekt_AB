# Clock and Temperature Project

Author: Axel Börjeson  
Date: 2024-10-25

This project combines real-time clock (RTC) functionality, temperature measurement, and motor and LED control in an Arduino environment. Using a DS3231 RTC module, an OLED display, a servo motor, an analog temperature sensor, and RGB LEDs, this project dynamically displays the current time and adjusts the servo motor position and LED color based on the current temperature.

## Components

- **DS3231 RTC Module**: Tracks the current date and time.
- **OLED Display (SSD1306)**: Displays time in a clear, readable format.
- **Analog Temperature Sensor**: Reads ambient temperature, with Steinhart-Hart calculations for accuracy.
- **Servo Motor (9g)**: Adjusts based on temperature, providing a visual representation of temperature changes.
- **RGB LED**: Changes color gradually based on the difference between the temperature and a preset threshold.

## Libraries Used

- `U8glib`: For interfacing with the OLED display.
- `RTClib`: For interfacing with the DS3231 RTC module.
- `Wire`: To enable communication between Arduino and I2C devices.

## Project Setup

1. **Wiring**

   - **Temperature Sensor**: Analog pin `A1`.
   - **Servo Motor**: Digital pin `9`.
   - **RGB LED**:
     - Red - Digital pin `5`
     - Green - Digital pin `6`
     - Blue - Digital pin `7`
   - **Motor Control**: Analog pin `A0`.

2. **Libraries**  
   Install the required libraries in your Arduino IDE:

   - `U8glib` for OLED displays
   - `RTClib` for the DS3231 RTC module
   - `Wire` library is included by default with Arduino

3. **Upload Code**  
   Upload the code to the Arduino. Adjust the threshold temperature in the code if needed.

## Code Overview

### Constants and Variables

- `const int threshold = 23;`  
  Defines the temperature threshold in Celsius. The RGB LED color changes based on its value.

- **Steinhart-Hart Coefficients**  
  Used in the `getTemp()` function to calculate temperature accurately from the thermistor's resistance.

### Functions

#### `setup()`

Initializes the RTC, OLED display, servo motor, temperature sensor, and RGB LED pins. It also sets up the RTC to the current time.

#### `loop()`

Main function loop:

1. Fetches current time from the RTC and displays it on the OLED.
2. Reads temperature and adjusts the servo motor position and RGB LED color based on this value.
3. Activates/deactivates a fan motor depending on the temperature threshold.

#### `getTime()`

Returns the current time as a formatted string (HH:MM:SS) from the RTC module.

#### `oledWrite(text)`

Displays text on the OLED screen using the `U8glib` library.

#### `getTemp()`

Reads an analog temperature sensor and calculates the temperature in Celsius using the Steinhart-Hart formula. This value is used for controlling the servo motor and LED color.

#### `servoWrite(seconds)`

Maps the current temperature reading to a servo motor angle, creating a visual representation of temperature on the servo position.

#### `setLEDColor(temp)`

Gradually changes the RGB LED color:

- **Green** if the temperature is below the threshold by 2 degrees or more.
- **Yellow** as it approaches the threshold.
- **Red** if it is above the threshold by 2 degrees or more.

#### `flaktmotor()`

Controls the fan motor:

- Activates if temperature exceeds the threshold.
- Deactivates if temperature is below the threshold.

## Usage

The project displays the current time and adjusts LED color and servo motor position based on temperature. For customization, adjust the temperature threshold or modify the Steinhart-Hart coefficients based on your specific temperature sensor.

1. Observe the RGB LED color:
   - **Green**: Temperature is comfortably below the threshold.
   - **Yellow**: Temperature is close to the threshold.
   - **Red**: Temperature is above the threshold.
2. View the current time on the OLED display.
3. Monitor the servo motor angle to understand temperature fluctuations visually.

## Future Improvements

- **Adjustable Threshold**: Add a potentiometer to allow real-time adjustment of the temperature threshold.
- **Additional Sensors**: Integrate humidity or additional environmental sensors for extended functionality.
- **Data Logging**: Log time and temperature readings for later analysis.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

---

By using this README template, you can provide clear instructions, helping others set up and understand the code and hardware connections in your Clock and Temperature Project.
