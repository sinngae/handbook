fn main() {
    let rust = "Rust";
	println!("hi,{}", rust);

    let a1 = 5;
    let a2:i32 = 5;
    assert_eq!(a1, a2);
    let b1:u32 = 5;
    //assert_eq!(a1, b1);

    let mut a: f64 = 1.0;
    let b = 2.0f32;
    a = 2.0;
    println!("a:{:?}", a);

    let a = a;

    //a = 3.0;
    //assert_eq!(a, b);
    //
    let mut num = 5;
    {
        let mut add_num = move |x:i32| num + x;
        add_num(5);
        println!("num:{}", add_num(5));
    }
    println!("num:{}", num);
}
