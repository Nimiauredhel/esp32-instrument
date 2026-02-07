
#include "common.h"

#include "audio.h"
#include "input.h"
#include "pitch_range.h"
#include "pitches.h"
#include <math.h>

#define TASK_STACK_SIZE 4096

static StaticTask_t main_task_buffer;
static TaskHandle_t main_task_handle = NULL;
static StackType_t main_task_stack[TASK_STACK_SIZE];

void main_task(void *arg)
{
    uint8_t prev_input_state = 0x00;
    audio_init();
    input_init();

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        const uint8_t input_state = input_get_state();
        if(input_state == prev_input_state) continue;
        prev_input_state = input_state;

        //printf("INPUT %u\n", input_state);
        //vTaskDelay(pdMS_TO_TICKS(10));

        if (input_state == 0x00)
        {
            stop_audio();
        }
        else
        {
            int pitch_modifier = (input_state >> 1U) * ((input_state & 1U) ? -1 : 1);

            printf("MODIFIER %d\n", pitch_modifier);

            int pitch_index = MIDDLE_PITCH_IDX + pitch_modifier;
            if (pitch_index < 0) pitch_index = 0;
            else if (pitch_index > NUM_PITCHES_IN_RANGE) pitch_index = NUM_PITCHES_IN_RANGE;
            set_audio(preset_pitches[pitch_index], 0.5f);
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
