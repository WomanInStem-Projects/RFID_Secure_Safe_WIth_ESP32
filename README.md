# RFID Secure Safe with ESP32

RFID-based access-control prototype built on an ESP32. The project combines embedded firmware, hardware prototyping, and security testing for a "secure safe" style system

## Current Status

This is an active prototype. The core firmware flow is implemented and running, but several additional features are planned (detialed at the bottom).

## Repository Layout

- `Firware/`: firmware project, PlatformIO config, local library sources, simulation files
- `Hardware/`: schematic and wiring references
- `Study/`: exploratory code, notes that aided in my comprehention, and supporting documents (datasheets)
- `Vulnerability_research/`: security analysis and attack-surface research materials

## What Is Implemented

- ESP32 + MFRC522 RFID reader integration
- SSD1306 OLED status messages
- Red/green LED feedback for deny/grant states
- Buzzer for audio signaling
- First-run admin password setup over Serial
- In-memory Access Control List (ACL) of RFID UIDs
- Unknown card enrollment flow gated by admin password

## Current Access Flow

1. On startup, user sets a 6+ character admin password through Serial
2. Device waits for an RFID card
3. If card UID is already in ACL, access is granted (green LED blink)
4. If card UID is not in ACL, access is denied (red LED blink)
5. User can choose to enroll that card and must pass admin password check

## Hardware Used

- ESP32 DevKit V1 
- MFRC522 RFID module
- SSD1306 128x64 OLED (I2C)
- 1x red LED
- 1x green LED
- 1x passive buzzer

## Pin Mapping (Current Firmware)

- OLED SDA: `GPIO13`
- OLED SCL: `GPIO14`
- RFID SCK: `GPIO18`
- RFID MISO: `GPIO19`
- RFID MOSI: `GPIO23`
- RFID SS/CS: `GPIO5`
- RFID RST: `GPIO4`
- Red LED: `GPIO26`
- Green LED: `GPIO21`
- Buzzer: `GPIO15`

## Software Stack

- PlatformIO + Arduino framework
- Main firmware: `Firware/src/main.cpp`
- Key libraries:
	- `MFRC522`
	- `Adafruit SSD1306`
	- `Adafruit GFX`

## Build and Run (PlatformIO)

If using VsCode extension simply click the run button:
Otherwise from the repository root:

```bash
cd Firware
pio run
pio run -t upload
pio device monitor -b 9600
```


## Wokwi Simulation

This Wokwi simulation is not always an accurate representation of current workings, but gives a generall idea of how the project is being implimented.

- Project link: https://wokwi.com/projects/458415152295436289


## Planned Next Steps

- Impliment a keypad or phyiscall input instead of relying on serial
- Impliment a buzzer and solinoid motor for audio and door unlocking
- Allow the buzzer, oled display, leds, and reading of cards to run concurrently 
- Impliment code as a state machine and allow for remote access
- 3D design and print enclosure that would bring circuit alive
- Create a PCB, purchase all parts and solder together
