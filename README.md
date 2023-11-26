# Water Level Monitor
In my home of The Royal bafokeng Nation in Phokeng, Rustenburg there is a water crisis. The temporary solution by the Administrative authority has been to install water tanks in household yards across the nation. Whenever the water depletes water trucks are sent to refill these tanks. The problem is that we only realise that we need refills once the water is completely depleted, by then we are already in distress, and it takes some time to organise a water truck to carry out the service. 

The problem I am solving with this project is the lack of real-time data that the Administrative authority (The Bafokeng Civic Center in this case) has access to. Using this device and the cloud platform, the Civic Centre would be alerted whenever water levels hit 20% and will be prompted to organise water trucks and dispatch the water trucks to refill the water tanks before they are completely dry.
Once this product is installed across the nation the data it collects will be stored in the cloud and will downloadable for analysis to use when planning for the nations future water infrastructure to create a more robust water distribution system for the Royal Bafokeng Nation.

This project implements a water level monitor using an ESP8266 microcontroller, ultrasonic sensor, OLED display, and Blynk app integration. The water level is measured using the ultrasonic sensor, and the data is displayed on an OLED screen. Additionally, the project includes Blynk integration for remote monitoring and control through a mobile app.

[Watch the video](https://www.loom.com/share/108345b7ba444a2fa8bcbce509b9260d)

## Features
- Real-time water level monitoring.
- Battery voltage monitoring.
- GPS tracking with speed, number of satellites, and orientation/direction.
- Blynk integration for remote monitoring and control.

## Hardware Requirements
- ESP8266 microcontroller (NodeMCU, Wemos D1 Mini, etc.).
- Ultrasonic sensor (HC-SR04).
- OLED display (SSD1306).
- GPS module.
- Battery for power.

## Dependencies
- [Blynk Library](https://github.com/blynkkk/blynk-library)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [AceButton Library](https://github.com/bxparks/AceButton)
- [TinyGPS++ Library](https://github.com/mikalhart/TinyGPSPlus)

## Setup
1. Clone or download the repository.
2. Install the required libraries using the Arduino Library Manager.
3. Configure the Blynk app with the provided template ID, name, and authentication token.
4. Update the Wi-Fi credentials (`ssid` and `pass`) in the code.
5. Adjust the virtual pin assignments according to your Blynk app configuration.
6. Connect the hardware components as per the defined pin connections.
7. Flash the code to your ESP8266 board.

## Blynk App Configuration
- Use the Blynk app to create buttons for setting the full and empty tank distances.
- Set up a map widget for visualizing the GPS location.

## Usage
1. Open the Arduino IDE and upload the code to your ESP8266 board.
2. Monitor the water level, battery percentage, and GPS information in real-time through the Blynk app.
3. Adjust the full and empty tank distances using the Blynk app as needed.

**Note:** Ensure that you have a stable internet connection for Blynk to work correctly.

## Troubleshooting
- If Blynk connection issues occur, check the Wi-Fi credentials and Blynk authentication token.
- Verify the correct wiring of hardware components.
- Ensure that the GPS module is functioning correctly.

Feel free to contribute and improve this project!
