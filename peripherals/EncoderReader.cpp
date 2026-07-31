#include "headers/peripherals.h"
#include "headers/data.h"
#include "hardware/adc.h"
#include <cstdint>

using namespace peripherals;

static bool adc_initialized = false;

EncoderReader::EncoderReader(bool XY) {
    if (!adc_initialized) {
        adc_init();
        adc_initialized = true;
    }

    _adc_channel = 1 + (uint8_t)XY;
    max_value   = hardware::encoder_max_value;

    adc_gpio_init(hardware::adc0_gpio + _adc_channel);
    update_last_position();
}

uint16_t EncoderReader::get_max_value() {
    return max_value;
}

uint8_t EncoderReader::get_channel() {
    return _adc_channel;
}

uint16_t EncoderReader::get_raw_position() {
    adc_select_input(_adc_channel);
    return adc_read();
}

void EncoderReader::update_last_position() {
    last_position = get_raw_position();
}

int16_t EncoderReader::get_raw_step() {
    uint16_t l = last_position;
    update_last_position();
    int16_t s = last_position - l;

    if (s < -max_value / 2)
        s += max_value;
    if (s > max_value / 2)
        s -= max_value;
    
    return s;
}

void EncoderReader::reinit() {
    update_last_position();
}