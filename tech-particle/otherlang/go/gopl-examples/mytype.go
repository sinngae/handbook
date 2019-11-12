package mytype

import "fmt"

type Ctype int
type Dtype int

var test int

func init() {
    test = 1
    fmt.Println("this is init")
}

func (c Ctype) PrintFunc() string { return fmt.Sprintf("%d Ctype", c) }
