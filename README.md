# OpenRGB ASRock RX 9070 XT Steel Legend Plugin

OpenRGB plugin for controlling the RGB lighting on the **ASRock Radeon RX 9070 XT Steel Legend** GPU on Linux.

This is an early hardware-specific plugin based on the I2C addresses discovered for this card. It exposes the card as one normal OpenRGB device and also adds a plugin settings tab for reliable per-channel control.

## Hardware target

This plugin is intended for the **ASRock Radeon RX 9070 XT Steel Legend**.

Known working hardware values:

- I2C bus: `7`
- I2C address: `0x36`
- Channels:
  - `3` = ARGB Header
  - `6` = Top / Side
  - `7` = Fan

Do not use this plugin with unrelated GPUs unless you know they use the same RGB controller and I2C protocol.

## Features

- Static color control
- Hardware effects such as Breathing, Strobe, RGB Cycle, Color Wave, etc.
- Brightness control
- Speed control with corrected OpenRGB-to-hardware speed mapping
- One normal OpenRGB device: `ASRock RX 9070 XT Steel Legend`
- Custom plugin tab for per-channel control:
  - All GPU RGB zones
  - ARGB Header
  - Top / Side
  - Fan

## Why the custom tab exists

OpenRGB's built-in **Mode** selector is controller-wide. On this GPU, the hardware supports separate channel writes, but OpenRGB's normal Zone/LED selectors do not reliably preserve the selected zone when changing modes.

Use the normal OpenRGB device page for whole-GPU changes.

Use the plugin tab for reliable per-channel control:

```text
Settings -> Plugins -> ASRock GPU RGB
```

## Requirements

- Linux
- OpenRGB built with Qt5
- Matching OpenRGB source tree for the OpenRGB version you are building against
- Qt5 build tools
- Working I2C access for OpenRGB

On Arch/CachyOS, the useful packages are typically:

```bash
sudo pacman -S --needed git base-devel qt5-base qt5-tools
```

## Check your OpenRGB version

Check the installed OpenRGB version and commit:

```bash
openrgb --version
```

Example tested version:

```text
OpenRGB 0.9+ (git1974)
Git Commit ID 4306603a28c86e91f4dd4f89b41efd3005f0b810
Qt5
```

Check whether your OpenRGB binary uses Qt5:

```bash
ldd "$(command -v openrgb)" | grep -E 'Qt[56](Core|Gui|Widgets)'
```

This plugin currently expects Qt5. If your OpenRGB uses Qt6, build instructions will need to be adjusted.

## Get matching OpenRGB source

The plugin should be built against the same OpenRGB source revision as your installed OpenRGB package.

Replace the checkout hash below with the commit shown by `openrgb --version` if yours is different:

```bash
cd ~
git clone https://gitlab.com/CalcProgrammer1/OpenRGB.git
cd ~/OpenRGB
git checkout 4306603a28c86e91f4dd4f89b41efd3005f0b810
```

Confirm the checkout:

```bash
git rev-parse HEAD
ls OpenRGBPluginInterface.h ResourceManagerInterface.h
```

## Build the plugin

Clone or extract this plugin repo, then build it with Qt5 `qmake`:

```bash
cd ~/openrgb-asrock-rx9070xt-steel-legend-plugin
OPENRGB_ROOT=~/OpenRGB qmake OpenRGBASRockRX9070XTPlugin.pro
make -j"$(nproc)"
```

The built plugin should appear here:

```text
build/libOpenRGBASRockRX9070XTPlugin.so
```

## Install the plugin locally

Close OpenRGB first, then copy the plugin into OpenRGB's user plugin folder:

```bash
mkdir -p ~/.config/OpenRGB/plugins
cp -v build/libOpenRGBASRockRX9070XTPlugin.so ~/.config/OpenRGB/plugins/
```

Start OpenRGB from a terminal so any plugin errors are visible:

```bash
openrgb
```

You should see one device:

```text
ASRock RX 9070 XT Steel Legend
```

And one plugin settings page:

```text
Settings -> Plugins -> ASRock GPU RGB
```

## Remove the plugin

Close OpenRGB, then remove the plugin file:

```bash
rm -f ~/.config/OpenRGB/plugins/libOpenRGBASRockRX9070XTPlugin.so
```

Then reopen OpenRGB:

```bash
openrgb
```

## Basic test

Use the plugin tab, not the normal OpenRGB Zone/LED dropdowns, for per-channel hardware effects.

Open:

```text
Settings -> Plugins -> ASRock GPU RGB
```

Test fan only:

```text
Target: Fan
Mode: Static
Color: Red
Apply
```

Expected result: only the fan lighting changes.

Test side only:

```text
Target: Top / Side
Mode: Breathing
Color: Blue
Apply
```

Expected result: only the top/side lighting changes.

Test all channels:

```text
Target: All GPU RGB zones
Mode: Color Wave
Apply
```

Expected result: all GPU RGB channels change.

## Notes and limitations

- This is a hardware-specific plugin for the ASRock RX 9070 XT Steel Legend.
- The normal OpenRGB Mode dropdown is best treated as whole-device control.
- For per-channel mode control, use the plugin tab.
- Direct Mode / Effects Plugin support is not enabled yet.
- The known working I2C bus is currently hardcoded to bus `7`.
- Unknown modes or addresses should not be tested blindly; earlier hardware testing showed that some invalid mode values could wedge the GPU RGB controller until a cold power cycle.

## Development build command summary

```bash
cd ~
git clone https://gitlab.com/CalcProgrammer1/OpenRGB.git
cd ~/OpenRGB
git checkout 4306603a28c86e91f4dd4f89b41efd3005f0b810

cd ~/openrgb-asrock-rx9070xt-steel-legend-plugin
OPENRGB_ROOT=~/OpenRGB qmake OpenRGBASRockRX9070XTPlugin.pro
make -j"$(nproc)"

mkdir -p ~/.config/OpenRGB/plugins
cp -v build/libOpenRGBASRockRX9070XTPlugin.so ~/.config/OpenRGB/plugins/
openrgb
```
