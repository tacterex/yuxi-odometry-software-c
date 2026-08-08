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
    address(_u_address),
    tx_buffer(_u_buffer),
    _sda(_u_sda),
    _scl(_u_scl),
    baudrate(_u_baudrate),
    w_registers(_u_w_registers) ,
    nw_registers(_u_nw_registers),
    updated(_u_updated)
{
    _instance = this;
    _i2c = get_i2c(_i2c_i);
    current_register = DEF_ADDR;
    waiting_for_register = true;
}

void I2CSlave::_handle_i2c_receive() {
    temp_byte = i2c_read_byte_raw(_i2c);

    switch (temp_byte) {
        case START_READ:
            current_register = START_READ;
            waiting_for_register = true;
            break;

        case RESET_NW:
            nw_registers[0] = 1;
            updated = true;
            waiting_for_register = true;
            break;

        default:
            if (waiting_for_register) {
                current_register = static_cast<slave_reg_t>(temp_byte);
                waiting_for_register = false;
            }
            else {
                switch (current_register) {
                    case X_ENABLED:
                        w_registers[0] = temp_byte;
                        break;
                    case Y_ENABLED:
                        w_registers[1] = temp_byte;
                        break;
                    case G_ENABLED:
                        w_registers[2] = temp_byte;
                        break;
                }
                waiting_for_register = true;
                updated = true;
                current_register = DEF_ADDR;
            }
            break;
    }
}

void I2CSlave::_handle_i2c_request() {
    if (
            current_register < 3 * U_FLOAT_SIZE + START_READ &&
            current_register >= START_READ
    )
    {
        i2c_write_byte_raw(_i2c, tx_buffer[current_register - START_READ]);
        ++current_register;
    }
    else{
        switch (current_register) {
            case X_ENABLED:
                i2c_write_byte_raw(_i2c, w_registers[0]);
                break;
            case Y_ENABLED:
                i2c_write_byte_raw(_i2c, w_registers[1]);
                break;
            case G_ENABLED:
                i2c_write_byte_raw(_i2c, w_registers[2]);
                break;
            default:
                i2c_write_byte_raw(_i2c, 0);
                break;
        }
    }
}

void I2CSlave::_handle_i2c_finish() {
    waiting_for_register = true;
}

void I2CSlave::_handle_i2c_event(i2c_inst_t* _r_i2c, i2c_slave_event_t event) {
    switch (event) {
        case I2C_SLAVE_RECEIVE:
            _handle_i2c_receive();
            break;

        case I2C_SLAVE_REQUEST:
            _handle_i2c_request();
            break;

        case I2C_SLAVE_FINISH:
            _handle_i2c_finish();
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
    i2c_init(_i2c, baudrate);

    gpio_set_function(_sda, GPIO_FUNC_I2C);
    gpio_set_function(_scl, GPIO_FUNC_I2C);
    gpio_pull_up(_sda);
    gpio_pull_up(_scl);

    i2c_slave_init(
        _i2c,
        address,
        _i2c_handler
    );

}