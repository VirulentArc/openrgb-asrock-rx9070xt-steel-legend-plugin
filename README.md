# OpenRGB ASRock RX 9070 XT Steel Legend Plugin

OpenRGB plugin for controlling the RGB lighting on the **ASRock Radeon RX 9070 XT Steel Legend** GPU.

This plugin exposes the card as one normal OpenRGB device and adds a plugin tab for reliable per-channel control.

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

Download the compiled plugin file for your operating system from the release page.

OpenRGB's normal plugin installer is the recommended method:

```text
Settings -> Plugins -> Install Plugin
```

Select the plugin file for your operating system:

```text
Windows: OpenRGBASRockRX9070XTPlugin.dll
Linux:   libOpenRGBASRockRX9070XTPlugin.so
```

Restart OpenRGB after installing.

You should see one device:

```text
ASRock RX 9070 XT Steel Legend
```

You should also see the plugin page under:

```text
Settings -> Plugins -> ASRock GPU RGB
```

## Manual install

If you do not want to use the OpenRGB plugin installer, copy the plugin file into OpenRGB's plugin folder.

Windows:

```text
%APPDATA%\OpenRGB\plugins
```

Linux:

```text
~/.config/OpenRGB/plugins
```

Then restart OpenRGB.

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

## Compatibility

This plugin is intended for the **ASRock Radeon RX 9070 XT Steel Legend**.

Do not use this plugin with unrelated GPUs unless you know they use the same RGB controller and I2C protocol.

Platform status:

- Linux: tested
- Windows: intended/supported by source, but needs a Windows `.dll` build and user testing

A Linux `.so` file will not load on Windows. Windows users need a Windows plugin build, usually named something like:

```text
OpenRGBASRockRX9070XTPlugin.dll
```

Linux users need a Linux plugin build, usually named something like:

```text
libOpenRGBASRockRX9070XTPlugin.so
```

## Remove

Close OpenRGB and remove the plugin file from OpenRGB's plugin folder.

Windows:

```text
%APPDATA%\OpenRGB\plugins\OpenRGBASRockRX9070XTPlugin.dll
```

Linux:

```bash
rm -f ~/.config/OpenRGB/plugins/libOpenRGBASRockRX9070XTPlugin.so
```

Then start OpenRGB again.

## Build from source

Most users do **not** need this section. This is only needed if you want to build the plugin yourself.

Important: build the plugin with the same Qt major version and a compatible compiler/toolchain for the OpenRGB build you are using.

### Linux example

Requirements:

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

### Windows example

Build from a Qt command prompt or Qt Creator using the same Qt major version/toolchain as your OpenRGB build.

Set `OPENRGB_ROOT` to the OpenRGB source tree, then run qmake and your compiler's make tool. For a MinGW Qt5 build, that usually looks like:

```bat
set OPENRGB_ROOT=C:\path\to\OpenRGB
qmake OpenRGBASRockRX9070XTPlugin.pro
mingw32-make
```

The Windows build should produce a `.dll` plugin file. Install that `.dll` through OpenRGB's plugin installer or copy it to:

```text
%APPDATA%\OpenRGB\plugins
```

## Technical notes

Known working hardware values from testing:

- I2C address: `0x36`
- OpenRGB I2C bus ID used by the current test build: `7`
- Channels:
  - `3` = ARGB Header
  - `6` = Top / Side
  - `7` = Fan

Notes and limitations:

- The normal OpenRGB Mode dropdown is best treated as whole-device control.
- For per-channel mode control, use the plugin tab.
- Direct Mode / Effects Plugin support is not enabled yet.
- The current test build targets OpenRGB I2C bus ID `7`. If a system enumerates the GPU RGB controller under a different OpenRGB bus ID, the plugin may need a configurable bus setting in a future release.
- Unknown modes or addresses should not be tested blindly. Earlier hardware testing showed that some invalid mode values could wedge the GPU RGB controller until a cold power cycle.

## License

See the repository license file.
