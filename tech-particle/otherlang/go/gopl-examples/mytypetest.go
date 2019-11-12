// test mytype
// it is needed to copy mytype.go into $GOPATH/src
package main

import (
	atype "aren/mytype"
	"fmt"
)

func main() {
	var val0 atype.Ctype = 1
	var val1 atype.Dtype = 2
	fmt.Println(val0, val1)
	//val0 = val1 // compile error
	val0 = atype.Ctype(val1)
	fmt.Println(val0, val1)

	fmt.Println(val0.PrintFunc())
}
