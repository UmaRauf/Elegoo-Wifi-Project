This is a WiFi resilience project for an Arduino (Elegoo R3) wheeled robot. 
The robot is controlled through WiFi generated from an Espressif ESP32, acting as an access point.


When connected to the access point (PC or Mobile OK), opening the generated IP address (default is http://192.168.4.1/) will show basic control buttons for the user to steer the robot. 

Code uploading guide:

Preferably use Arduino IDE. Install ESP32 packages and any libraries necessary (arduino type used, web server type etc)

1. MotorTest is a check using USB to serial communication to ensure motor and batteries are working.

2. ArduinoMain is the code to upload on the Arduino (disconnect other components beforehand)
- when uploading select the proper board type and select PC port used first before verifying or uploading code (eg. Arduino 3 on COM4)

3. ESP32Main is dedicated code for the ESP32 ONLY. (use a USB-C Data Cable and check if drivers are required on your machine (LINK HERE))
- switch ESP32 to download mode first
- exact boot or reset modes are model dependent but typically the method is by holding the BOOT button then selecting the new port on PC.
- disconnect other components beforehand as well.
- once uploaded, check WiFi networks nearby and search for "Robot-Control" (wifi generated is 2.4 GHz). this means code is working.

For component, wiring and hardware guides search for your exact model datasheets or email Elegoo UK at service@elegoo.com, they were very helpful when 
I encountered hardware issues.
