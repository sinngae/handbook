package main

import (
    "fmt"
    "unicode/utf8"
    "bytes"
)

func main () {
    s := "hi, string"
    fmt.Println(len(s))
    fmt.Println(s[0], s[7])
    //fmt.Println(s[len(s)]) // panic: index out of range

    str := "hi, 世界"
    fmt.Println(len(str))
    fmt.Println(utf8.RuneCountInString(str))
    for i := 0; i < len(str); {
        r, size := utf8.DecodeRuneInString(str[i:])
        fmt.Printf("%d\t%c\t%c\n", i, r, str[i])
        i += size
    }

    for i, r := range str {
        fmt.Printf("%d\t%q\t%d\n", i, r, r)
    }

    stemp := "abc"
    btemp := []byte(stemp)
    stemp2 := string(btemp)

    btemp[1] = 'x'
    //stemp2[1] = 'y' // error cannot

    fmt.Printf("%s\t%s\t%s\n", stemp, btemp, stemp2)

    fmt.Println(intsToString([]int{1, 2, 3}))
}

func intsToString(values []int) string {
    var buf bytes.Buffer
    buf.WriteByte('[')
    for i, v := range values {
        if i > 0 {
            buf.WriteString(", ")
        }
        fmt.Fprintf(&buf, "%d", v)
    }
    buf.WriteByte(']')
    return buf.String()
}
