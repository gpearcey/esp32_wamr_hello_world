#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "esp_system.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "bh_platform.h"
#include "wasm_export.h"
#include "bh_read_file.h"
#include "bh_getopt.h"

static const char* TAG = "main.cpp";

void blink_led(int32_t blink_gpio, uint32_t led_state);
void configure_led(int32_t blink_gpio);
void delay(int32_t ms);
hi
/**
 * WebAssembly app 
*/
#include "wasm_project.wasm.h"
//
//#define WASM_STACK_SLOTS (512)
//#define WASM_MEMORY_LIMIT (2*1024)
//#define NATIVE_STACK_SIZE (4*1024)

static void * app_instance_main(wasm_module_inst_t module_inst)
{
    const char *exception;

    wasm_application_execute_main(module_inst, 0, NULL);
    if ((exception = wasm_runtime_get_exception(module_inst)))
        printf("%s\n", exception);
    return NULL;
}


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
void blink_led(wasm_exec_env_t exec_env, int32_t blink_gpio, uint32_t led_state)
{
    gpio_num_t blink_gpio_val;
    getGPIO(blink_gpio, blink_gpio_val);
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(blink_gpio_val, led_state);
    ESP_LOGI(TAG,"blink_led called with state %" PRIu32 "", led_state);
}

void configure_led(wasm_exec_env_t exec_env, int32_t blink_gpio)
{
    gpio_num_t blink_gpio_val;
    getGPIO(blink_gpio, blink_gpio_val);
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(blink_gpio_val);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(blink_gpio_val, GPIO_MODE_OUTPUT);
}

void delay(wasm_exec_env_t exec_env, int32_t ms){
    vTaskDelay(pdMS_TO_TICKS(ms)); // Perform the delay
}


/**
 * wasm_task
 * Parses wasm module
*/
int wasm_task(void *arg){

    (void)arg; /* unused */
    /* setup variables for instantiating and running the wasm module */
    uint8_t *wasm_file_buf = NULL;
    unsigned wasm_file_buf_size = 0;
    wasm_module_t wasm_module = NULL;
    wasm_module_inst_t wasm_module_inst = NULL;
    char error_buf[128];
    void *ret;
    RuntimeInitArgs init_args;

    // Define an array of NativeSymbol for the APIs to be exported.
    // Note: the array must be static defined since runtime
    //            will keep it after registration
    // For the function signature specifications, goto the link:
    // https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/export_native_api.md

    static NativeSymbol native_symbols[] = {
        {
            "blink_led", // the name of WASM function name
            blinkLed,   // the native function pointer
            "(ii)v",  // the function prototype signature, avoid to use i32
            NULL        // attachment is NULL
        },
        {
            "configure_led", // the name of WASM function name
            configLed,   // the native function pointer
            "(i)v",   // the function prototype signature, avoid to use i32
            NULL       // attachment is NULL
        },
        { "delay", delay, "(i)v", NULL }
    };
/* configure memory allocation */
    memset(&init_args, 0, sizeof(RuntimeInitArgs));
#if WASM_ENABLE_GLOBAL_HEAP_POOL == 0
    init_args.mem_alloc_type = Alloc_With_Allocator;
    init_args.mem_alloc_option.allocator.malloc_func = (void *)os_malloc;
    init_args.mem_alloc_option.allocator.realloc_func = (void *)os_realloc;
    init_args.mem_alloc_option.allocator.free_func = (void *)os_free;
#else
#error The usage of a global heap pool is not implemented yet for esp-idf.
#endif

    ESP_LOGI(LOG_TAG, "Initialize WASM runtime");
    /* initialize runtime environment */
    if (!wasm_runtime_full_init(&init_args)) {
        ESP_LOGE(LOG_TAG, "Init runtime failed.");
        return NULL;
    }

#if WASM_ENABLE_INTERP != 0
    ESP_LOGI(LOG_TAG, "Run wamr with interpreter");

    wasm_file_buf = (uint8_t *)wasm_test_file_interp;
    wasm_file_buf_size = sizeof(wasm_test_file_interp);

    /* load WASM module */
    if (!(wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_buf_size,
                                          error_buf, sizeof(error_buf)))) {
        ESP_LOGE(LOG_TAG, "Error in wasm_runtime_load: %s", error_buf);
        goto fail1interp;
    }

    ESP_LOGI(LOG_TAG, "Instantiate WASM runtime");
    if (!(wasm_module_inst =
              wasm_runtime_instantiate(wasm_module, 32 * 1024, // stack size
                                       32 * 1024,              // heap size
                                       error_buf, sizeof(error_buf)))) {
        ESP_LOGE(LOG_TAG, "Error while instantiating: %s", error_buf);
        goto fail2interp;
    }

    ESP_LOGI(LOG_TAG, "run main() of the application");
    ret = app_instance_main(wasm_module_inst);
    assert(!ret);

    /* destroy the module instance */
    ESP_LOGI(LOG_TAG, "Deinstantiate WASM runtime");
    wasm_runtime_deinstantiate(wasm_module_inst);

fail2interp:
    /* unload the module */
    ESP_LOGI(LOG_TAG, "Unload WASM module");
    wasm_runtime_unload(wasm_module);

fail1interp:
#endif
#if WASM_ENABLE_AOT != 0
    ESP_LOGI(LOG_TAG, "Run wamr with AoT");

    wasm_file_buf = (uint8_t *)wasm_test_file_aot;
    wasm_file_buf_size = sizeof(wasm_test_file_aot);

    /* load WASM module */
    if (!(wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_buf_size,
                                          error_buf, sizeof(error_buf)))) {
        ESP_LOGE(LOG_TAG, "Error in wasm_runtime_load: %s", error_buf);
        goto fail1aot;
    }

    ESP_LOGI(LOG_TAG, "Instantiate WASM runtime");
    if (!(wasm_module_inst =
              wasm_runtime_instantiate(wasm_module, 32 * 1024, // stack size
                                       32 * 1024,              // heap size
                                       error_buf, sizeof(error_buf)))) {
        ESP_LOGE(LOG_TAG, "Error while instantiating: %s", error_buf);
        goto fail2aot;
    }

    ESP_LOGI(LOG_TAG, "run main() of the application");
    ret = app_instance_main(wasm_module_inst);
    assert(!ret);

    /* destroy the module instance */
    ESP_LOGI(LOG_TAG, "Deinstantiate WASM runtime");
    wasm_runtime_deinstantiate(wasm_module_inst);

fail2aot:
    /* unload the module */
    ESP_LOGI(LOG_TAG, "Unload WASM module");
    wasm_runtime_unload(wasm_module);
fail1aot:
#endif

    /* destroy runtime environment */
    ESP_LOGI(LOG_TAG, "Destroy WASM runtime");
    wasm_runtime_destroy();

    return NULL;
}

void runWasmFile(const char *path) {

    xTaskCreate(
        &wasm_task,           // Pointer to the task entry function.
        "wamr",               // A descriptive name for the task for debugging.
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
    ESP_LOGI(LOG_TAG, "Exiting...");
}
