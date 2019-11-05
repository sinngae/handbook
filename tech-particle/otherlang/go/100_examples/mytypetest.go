package main

import "mytype"

func main() {
	var val0 Ctype = 1
	var val1 Dtype = 2
	fmt.Println(val0, val1)
	//val0 = val1 // compile error
	val0 = Ctype(val1)
	fmt.Println(val0, val1)

	fmt.Println(val0.PrintFunc())
}
