The idea is to build a small driver device (based on an ESP32-C3), powered by a battery bank (5V/~2A), which is able to drive a various amount of WS2812 LEDs.
Individual LED sequences can be edited by a separate python app that generates a binary file, which can be uploaded byt the minimalistic upload page of the device via WLAN connection.

# Hardware
- ESP32-C3-SuperMini board
- some push-buttons
- WS2812 LED pad or chain
- 
[Using this level shifting trick](https://hackaday.com/2017/01/20/cheating-at-5v-ws2812-control-to-use-a-3-3v-data-line/)

# Firmware

[Template for LED driver via RMT}(https://github.com/JSchaenzle/ESP32-NeoPixel-WS2812-RMT)

# Software
- python
- Tck/Tk GUI to edit the sequence
