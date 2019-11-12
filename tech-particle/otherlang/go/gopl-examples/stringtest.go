package main

import (
    "fmt"
)

func main () {
    s := "hi, string"
    fmt.Println(len(s))
    fmt.Println(s[0], s[7])
    fmt.Println(s[len(s)]) // panic: index out of range
}
