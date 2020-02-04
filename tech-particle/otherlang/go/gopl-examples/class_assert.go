/************************************************
> File Name: class_assert.go
> Author: Philo
> Mail: 
> Created Time: Fri 24 Jan 2020 01:35:29 PM CST
*************************************************/
package main

import (
    "fmt"
    "io"
    "os"
)

func main() {
    var w io.Writer
    w = os.Stdout
    //fmt.Printf("%T\n", *w)
    rw := w.(io.ReadWriter)
    //fmt.Printf("%T\n", *rw)
    f, ok := w.(*os.File)
    b, ok := w.(*bytes.Buffer)
}
