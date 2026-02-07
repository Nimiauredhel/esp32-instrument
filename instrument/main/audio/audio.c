#include "audio.h"

#include "common.h"
#include "pitches.h"
#include "driver/ledc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const ledc_timer_config_t audio_timer_config =
{
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 440,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .clk_cfg = LEDC_USE_REF_TICK,
};

static const ledc_channel_config_t audio_channel_config =
{
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty = 0,
    .channel = LEDC_CHANNEL_0,
    .gpio_num = AUDIO_PWM_PIN,
    .timer_sel = LEDC_TIMER_0,
};

void audio_init(void)
{
    printf("Initializing LEDC for audio.\n");

    if (AUDIO_GND_PIN >= 0)
    {
        gpio_set_pull_mode(AUDIO_GND_PIN, GPIO_PULLDOWN_ONLY);
        gpio_set_level(AUDIO_GND_PIN, 0);
    }

    ESP_ERROR_CHECK(ledc_timer_config(&audio_timer_config));
    ESP_ERROR_CHECK(ledc_channel_config(&audio_channel_config));
}

void set_audio(uint32_t pitch_period, float duty_percent)
{
    uint32_t duty_resolution = 8;
    uint32_t duty_max = 255;

    if (duty_percent > 0.0f)
    {
        ESP_ERROR_CHECK(ledc_timer_set(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0, pitch_period, duty_resolution, LEDC_APB_CLK));
    }

    ESP_ERROR_CHECK(ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_max * duty_percent));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0));
}

void stop_audio(void)
{
	set_audio(0, 0);
}

void deinit_audio(void)
{
}
