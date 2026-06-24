#include "ASRockRX9070XTController.h"

ASRockRX9070XTController::ASRockRX9070XTController(i2c_smbus_interface* bus_ptr)
    : bus(bus_ptr)
{
    channels.push_back({ "ARGB Header", 0x03 });
    channels.push_back({ "Top / Side",   0x06 });
    channels.push_back({ "Fan",          0x07 });
}

std::string ASRockRX9070XTController::GetLocation() const
{
    if(bus == nullptr)
    {
        return "I2C: unavailable";
    }

    return std::string("I2C bus ") + std::to_string(bus->bus_id) +
           " addr 0x36";
}

unsigned int ASRockRX9070XTController::GetChannelCount() const
{
    return static_cast<unsigned int>(channels.size());
}

const ASRockRX9070XTController::Channel& ASRockRX9070XTController::GetChannel(unsigned int index) const
{
    return channels[index];
}

bool ASRockRX9070XTController::WriteChannel(uint8_t channel,
                                            uint8_t mode,
                                            uint8_t red,
                                            uint8_t green,
                                            uint8_t blue,
                                            uint8_t speed,
                                            uint8_t brightness,
                                            uint8_t direction)
{
    if(bus == nullptr)
    {
        return false;
    }

    uint8_t packet[12] =
    {
        0x10, 0x00, channel,
        mode, red, green, blue,
        speed, brightness, direction,
        0x1A, 0x00
    };

    return bus->i2c_write_block(I2C_ADDRESS, sizeof(packet), packet) >= 0;
}
