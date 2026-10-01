# NFC Smart Key

STM32G0C1CE firmware testbench for an NXP PN5180 NFC frontend connected over
SPI. It uses NXP's NFC Reader Library to poll and activate ISO/IEC 14443 Type A
cards, identify MIFARE Classic 1K/4K cards, authenticate, and read a test block.

## Current functionality

- PN5180 SPI board-abstraction layer for STM32 HAL
- NFC-A polling and activation through `phacDiscLoop`
- MIFARE Classic 1K/4K detection using the card SAK
- KeyStore, software crypto, authentication, and block reads
- CMake Debug and Release build presets

The test currently authenticates block 4 with Key A from KeyStore entry 1. The
library's development entry uses the factory-default MIFARE key
`FF FF FF FF FF FF`; replace it before using non-development cards.

## PN5180 connections

| PN5180 signal | STM32G0 pin |
| --- | --- |
| NSS | PA4 |
| SCK | PA5 |
| MISO | PA6 |
| MOSI | PA7 |
| BUSY | PB0 |
| IRQ | PB1 |
| RESET_N | PB2 |

## Build

Requirements: CMake, Ninja, and the Arm GNU toolchain available on `PATH`.

```sh
cmake --preset Debug
cmake --build --preset Debug
```

The resulting firmware is `build/Debug/NFC_smart_key.elf`. Hardware and pin
configuration can be adjusted in `NFC_smart_key.ioc` with STM32CubeMX.
