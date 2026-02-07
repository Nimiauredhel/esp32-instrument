
#include "common.h"

#include "audio.h"
#include "input.h"
#include "pitch_range.h"
#include "esp_task_wdt.h"

#define TASK_STACK_SIZE 4096

static StaticTask_t main_task_buffer;
static TaskHandle_t main_task_handle = NULL;
static StackType_t main_task_stack[TASK_STACK_SIZE];

void main_task(void *arg)
{
    audio_init();
    input_init();

    stop_audio();

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        uint8_t input_state = input_get_state();

        if (input_state == 0x00)
        {
            stop_audio();
        }
        else
        {
            int8_t pitch_modifier = 0;
            uint8_t step = 1;

            for (uint8_t i = 1; i < INPUT_GPIO_COUNT; i++)
            {
                if (input_state & (1U << i))
                {
                    pitch_modifier += step;
                }

                step *= 2;
            }

            if (input_state & 1U)
            {
                pitch_modifier *= -1;
            }

            uint8_t pitch_index = MIDDLE_PITCH_IDX + pitch_modifier;
            set_audio(preset_pitches[pitch_index], 0.2f);
        }
    }
}

void app_main(void)
{
    main_task_handle = xTaskCreateStaticPinnedToCore(main_task, "Main_Task", TASK_STACK_SIZE, NULL, 10, main_task_stack, &main_task_buffer, 0);

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
