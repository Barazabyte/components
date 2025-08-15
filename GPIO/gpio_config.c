#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "hal/gpio_types.h"


void vConfigPin(
	int iPIN,
	gpio_mode_t PIN_MODE, 
	gpio_int_type_t INTR_MODE, 
	gpio_pullup_t PULL_UP, 
	gpio_pulldown_t PULL_DOWN
){

	gpio_config_t pin_config = {
		.pin_bit_mask = (1ULL << iPIN),
		.mode		  = PIN_MODE,
		.pull_up_en   = PULL_UP,
		.pull_down_en = PULL_DOWN,
		.intr_type    = INTR_MODE
	};

	ESP_ERROR_CHECK(gpio_config(&pin_config));
	ESP_LOGI("GPIO", "Configuration was successful");
	vTaskDelay(pdMS_TO_TICKS(200));
}
