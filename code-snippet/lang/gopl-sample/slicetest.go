package main

import (
    "fmt"
)

func main() {
    str := "hi, work"
    fmt.Printf("%s\t%d\n", str, len(str)/*, cap(str)*/) // cap a string return compile error

    astr := [10]byte{'1', '2', '3', '\000', '4'}
    fmt.Printf("%s\t%v\t%d\t%d\n", astr, astr, len(astr), cap(astr))

    astr2 := [...]byte{'1', '2', '3', '\000', '4'}
    fmt.Printf("%s\t%v\t%d\t%d\n", astr2, astr2, len(astr2), cap(astr2))

    sstr := []byte{'1', '2', '3', '\000', '4'}
    fmt.Printf("%s\t%v\t%d\t%d\n", sstr, sstr, len(sstr), cap(sstr))

    sstr2 := astr[2:5]
    fmt.Printf("%s\t%v\t%d\t%d\n", sstr2, sstr2, len(sstr2), cap(sstr2))

    var sstr3 []byte    // len(sstr3) == 0, sstr3 == nil
    sstr3 = nil         // len(sstr3) == 0, sstr3 == nil
    sstr3 = []byte(nil)  // len(sstr3) == 0, sstr3 == nil
    sstr3 = []byte{}     // len(sstr3) == 0, sstr3 != nil
    fmt.Printf("%s\t%v\t%d\t%d\n", sstr3, sstr3, len(sstr3), cap(sstr3))

    sstr4 := make([]byte, 10)[:5]
    fmt.Printf("%s\t%v\t%d\t%d\n", sstr4, sstr4, len(sstr4), cap(sstr4))
}
