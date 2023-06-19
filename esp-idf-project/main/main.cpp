#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "esp_system.h"
#include "wasm3.h"
#include "driver/gpio.h"
#include "extra/fib32.wasm.h"
#include "esp_log.h"
#include <m3_env.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 * WebAssembly app 
*/
#include "wasm_project.wasm.h"

#define WASM_STACK_SLOTS (512)
#define WASM_MEMORY_LIMIT (2*1024)
#define NATIVE_STACK_SIZE (4*1024)


static const char* TAG = "main.cpp";


/**
 * Converts int to GPIO_NUM enum value
*/
bool getGPIO(uint32_t gpio_num, gpio_num_t &gpio_num_value){
    switch(gpio_num){
        case 0: 
            gpio_num_value = GPIO_NUM_0;
            return true;
        case 1: 
            gpio_num_value = GPIO_NUM_1;
            return true;
        case 2: 
            gpio_num_value =  GPIO_NUM_2;
            return true;
        case 3:
            gpio_num_value =  GPIO_NUM_3;
            return true;
        case 4:
            gpio_num_value =  GPIO_NUM_4;
            return true;
        case 5:
            gpio_num_value =  GPIO_NUM_5;
            return true;
        case 6:
            gpio_num_value =  GPIO_NUM_6;
            return true;
        case 7:
            gpio_num_value =  GPIO_NUM_7;
            return true;
        case 8:
            gpio_num_value =  GPIO_NUM_8;
            return true;
        case 9:
            gpio_num_value =  GPIO_NUM_9;
            return true;
        case 10:
            gpio_num_value =  GPIO_NUM_10;
            return true;
        case 11:
            gpio_num_value =  GPIO_NUM_11;
            return true;
        case 12:
            gpio_num_value =  GPIO_NUM_12;
            return true;
        case 13:
            gpio_num_value =  GPIO_NUM_13;
            return true;
        case 14:
            gpio_num_value =  GPIO_NUM_14;
            return true;
        case 15:
            gpio_num_value =  GPIO_NUM_15;
            return true;
        case 16:
            gpio_num_value =  GPIO_NUM_16;
            return true;
        case 17:
            gpio_num_value =  GPIO_NUM_17;
            return true;
        case 18:
            gpio_num_value =  GPIO_NUM_18;
            return true;
        case 19:
            gpio_num_value =  GPIO_NUM_19;
            return true;
        case 21:
            gpio_num_value =  GPIO_NUM_21;
            return true;
        case 22:
            gpio_num_value =  GPIO_NUM_22;
            return true;
        case 23:
            gpio_num_value =  GPIO_NUM_23;
            return true;
        case 25:
            gpio_num_value =  GPIO_NUM_25;
            return true;
        case 26:
            gpio_num_value =  GPIO_NUM_26;
            return true;
        case 27:
            gpio_num_value =  GPIO_NUM_27;
            return true;
        case 32:
            gpio_num_value =  GPIO_NUM_32;
            return true;
        case 33:
            gpio_num_value =  GPIO_NUM_33;
            return true;
        case 34:
            gpio_num_value =  GPIO_NUM_34;
            return true;
        case 35:
            gpio_num_value =  GPIO_NUM_35;
            return true;
        case 36:
            gpio_num_value =  GPIO_NUM_36;
            return true;
        case 39:
            gpio_num_value =  GPIO_NUM_39;
            return true;
        default:
            ESP_LOGW(TAG, "Invalide GPIO selected \n");
            return false;
    }


}

/********************************************************************************************************************
 * Foward Declarations
*/

void runWasmFile(const char *path);


/*************************************************************************************************************
 * ESP32 LED functions
*/
void blink_led(int32_t blink_gpio, uint32_t led_state)
{
    gpio_num_t blink_gpio_val;
    getGPIO(blink_gpio, blink_gpio_val);
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(blink_gpio_val, led_state);
    ESP_LOGI(TAG,"blink_led called with state %" PRIu32 "", led_state);
};

void configure_led(int32_t blink_gpio)
{
    gpio_num_t blink_gpio_val;
    getGPIO(blink_gpio, blink_gpio_val);
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(blink_gpio_val);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(blink_gpio_val, GPIO_MODE_OUTPUT);
};

void delay(int32_t ms){
    vTaskDelay(pdMS_TO_TICKS(ms)); // Perform the delay
};

/********************************************************************************************************************
 * API Bindings
*/
m3ApiRawFunction(m3_blink_led){
    m3ApiGetArg(int32_t, blink_gpio);
    m3ApiGetArg(uint8_t, led_state);
    blink_led(blink_gpio,led_state);

    m3ApiSuccess();
};

m3ApiRawFunction(m3_configure_led){
    m3ApiGetArg(int32_t, blink_gpio);
    configure_led(blink_gpio);
    m3ApiSuccess();
};

m3ApiRawFunction(m3_delay){
    m3ApiGetArg(int32_t, ms);
    delay(ms);
    m3ApiSuccess();
};
   

/**
 * Linking led control functions
*/
M3Result  LinkESP32(IM3Runtime runtime)
{
    IM3Module module = runtime->modules;
    const char* module_name = "env";

    m3_LinkRawFunction (module, module_name, "blink_led",           "v(ii)",    &m3_blink_led);
    m3_LinkRawFunction (module, module_name, "configure_led",       "v(i)",    &m3_configure_led);
    m3_LinkRawFunction (module, module_name, "delay",               "v(i)",    &m3_delay);

    return m3Err_none;
}

/**
 * wasm_task
 * Parses wasm module
*/
void wasm_task(void *arg){
    M3Result result = m3Err_none;

    printf("Loading WebAssembly...\n");
    IM3Environment env = m3_NewEnvironment ();
    if (!env) ESP_LOGE(TAG,"m3_NewEnvironment failed");

    IM3Runtime runtime = m3_NewRuntime (env, WASM_STACK_SLOTS, NULL);
    if (!runtime) ESP_LOGE(TAG,"m3_NewRuntime failed");


    runtime->memoryLimit = WASM_MEMORY_LIMIT;

    IM3Module module;
    result = m3_ParseModule (env, &module, wasm_project_wasm, wasm_project_wasm_len);
    if (result) ESP_LOGE(TAG,"m3_ParseModule: %s", result);

    result = m3_LoadModule (runtime, module);
    if (result) ESP_LOGE(TAG,"LoadModule: %s", result);

    result = LinkESP32 (runtime);
    if (result) ESP_LOGE(TAG,"LinkESP32: %s", result);

    IM3Function f;
    result = m3_FindFunction (&f, runtime, "_start");
    if (result) ESP_LOGE(TAG,"m3_FindFunction: %s", result);

    result = m3_CallV(f);

    if (result) ESP_LOGE(TAG,"m3_GetResults: %s", result);//should not arrive here
}

void runWasmFile(const char *path) {

    xTaskCreate(
        &wasm_task,           // Pointer to the task entry function.
        "wasm3",              // A descriptive name for the task for debugging.
        NATIVE_STACK_SIZE,    // size of the task stack in bytes.
        NULL,                 // Optional pointer to pvParameters
        5,                    // priority at which the task should run
        NULL                  // Optional pass back task handle
    );
    return;
}


extern "C" void app_main(void)
{
    runWasmFile("startup.wasm");
}
