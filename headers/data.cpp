#include <cstdint>
#include "headers/data.h"

_i2c_slave_registers& operator++(_i2c_slave_registers& reg) {
    switch(reg) {
        case START_READ: reg = READ_1; break;
        case READ_1: reg = READ_2; break;
        case READ_2: reg = READ_3; break;
        case READ_3: reg = READ_4; break;
        case READ_4: reg = READ_5; break;
        case READ_5: reg = READ_6; break;
        case READ_6: reg = READ_7; break;
        case READ_7: reg = READ_8; break;
        case READ_8: reg = READ_9; break;
        case READ_9: reg = READ_10; break;
        case READ_10: reg = READ_11; break;
        default: reg = DEF_ADDR; break;
    };
    return reg;
}