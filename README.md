# Eastec Apollo

**ESP32 Bluetooth Speaker with Ferrofluid Animation**


## Hardware

<img src="https://raw.githubusercontent.com/hotteshen/promo/develop/images/apollo.jpeg" width=480>

* [ESP32-WROOM](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf) for Bluetooth connectivity
* [CH340](http://www.wch-ic.com/products/CH340.html) for firmware update and debug
* [PCM5102](https://www.ti.com/lit/ds/symlink/pcm5102.pdf) for I2S DAC
* [TPA3118](https://www.ti.com/lit/ds/symlink/tpa3116d2.pdf) for audio power amplification
* [MSGEQ7](https://mix-sig.com/images/datasheets/MSGEQ7.pdf) (x2) for audio spectrum analysis
* [DRV8872](https://www.ti.com/lit/ds/symlink/drv8872.pdf) (x2) for electromagnet driving to control ferrofluid

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
idf.py -p PORT flash
```


## Reference

* [ESP-IDF A2DP-SINK demo](https://github.com/espressif/esp-idf/tree/master/examples/bluetooth/bluedroid/classic_bt/a2dp_sink)


---

* Created on 5th Aug, 2021
* Last updated on 5th Aug, 2021
