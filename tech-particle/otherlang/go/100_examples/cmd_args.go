// test command arguments
package main

import (
	"fmt"
	"os"
	"strconv"
)

func main() {
	fmt.Println(os.Args[0]);
	var s, sep string

	opt, err := strconv.Atoi(os.Args[1])
	if err != nil {
		panic(err)
	}
	switch opt {
	case 1:
		for i := 2; i < len(os.Args); i++ {
			s += sep + os.Args[i]
			sep = " "
		}
		fmt.Println(s)
		break;
	case 2:
		for _, arg := range os.Args[2:] {
			s += sep + arg
			sep = "_"
		}
		fmt.Println(s)
		break;
	default:
		fmt.Println("not supported case");
	}
}
