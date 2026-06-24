#include "RGBController_ASRockRX9070XT.h"

#include <algorithm>
#include <cstdint>

namespace
{
    static constexpr unsigned int DEFAULT_SPEED      = 0x80;
    static constexpr unsigned int DEFAULT_BRIGHTNESS = 0xFF;
    static constexpr unsigned int DEFAULT_DIRECTION  = 0x00;

    uint8_t ClampToByte(unsigned int value)
    {
        return static_cast<uint8_t>(std::min(value, 255U));
    }

    uint8_t MapOpenRGBSpeedToHardwareSpeed(unsigned int value, unsigned int minimum, unsigned int maximum)
    {
        unsigned int clamped = std::min(std::max(value, minimum), maximum);
        return static_cast<uint8_t>((minimum + maximum) - clamped);
    }
}

RGBController_ASRockRX9070XT::RGBController_ASRockRX9070XT(ASRockRX9070XTController* controller_ptr)
    : controller(controller_ptr)
{
    name        = "ASRock RX 9070 XT Steel Legend";
    vendor      = "ASRock";
    description = "ASRock RX 9070 XT Steel Legend GPU RGB Controller";
    version     = "0.1.1";
    serial      = "";
    location    = controller ? controller->GetLocation() : "I2C bus unavailable";
    type        = DEVICE_TYPE_GPU;
    flags       = CONTROLLER_FLAG_LOCAL;

    AddHardwareMode("Static",                0x01, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Breathing",             0x02, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Strobe",                0x03, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("RGB Cycle",             0x04, MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_NONE);
    AddHardwareMode("Random",                0x05, MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR, MODE_COLORS_RANDOM);
    AddHardwareMode("Color Shift",           0x07, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Visor",                 0x08, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Stacking",              0x09, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Fill Wave",             0x0A, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Traveling Wave",        0x0B, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Marquee Color",         0x0C, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Marquee Random",        0x0D, MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_RANDOM_COLOR, MODE_COLORS_RANDOM);
    AddHardwareMode("Color Wave",            0x0E, MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_PER_LED);
    AddHardwareMode("Rainbow",               0x0F, MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS, MODE_COLORS_NONE);

    SetupZones();
}

RGBController_ASRockRX9070XT::~RGBController_ASRockRX9070XT()
{
    delete controller;
}

void RGBController_ASRockRX9070XT::AddHardwareMode(const char* mode_name,
                                                   uint8_t value,
                                                   unsigned int mode_flags,
                                                   unsigned int color_mode)
{
    mode new_mode;
    new_mode.name           = mode_name;
    new_mode.value          = value;
    new_mode.flags          = mode_flags;
    new_mode.speed_min      = 0x20;
    new_mode.speed_max      = 0xFF;
    new_mode.speed          = DEFAULT_SPEED;
    new_mode.brightness_min = 0x20;
    new_mode.brightness_max = 0xFF;
    new_mode.brightness     = DEFAULT_BRIGHTNESS;
    new_mode.direction      = DEFAULT_DIRECTION;
    new_mode.color_mode     = color_mode;
    new_mode.colors_min     = 1;
    new_mode.colors_max     = 1;
    new_mode.colors.resize(1);
    new_mode.colors[0]      = ToRGBColor(255, 255, 255);

    modes.push_back(new_mode);
}

void RGBController_ASRockRX9070XT::SetupZones()
{
    if(controller == nullptr)
    {
        return;
    }

    zones.clear();
    leds.clear();

    for(unsigned int channel_idx = 0; channel_idx < controller->GetChannelCount(); channel_idx++)
    {
        const auto& channel = controller->GetChannel(channel_idx);

        zone new_zone;
        new_zone.name       = channel.name;
        new_zone.type       = ZONE_TYPE_SINGLE;
        new_zone.leds_min   = 1;
        new_zone.leds_max   = 1;
        new_zone.leds_count = 1;
        new_zone.matrix_map = nullptr;
        new_zone.start_idx  = channel_idx;
        zones.push_back(new_zone);

        led new_led;
        new_led.name  = std::string(channel.name) + " LED";
        new_led.value = channel.value;
        leds.push_back(new_led);
    }

    SetupColors();

    for(unsigned int idx = 0; idx < colors.size(); idx++)
    {
        colors[idx] = ToRGBColor(255, 255, 255);
    }
}


unsigned int RGBController_ASRockRX9070XT::GetHardwareChannelCount() const
{
    return controller ? controller->GetChannelCount() : 0;
}

std::string RGBController_ASRockRX9070XT::GetHardwareChannelName(unsigned int index) const
{
    if(controller == nullptr || index >= controller->GetChannelCount())
    {
        return std::string();
    }

    return controller->GetChannel(index).name;
}

void RGBController_ASRockRX9070XT::ApplyCustomSettings(int target_index,
                                                       uint8_t mode_value,
                                                       uint8_t red,
                                                       uint8_t green,
                                                       uint8_t blue,
                                                       uint8_t speed_value,
                                                       uint8_t brightness_value,
                                                       uint8_t direction_value)
{
    if(controller == nullptr)
    {
        return;
    }

    const uint8_t hardware_speed = MapOpenRGBSpeedToHardwareSpeed(speed_value, 0x20, 0xFF);

    auto write_index = [&](unsigned int index)
    {
        if(index >= controller->GetChannelCount())
        {
            return;
        }

        const auto& channel = controller->GetChannel(index);
        controller->WriteChannel(channel.value,
                                 mode_value,
                                 red,
                                 green,
                                 blue,
                                 hardware_speed,
                                 brightness_value,
                                 direction_value);
    };

    if(target_index < 0)
    {
        for(unsigned int index = 0; index < controller->GetChannelCount(); index++)
        {
            write_index(index);
        }
    }
    else
    {
        write_index(static_cast<unsigned int>(target_index));
    }
}

void RGBController_ASRockRX9070XT::ResizeZone(int /*zone*/, int /*new_size*/)
{
    // Fixed hardware zones.
}

void RGBController_ASRockRX9070XT::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_ASRockRX9070XT::UpdateZoneLEDs(int zone)
{
    if(zone < 0 || static_cast<unsigned int>(zone) >= zones.size())
    {
        return;
    }

    if(controller == nullptr || active_mode < 0 || static_cast<unsigned int>(active_mode) >= modes.size())
    {
        return;
    }

    mode& current_mode = modes[active_mode];
    const auto& channel = controller->GetChannel(static_cast<unsigned int>(zone));
    RGBColor color = colors[zones[zone].start_idx];

    if(current_mode.color_mode == MODE_COLORS_MODE_SPECIFIC && !current_mode.colors.empty())
    {
        color = current_mode.colors[0];
    }

    controller->WriteChannel(channel.value,
                             ClampToByte(current_mode.value),
                             RGBGetRValue(color),
                             RGBGetGValue(color),
                             RGBGetBValue(color),
                             MapOpenRGBSpeedToHardwareSpeed(current_mode.speed, current_mode.speed_min, current_mode.speed_max),
                             ClampToByte(current_mode.brightness),
                             ClampToByte(current_mode.direction));
}

void RGBController_ASRockRX9070XT::UpdateSingleLED(int led_idx)
{
    UpdateZoneLEDs(led_idx);
}

void RGBController_ASRockRX9070XT::DeviceUpdateMode()
{
    for(unsigned int zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(static_cast<int>(zone_idx));
    }
}
