# OpenRGB ASRock RX 9070 XT Steel Legend Plugin

OpenRGB plugin for controlling the RGB lighting on the **ASRock Radeon RX 9070 XT Steel Legend** GPU.

This plugin exposes the card as one normal OpenRGB device and adds a plugin tab for reliable per-channel control.

## Features

- Static color control
- Hardware effects such as Breathing, Strobe, Cycling, Rainbow, Rainbow 2, and others
- Brightness control
- Speed control with corrected OpenRGB-to-hardware speed mapping
- One normal OpenRGB device: `ASRock RX 9070 XT Steel Legend`
- Plugin tab for reliable per-channel control:
  - All GPU RGB zones
  - ARGB Header
  - Top / Side
  - Fan

## Install

This repository currently provides the plugin source code.

Prebuilt plugin files may be attached to GitHub releases when available. Use the plugin file that matches your operating system and OpenRGB build.

Current binary status:

- Linux: tested locally as `libOpenRGBASRockRX9070XTPlugin.so`
- Windows: source is intended to support Windows through OpenRGB's plugin/I2C layer, but no prebuilt Windows `.dll` is provided yet

To install a prebuilt plugin file, use OpenRGB's plugin installer:

```text
Settings -> Plugins -> Install Plugin
```

Then restart OpenRGB.

You should see one device:

```text
ASRock RX 9070 XT Steel Legend
```

You should also see the plugin page under:

```text
Settings -> Plugins -> ASRock GPU RGB
```

## Manual install

If you do not want to use OpenRGB's plugin installer, copy the plugin file into OpenRGB's plugin folder.

Linux:

```text
~/.config/OpenRGB/plugins
```

Windows:

```text
%APPDATA%\OpenRGB\plugins
```

Then restart OpenRGB.

A Linux `.so` file will not load on Windows. Windows needs a Windows `.dll` build.

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
Mode: Rainbow 2
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
- Windows: intended by source design, but not yet built or tested

## Remove

Close OpenRGB and remove the plugin file from OpenRGB's plugin folder.

Linux:

```bash
rm -f ~/.config/OpenRGB/plugins/libOpenRGBASRockRX9070XTPlugin.so
```

Windows:

```text
%APPDATA%\OpenRGB\plugins
```

Remove the plugin `.dll` from that folder if you installed a Windows build.

Then start OpenRGB again.

## Build from source

Most users do **not** need this section. This is only needed if you want to build the plugin yourself or if a prebuilt plugin is not available for your operating system.

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

### Windows build notes

A Windows `.dll` is not currently provided by this repository.

To make one, build from a Qt command prompt or Qt Creator using the same Qt major version and compiler family as your OpenRGB build. For example, if your OpenRGB build uses Qt5 MinGW, build this plugin with Qt5 MinGW as well.

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

See [LICENSE](LICENSE) if included with this repository.
