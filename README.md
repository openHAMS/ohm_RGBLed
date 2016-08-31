# OHM: RGBLed
An ESP8266 RGB led controller module for openHAMS

##Dependencies
The following are required to use this:
- Adafruit HUZZAH Breakout or other compatible ESP8266
- Power supply
- RGB LED strip
- 3 MOSFETs for each color
- openHAMS server

##Features
- change LED color via `rgbled/set`
- confirmation of color change via `rgbled/status`

##Thanks
- [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
- [Async MQTT client for ESP8266](https://github.com/marvinroger/async-mqtt-client) by [marvinroger](https://github.com/marvinroger)
