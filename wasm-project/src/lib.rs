//links to C++ functions
extern "C" {
    #[link_name = "printHello"]
    fn printHello(number: i32) -> i32;
}

#[no_mangle]
fn main() {

    unsafe{
        printHello(11);

    }
}