# OpenRGB ASRock RX 9070 XT Steel Legend Plugin

Development build for the ASRock RX 9070 XT Steel Legend RGB controller.

This build keeps one normal OpenRGB device, but also adds a plugin settings tab named
**ASRock GPU RGB**. Use that tab for per-channel hardware mode control.

## Hardware target

- I2C bus: `7`
- I2C address: `0x36`
- Channels:
  - `3` = ARGB Header
  - `6` = Top / Side
  - `7` = Fan

## Why the custom tab exists

OpenRGB's normal Mode dropdown is controller-wide. It can reset the Zone/LED selection
when changing hardware modes. The custom tab avoids that by writing the selected GPU
channel directly.

## Build

```bash
OPENRGB_ROOT=~/OpenRGB qmake OpenRGBASRockRX9070XTPlugin.pro
make -j$(nproc)
```

## Install for local testing

```bash
mkdir -p ~/.config/OpenRGB/plugins
cp -v build/libOpenRGBASRockRX9070XTPlugin.so ~/.config/OpenRGB/plugins/
openrgb
```

## Test

Open **Settings -> Plugins -> ASRock GPU RGB** and use:

- Target: All GPU RGB zones / ARGB Header / Top Side / Fan
- Mode
- Color
- Speed
- Brightness
- Apply
