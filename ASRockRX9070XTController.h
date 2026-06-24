#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "i2c_smbus.h"

class ASRockRX9070XTController
{
public:
    static constexpr uint8_t I2C_ADDRESS = 0x36;

    struct Channel
    {
        const char* name;
        uint8_t     value;
    };

    explicit ASRockRX9070XTController(i2c_smbus_interface* bus_ptr);

    std::string GetLocation() const;
    unsigned int GetChannelCount() const;
    const Channel& GetChannel(unsigned int index) const;

    bool WriteChannel(uint8_t channel,
                      uint8_t mode,
                      uint8_t red,
                      uint8_t green,
                      uint8_t blue,
                      uint8_t speed,
                      uint8_t brightness,
                      uint8_t direction);

private:
    i2c_smbus_interface* bus;
    std::vector<Channel> channels;
};
