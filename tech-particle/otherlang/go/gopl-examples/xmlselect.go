/************************************************
  > File Name: xmlselect.go
  > Author: Philo
  > Mail: 
  > Created Time: Tue 28 Jan 2020 03:41:12 PM CST
*************************************************/
package main

import (
    "encoding/xml"
    "fmt"
    "io"
    "os"
    "strings"
)

func main() {
    dec := xml.NewDecoder(os.Stdin)
    var stack []string
    for {
        tok, err := dec.Token()
        if err == io.EOF {
            break
        }
    }
}
