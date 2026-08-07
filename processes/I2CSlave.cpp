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

I2CSlave::I2CSlave(
    uint8_t _u_i2c_i,
    uint8_t _u_sda,
    uint8_t _u_scl,
    uint8_t _u_address,
    uint32_t _u_baudrate,
    uint8_t* _u_buffer,
    uint8_t* _u_w_registers,
    uint8_t* _u_nw_registers,
    bool& _u_updated
):
    _i2c_i(_u_i2c_i),
    _address(_u_address),
    _tx_buffer(_u_buffer),
    _sda(_u_sda),
    _scl(_u_scl),
    _baudrate(_u_baudrate),
    _w_registers(_u_w_registers) ,
    _nw_registers(_u_nw_registers),
    _updated(_u_updated)
{
    _instance = this;
    _i2c = get_i2c(_i2c_i);
    _current_register = 0;
    waiting_for_register = true;
}

void I2CSlave::_handle_i2c_event(i2c_inst_t* _r_i2c, i2c_slave_event_t event) {
    switch (event) {
        case I2C_SLAVE_RECEIVE:
            t_byte = i2c_read_byte_raw(_i2c);

            switch (t_byte) {
                case _i2c_slave_registers::START_READ:
                    _current_register = _i2c_slave_registers::START_READ;
                    waiting_for_register = true;
                    break;

                case _i2c_slave_registers::RESET_NW:
                    _nw_registers[0] = 1;
                    _updated = true;
                    waiting_for_register = true;
                    break;

                default:
                    if (waiting_for_register) {
                        _current_register = t_byte;
                        waiting_for_register = false;
                    }
                    else {
                        switch (_current_register) {
                            case _i2c_slave_registers::X_ENABLED:
                                _w_registers[0] = t_byte;
                                break;
                            case _i2c_slave_registers::Y_ENABLED:
                                _w_registers[1] = t_byte;
                                break;
                            case _i2c_slave_registers::G_ENABLED:
                                _w_registers[2] = t_byte;
                                break;
                        }

                        waiting_for_register = true;
                        _updated = true;
                    }
                    break;
            }
            break;

        case I2C_SLAVE_REQUEST:
            if (
                _current_register < 3 * U_FLOAT_SIZE + _i2c_slave_registers::START_READ &&
                _current_register >= _i2c_slave_registers::START_READ
            )
            {
                i2c_write_byte_raw(_i2c, _tx_buffer[_current_register++ - _i2c_slave_registers::START_READ]);
            }
            else{
                switch (_current_register) {
                    case _i2c_slave_registers::X_ENABLED:
                        i2c_write_byte_raw(_i2c, _w_registers[0]);
                        break;
                    case _i2c_slave_registers::Y_ENABLED:
                        i2c_write_byte_raw(_i2c, _w_registers[1]);
                        break;
                    case _i2c_slave_registers::G_ENABLED:
                        i2c_write_byte_raw(_i2c, _w_registers[2]);
                        break;
                    default:
                        i2c_write_byte_raw(_i2c, 0);
                        break;
                }
            }
            break;

        case I2C_SLAVE_FINISH:
            waiting_for_register = true;
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
    i2c_init(_i2c, _baudrate);

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