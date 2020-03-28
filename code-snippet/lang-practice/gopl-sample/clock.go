/************************************************
  > File Name: clock.go
  > Author: Philo
  > Mail: 
  > Created Time: Tue 28 Jan 2020 10:38:02 PM CST
*************************************************/
package main

import (
    "io"
    "log"
    "net"
    "time"
    "bufio"
    "fmt"
    "strings"
)

func main() {
    listener, err := net.Listen("tcp", "localhost:8000")
    if err != nil {
        log.Fatal(err)
    }

    for {
        conn, err := listener.Accept()
        if err != nil {
            log.Print(err)
            continue
        }
        go handleConn(conn)
    }
}

func handleConn(c net.Conn) {
    defer c.Close()

    input := bufio.NewScanner(c)
    for input.Scan() {
        echo(c, input.Text(), 1 * time.Second)
    }

    for {
        _, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
        if err != nil {
            return
        }
        time.Sleep(1 * time.Second)
    }
}

func echo (c net.Conn, shout string, delay time.Duration) {
    fmt.Fprintln(c, "\t", strings.ToUpper(shout))
    time.Sleep(delay)
    fmt.Fprintln(c, "\t", shout)
    time.Sleep(delay)
    fmt.Fprintln(c, "\t", strings.ToLower(shout))
}
