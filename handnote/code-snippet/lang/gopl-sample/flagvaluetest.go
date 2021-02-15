package main

import (
    "fmt"
    "flag"
    "time"
)

type celsiusFlag struct {
    Celsius
}

func (f *celsiusFlag) Set(s string) error {
    var unit string
    var value float64
    fmt.Sscanf(s, "%f%s", &value, &unit)
    switch unit {
    case "C", ".C":
        f.Celsius = Celsius(value)
        return nil
    case "F", ".F":
        f.Celsius = FToC(Fahrenheit(value))
        return nil
    }
    return fmt.Errorf("invalid temperature %q", s)
}

func CelsiusFlag(name string, value Celsius, usage string) *Celsius {
    f := celsiusFlag{value}
    flag.CommandLine.Var(&f, name, usage)
    return &f.Celsius
}

var period = flag.Duration("period", 1*time.Second, "sleep period")
var temp = CelsiusFlag("temp", 20.0, "the temperatrue")

func main() {
    flag.Parse()
    fmt.Println(*temp)
    fmt.Printf("Sleeping for %v..", *period)
    time.Sleep(*period)
    fmt.Println()
}
