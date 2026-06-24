#pragma once

#include <cstdint>
#include <string>

#include "RGBController.h"
#include "ASRockRX9070XTController.h"

class RGBController_ASRockRX9070XT : public RGBController
{
public:
    explicit RGBController_ASRockRX9070XT(ASRockRX9070XTController* controller_ptr);
    ~RGBController_ASRockRX9070XT() override;

    void SetupZones() override;
    void ResizeZone(int zone, int new_size) override;
    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;
    void DeviceUpdateMode() override;

    unsigned int GetHardwareChannelCount() const;
    std::string GetHardwareChannelName(unsigned int index) const;

    void ApplyCustomSettings(int target_index,
                             uint8_t mode_value,
                             uint8_t red,
                             uint8_t green,
                             uint8_t blue,
                             uint8_t speed_value,
                             uint8_t brightness_value,
                             uint8_t direction_value);

private:
    ASRockRX9070XTController* controller;

    void AddHardwareMode(const char* name,
                         uint8_t value,
                         unsigned int flags,
                         unsigned int color_mode);
};
