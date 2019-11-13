package main

import (
    "fmt"
)

func main() {
    mmap := map[string]string{"key":"value", "a":"b", "1":"2", "df":"adf"}
    fmt.Println(mmap)

    key, ok := mmap["a"];
    if ok {
        fmt.Println(mmap["a"])
        fmt.Println(key)
    }
    if key, ok = mmap["1"]; ok {
        fmt.Println(mmap["1"])
    }
}
