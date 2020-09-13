## 准备

### 硬件

- NodeMCU-ESP8266开发板
- lcd1602显示屏（使用I2C总线）
- 杜邦线若干
- 可以联网的WiFi信号

### 软件环境

- Arduino IDE
- 心知天气API（免费版够用）

### 连线  NodeMCU——I2C

- GPIO4——SDA
- GPIO5——SCL
- VCC——3.3V
- GND——GND

####  GPIO overview

|  GPIO  | Function             |      State       | Restrictions                                                 |
| :----: | :------------------- | :--------------: | :----------------------------------------------------------- |
|   0    | Boot mode select     |       3.3V       | No Hi-Z                                                      |
|   1    | TX0                  |        -         | Not usable during Serial transmission                        |
|   2    | Boot mode select TX1 | 3.3V (boot only) | Don’t connect to ground at boot time Sends debug data at boot time |
|   3    | RX0                  |        -         | Not usable during Serial transmission                        |
|   4    | SDA (I²C)            |        -         | -                                                            |
|   5    | SCL (I²C)            |        -         | -                                                            |
| 6 - 11 | Flash connection     |        x         | Not usable, and not broken out                               |
|   12   | MISO (SPI)           |        -         | -                                                            |
|   13   | MOSI (SPI)           |        -         | -                                                            |
|   14   | SCK (SPI)            |        -         | -                                                            |
|   15   | SS (SPI)             |        0V        | Pull-up resistor not usable                                  |
|   16   | Wake up from sleep   |        -         | No pull-up resistor, but pull-down instead Should be connected to RST to wake up |

## 原理解析

