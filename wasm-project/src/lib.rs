//links to C++ functions
extern "C" {
    #[link_name = "printHi"]
    fn printHi() -> i32;
}


#[no_mangle]
fn main() {

    unsafe{
        printHi();

    }
}