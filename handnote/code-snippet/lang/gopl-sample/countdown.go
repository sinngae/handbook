/************************************************
  > File Name: countdown.go
  > Author: Philo
  > Mail: 
  > Created Time: Mon 03 Feb 2020 05:34:38 PM CST
*************************************************/
package main

import (
    "fmt"
    "time"
    "os"
)

func main() {
    abort := make(chan struct{})
    go func() {
        os.Stdin.Read(make([]byte, 1))
        abort <- struct{}{}
    }()
    fmt.Println("Commencing countdown.")
    //tick := time.Tick(1 * time.Second)
    ticker := time.NewTicker(1 * time.Second)
    defer ticker.Stop()
    for countdown := 10; countdown > 0; countdown-- {
        fmt.Println(countdown)
        //j := <-tick
        select {
        case j := <-ticker.C:
            fmt.Println(j)
            // do nothing
        case <-abort:
            fmt.Println("launch abort")
            return
        /*default:
            // do nothing?
            fmt.Println("default action")*/
        }
    }
    /*select {
    case <-time.After(10 * time.Second):
        // Do nothing
    case <-abort:
        fmt.Println("launch aborted!")
        return
    }*/
    launch()
}

func launch() {
    fmt.Println("Launch.")
}
