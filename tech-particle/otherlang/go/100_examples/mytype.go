package mytype

import "fmt"

type Ctype int
type Dtype int

func (c Ctype) PrintFunc() string { return fmt.Sprintf("%d Ctype", c) }
