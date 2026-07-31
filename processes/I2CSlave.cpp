#include "headers/processes.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/i2c_slave.h"
#include "headers/data.h"
#include "headers/peripherals.h"

using namespace processes;

i2c_inst_t* get_i2c(uint8_t i){
    return i2c0;
}

I2CSlave* I2CSlave::_instance = nullptr;

I2CSlave::I2CSlave(uint8_t _u_i2c_i, uint8_t _u_sda, uint8_t _u_scl, uint8_t _u_address,
     uint8_t* _u_buffer, bool& _u_special_request_pointer):
        _i2c_i(_u_i2c_i), _address(_u_address), _tx_buffer(_u_buffer), _sda(_u_sda), _scl(_u_scl),
         _special_request_pointer(_u_special_request_pointer){
    _instance = this;
    _current_register = 0;
}

void I2CSlave::_handle_i2c_event(i2c_inst_t* _r_i2c, i2c_slave_event_t event) {
    switch (event) {
        case I2C_SLAVE_RECEIVE:
            _current_register = i2c_read_byte_raw(_i2c);
            if (_current_register == _i2c_slave_registers::SPECIAL_COMMAND) {
                _special_request_pointer = true;
            }
            break;

        case I2C_SLAVE_REQUEST:
            if (_current_register < 3 * U_FLOAT_SIZE + _i2c_slave_registers::START_READ &&
            _current_register >= _i2c_slave_registers::START_READ) {
                i2c_write_byte_raw(_i2c, _tx_buffer[_current_register++ - _i2c_slave_registers::START_READ]);
            }
            else{
                i2c_write_byte_raw(_i2c, 0);
            }
            break;

        case I2C_SLAVE_FINISH:

            break;

        default:
            break;
    };
}

void I2CSlave::_i2c_handler(i2c_inst_t* _r_i2c, i2c_slave_event_t event) {
    if (_instance){
        _instance -> _handle_i2c_event(_r_i2c, event);
    }
}

void I2CSlave::set_current_slave_defult() {
    _instance = this;
}

void I2CSlave::build() {
    _i2c = get_i2c(_i2c_i);

    i2c_init(_i2c, 100 * 1000);

    gpio_set_function(_sda, GPIO_FUNC_I2C);
    gpio_set_function(_scl, GPIO_FUNC_I2C);
    gpio_pull_up(_sda);
    gpio_pull_up(_scl);

    i2c_slave_init(
        _i2c,
        _address,
        _i2c_handler
    );

}