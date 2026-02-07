#include "common.h"
#include "input.h"

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const uint8_t input_gpio[INPUT_GPIO_COUNT] =
{
    GPIO_NUM_27, GPIO_NUM_26, GPIO_NUM_25, GPIO_NUM_33, GPIO_NUM_15,
};

static const gpio_config_t input_config =
{
    .pull_down_en = GPIO_PULLDOWN_ENABLE,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .mode = GPIO_MODE_DEF_INPUT,
    .pin_bit_mask = (1ULL<<(uint64_t)input_gpio[0])
                  | (1ULL<<(uint64_t)input_gpio[1])
                  | (1ULL<<(uint64_t)input_gpio[2])
                  | (1ULL<<(uint64_t)input_gpio[3])
                  | (1ULL<<(uint64_t)input_gpio[4]),
    .intr_type = GPIO_INTR_DISABLE,
}; 

void input_init(void)
{
    ESP_ERROR_CHECK(gpio_config(&input_config));

	for (uint8_t i = 0; i < INPUT_GPIO_COUNT; i++)
	{
        gpio_input_enable(input_gpio[i]);
        gpio_set_pull_mode(input_gpio[i], GPIO_PULLDOWN_ONLY);
        ESP_ERROR_CHECK(gpio_set_level(input_gpio[i], 0));
	}
}

uint8_t input_get_state(void)
{
    volatile uint8_t state = 0x00;

    for (uint8_t c = 0; c < INPUT_GPIO_COUNT; c++)
    {
        if (gpio_get_level(input_gpio[c]) == 1)
        {
            state |= (1ULL << c);
        }
    }

    return state;
}
