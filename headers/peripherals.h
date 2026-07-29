#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

#include <cstdint>

namespace peripherals {
    class EncoderReader {
    private:
        uint8_t  _adc_channel;
        uint16_t _last_position;
        uint16_t _max_value;
        void update_last_position();

    public:
        EncoderReader(bool XY);
        uint16_t get_raw_position();
        int16_t  get_raw_step();
        uint16_t get_max_value();
        uint8_t  get_channel();
    };
}

#endif