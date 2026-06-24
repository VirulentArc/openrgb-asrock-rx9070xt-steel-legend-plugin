# OpenRGB ASRock RX 9070 XT Steel Legend Plugin

OpenRGB plugin for controlling the RGB lighting on the **ASRock Radeon RX 9070 XT Steel Legend** GPU on Linux.

This plugin exposes the card as one normal OpenRGB device and also adds a plugin tab for reliable per-channel control.

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
- Hardware effects such as Breathing, Strobe, RGB Cycle, Color Wave, and others
- Brightness control
- Speed control with corrected OpenRGB-to-hardware speed mapping
- One normal OpenRGB device: `ASRock RX 9070 XT Steel Legend`
- Plugin tab for reliable per-channel control:
  - All GPU RGB zones
  - ARGB Header
  - Top / Side
  - Fan

## Install

Download the compiled plugin file from the release page:

```text
libOpenRGBASRockRX9070XTPlugin.so
```

Close OpenRGB.

Copy the plugin into your OpenRGB plugins folder:

```bash
mkdir -p ~/.config/OpenRGB/plugins
cp libOpenRGBASRockRX9070XTPlugin.so ~/.config/OpenRGB/plugins/
```

Start OpenRGB:

```bash
openrgb
```

You should see one device:

```text
ASRock RX 9070 XT Steel Legend
```

You should also see the plugin page under:

```text
Settings -> Plugins -> ASRock GPU RGB
```

## GUI install

You can also install the `.so` from OpenRGB's plugin page:

```text
Settings -> Plugins -> Install Plugin
```

Select:

```text
libOpenRGBASRockRX9070XTPlugin.so
```

Restart OpenRGB after installing.

## Usage

The normal OpenRGB device page can be used for whole-GPU changes.

For reliable per-channel control, use the plugin tab:

```text
Settings -> Plugins -> ASRock GPU RGB
```

Example: fan only

```text
Target: Fan
Mode: Static
Color: Red
Apply
```

Expected result: only the fan lighting changes.

Example: top/side only

```text
Target: Top / Side
Mode: Breathing
Color: Blue
Apply
```

Expected result: only the top/side lighting changes.

Example: all channels

```text
Target: All GPU RGB zones
Mode: Color Wave
Apply
```

Expected result: all GPU RGB channels change.

## Why the plugin tab exists

OpenRGB's built-in **Mode** selector is controller-wide. On this GPU, the hardware supports separate channel writes, but OpenRGB's normal Zone/LED selectors do not reliably preserve the selected zone when changing modes.

Use the normal OpenRGB device page for whole-GPU changes.

Use the plugin tab for per-channel mode/color/speed/brightness control.

## Remove

Close OpenRGB, then remove the plugin file:

```bash
rm -f ~/.config/OpenRGB/plugins/libOpenRGBASRockRX9070XTPlugin.so
```

Then start OpenRGB again:

```bash
openrgb
```

## Build from source

Most users do **not** need this section. This is only needed if you want to build the plugin yourself.

Requirements:

- Linux
- OpenRGB built with Qt5
- Qt5 build tools
- Matching OpenRGB source tree for the OpenRGB version you are building against

On Arch/CachyOS:

```bash
sudo pacman -S --needed git base-devel qt5-base qt5-tools
```

Get OpenRGB source:

```bash
cd ~
git clone https://gitlab.com/CalcProgrammer1/OpenRGB.git
```

If your installed OpenRGB package was built from a specific commit, check out that same commit before building the plugin.

Build the plugin:

```bash
cd ~/openrgb-asrock-rx9070xt-steel-legend-plugin
OPENRGB_ROOT=~/OpenRGB qmake OpenRGBASRockRX9070XTPlugin.pro
make -j"$(nproc)"
```

The built plugin should appear here:

```text
build/libOpenRGBASRockRX9070XTPlugin.so
```

Install the built plugin:

```bash
mkdir -p ~/.config/OpenRGB/plugins
cp -v build/libOpenRGBASRockRX9070XTPlugin.so ~/.config/OpenRGB/plugins/
openrgb
```

## Notes and limitations

- This is a hardware-specific plugin for the ASRock RX 9070 XT Steel Legend.
- The normal OpenRGB Mode dropdown is best treated as whole-device control.
- For per-channel mode control, use the plugin tab.
- Direct Mode / Effects Plugin support is not enabled yet.
- The known working I2C bus is currently hardcoded to bus `7`.
- Unknown modes or addresses should not be tested blindly. Earlier hardware testing showed that some invalid mode values could wedge the GPU RGB controller until a cold power cycle.

## License

See the repository license file.
