/************************************************
  > File Name: test.go
  > Author: Philo
  > Mail: 
  > Created Time: Tue 26 Nov 2019 09:49:28 AM CST
*************************************************/
package main

import (
    "fmt"
)

import (
    "time"
)

func Add(a int, b int) int {
    return a + b
}

func main() {
    time := time.Now().Format("2006-01-02 15:04:05")
    fmt.Println(Add(3, 4))
    fmt.Println(time)
}
