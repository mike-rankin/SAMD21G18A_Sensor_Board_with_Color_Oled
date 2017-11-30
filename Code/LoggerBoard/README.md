# Firmware for Mike's SAMD21G Sensor Board with Color Oled.

This firmware allows you to view the sensors, and record them to the SD card.

- View live readings
- Set the RTC
- Record readings to a CSV on the SD card
- Browse and delete items from the SD card


## Hardware

- Processor: Cortex M0 (SAMD21G18A)
- Screen: 96x64 Color OLED (SSD1331)
- Sensors:
 - HDC1080DMBT (Temperature/Humidity)
 - MPL115A2 (Temperature/Pressure)


## Installation

### CPU/Board support

In Arduino IDE, go to **File** > **Preference**.
In the _Boards Manager URLs_ box, add `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json`

Now go to **Tools** > **Board** > **Boards Manager**, search for `Adafruit SAMD Boards` and install the latest version.

Go back to **Tools** > **Board** and select `Adafruit Feather M0`

### Libraries

In Arduino IDE, go to **Sketch** > **Include Library** > **Manage Libraries...**

Find the following libraries and install them:

- Adafruit_MPL115A2 (1.0.0)
- ClosedCube_HDC1080 (1.3.1)
- RTClib (1.2.0)
- Bounce2 (2.3.0)
- SD (1.1.1)*
- SSD_13XX (1.0)†

* _Additional modifications required_
† _Manual installation_


#### SD library patch

In order to support removing and re-inserting the SD card without restarting, you will need to apply a patch to the SD library.
Apply SD card patch: https://github.com/arduino-libraries/SD/pull/38
**NOTE:** Will no longer be required if the SD library is newer than 1.1.1

#### Manual install of SSD_13XX

Currently the SSD_13XX library is not listed in the Arduino IDE library manager.

- Download the ZIP from [here](https://github.com/sumotoy/SSD_13XX/archive/master.zip)
- Go to **Sketch** > **Include Library** > **Add .ZIP Library...**
- Select the downloaded ZIP file
