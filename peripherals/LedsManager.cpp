#include "headers/peripherals.h"
#include "hardware/gpio.h"

namespace peripherals {

    void leds_init() {
        gpio_init(LED_RED_GPIO);
        gpio_init(LED_GREEN_GPIO);
        gpio_init(LED_BLUE_GPIO);
    
        gpio_set_dir(LED_RED_GPIO, GPIO_OUT);
        gpio_set_dir(LED_GREEN_GPIO, GPIO_OUT);
        gpio_set_dir(LED_BLUE_GPIO, GPIO_OUT);
    
        gpio_put(LED_RED_GPIO, 1);
        gpio_put(LED_GREEN_GPIO, 1);
        gpio_put(LED_BLUE_GPIO, 1);
    }
    
    void leds_turn_on() {
        gpio_put(LED_RED_GPIO, 0);
        gpio_put(LED_GREEN_GPIO, 0);
        gpio_put(LED_BLUE_GPIO, 0);
    }
    
    void leds_turn_blue() {
        gpio_put(LED_RED_GPIO, 1);
        gpio_put(LED_GREEN_GPIO, 1);
        gpio_put(LED_BLUE_GPIO, 0);
    }
    
    void leds_turn_yellow() {
        gpio_put(LED_RED_GPIO, 0);
        gpio_put(LED_GREEN_GPIO, 0);
        gpio_put(LED_BLUE_GPIO, 1);
    }
    
    void leds_turn_off() {
        gpio_put(LED_RED_GPIO, 1);
        gpio_put(LED_GREEN_GPIO, 1);
        gpio_put(LED_BLUE_GPIO, 1);
    }
}