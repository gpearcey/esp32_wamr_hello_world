//links to C++ functions
extern "C" {
    #[link_name = "printHi"]
    fn printHi(number: i32) -> i32;
}

#[no_mangle]
fn main() {

    unsafe{
        printHi(11);

    }
}