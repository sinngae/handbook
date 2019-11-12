// test hello
package main

import "fmt"

var val_a, val_b int = 1, 2
var val = "test";

var (
	val_c, val_d, _ = 3, "val", 0.1
)

func main() {
	fmt.Println(val_a /*+ "test"*/)
	fmt.Printf(val + " Hi, work\n")
}
