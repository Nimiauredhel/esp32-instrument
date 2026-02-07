#ifndef INPUT_H
#define INPUT_H

#define INPUT_GPIO_COUNT (5)

extern const uint8_t input_gpio[INPUT_GPIO_COUNT];

void input_init(void);
uint8_t input_get_state(void);

#endif
