//links to C++ functions
extern "C" {
    #[link_name = "blink_led"]
    fn blink_led(blink_gpio: i32, led_state: i32);

    #[link_name = "configure_led"]
    fn configure_led(blink_gpio: i32);

    #[link_name = "delay"]
    fn delay(seconds: i32);
}

const LED: i32 = 0x02; //GPIO for LED
const LOW: i32 = 0x00;
const HIGH: i32 = 0x01;

//Configure LED
unsafe fn _setup() {
    configure_led(LED);
}

//Blink LED
unsafe fn _loop() {
    blink_led(LED, LOW);
    delay(500);
    blink_led(LED, HIGH);
    delay(500);
}

#[no_mangle]
fn main() {
    unsafe{
        _setup();
        loop {
            _loop();
        }
    }
}