#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"


// Useful Constants
#define SOUND     0.0343


// configurations
void vUltrasonicConfig(int TRIG_PIN, int ECHO_PIN) {

    gpio_config_t io1_conf = {
        .pin_bit_mask  = (1ULL << TRIG_PIN),
        .mode          = GPIO_MODE_OUTPUT,
        .pull_up_en    = GPIO_PULLUP_DISABLE,
        .pull_down_en  = GPIO_PULLDOWN_DISABLE,
        .intr_type     = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&io1_conf));
    ESP_LOGI("OUTPUT", "Configured Successively");


    gpio_config_t io2_conf = {
        .pin_bit_mask  = (1ULL << ECHO_PIN),
        .mode          = GPIO_MODE_INPUT,
        .pull_up_en    = GPIO_PULLUP_DISABLE,
        .pull_down_en  = GPIO_PULLDOWN_DISABLE,
        .intr_type     = GPIO_INTR_DISABLE
    };

    ESP_ERROR_CHECK(gpio_config(&io2_conf));
    ESP_LOGI("INPUT", "Configured Successively");

}

// Calculator
int iDistanceCalc(float fDuration) {
    float fDistance = (SOUND * fDuration)/2.0f;

    return (int)fDistance;
}

// Task Implementation
int iGetDistance(int TRIG_PIN, int ECHO_PIN) {
    // timing variables
    int iEchoStart    = 0;
    int iEchoEnd      = 0;
    int iDuration     = 0;
    int iStartTime    = 0;
	int iRange  	  = 0;	


    // Emmiting US radiations
        ESP_ERROR_CHECK(gpio_set_level(TRIG_PIN, 1));
        esp_rom_delay_us(10);                                // create a delay of 10microsec
        ESP_ERROR_CHECK(gpio_set_level(TRIG_PIN, 0));

    // implementing Task operations
        iStartTime = esp_timer_get_time();
        while(gpio_get_level(ECHO_PIN) == 0) {
            if((esp_timer_get_time() - iStartTime) > 100000) { 
                ESP_LOGE("ECHOLOW", "time out");
                return -1;
            }
        }

        iEchoStart = esp_timer_get_time();
        while(gpio_get_level(ECHO_PIN) == 1) {
            if((esp_timer_get_time() - iEchoStart) > 300000) { 
                ESP_LOGE("ECHOHIGH", "time out");
                return -1;
            }
        }

        iEchoEnd = esp_timer_get_time();

        iDuration = iEchoEnd - iEchoStart;
		iRange	  = iDistanceCalc((float)iDuration);
    ESP_LOGI("DISTANCE", "value : %d", iDistanceCalc((float)iDuration));
    vTaskDelay(pdMS_TO_TICKS(10));
                 

	return iRange;


}
