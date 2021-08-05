# Eastec Apollo

**ESP32 Bluetooth Speaker with Ferrofluid Animation**


## Hardware

<img src="https://raw.githubusercontent.com/hotteshen/promo/develop/images/apollo.jpeg" width=480>

* ESP32-WROOM for Bluetooth connectivity
* CH340 for firmware update and debug
* PCM5102 for I2S DAC
* TPA3118 for audio power amplification
* MSGEQ7 (x2) for audio spectrum analysis
* DRV8872 (x2) for electromagnet driving to control ferrofluid

| ESP pin   | I2S signal   |
| :-------- | :----------- |
| GPIO22    | LRCK         |
| GPIO25    | DATA         |
| GPIO26    | BCK          |


## Build and Flash

Build the project in the standard ESP-IDF way.
This source code building is tested on the ESP-IDF version 4.2.1 and Ubuntu 21.04.

```sh
# after entering to the project root.
idf.py build

# the CH340 is usually named as /dev/ttyUSB0 on Linux, replace PORT with it.
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

---

* Created on 5th Aug, 2021
* Last updated on 5th Aug, 2021
