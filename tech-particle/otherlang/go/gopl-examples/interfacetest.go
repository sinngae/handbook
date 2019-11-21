package main

import (
    "fmt"
)

type MyInterface interface {
    Test() int
}

func (m *MyInterface) Test() int {
    return 100
}

func Func(entity MyInterface) int {
    return entity.Test()
}

type MyStruct struct {
    a int
}

func (m *MyStruct) Test() int {
    return m.a
}

func main() {
    var ret int
    var entity MyInterface
    ret = Func(entity)
    fmt.Println(ret)

    var instance MyStruct
    ret = Func(instance)
    fmt.Println(ret)

    var inter MyInterface
    inter = MyStruct{12}
    ret = Func(inter)
    fmt.Println(ret)
}
