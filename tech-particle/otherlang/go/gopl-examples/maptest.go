package main

import (
    "fmt"
)

func main() {
    /*mmap := map[string]string{"key":"value", "a":"b", "1":"2", "df":"adf"}
    fmt.Printf("map: %v\n", mmap)

    if value, ok := mmap["a"]; ok {
        fmt.Printf("value: %v\n", value)
        value = "test"
        fmt.Printf("value: %v\n", mmap["a"])
        mmap["a"] = "test"
        fmt.Printf("value: %v\n", mmap["a"])
    }*/

    type MyType struct {
        A   int32
        B   int32
    }
    //var myTypeMap map[int32]MyType
    myTypeMap := make(map[int32]*MyType, 2)
    fmt.Printf("myTypeMap: %v\n", myTypeMap)
    //fmt.Printf("myTypeMap[1].A: %v\n", myTypeMap[1].A)
    if _, ok := myTypeMap[1] ; !ok {
        myTypeMap[1] = new(MyType)
    }
    myTypeMap[1].A += 1
    //myTypeMap[2].B = 2
    fmt.Printf("myTypeMap: %v\n", myTypeMap)
}
