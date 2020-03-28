/************************************************
  > File Name: reflecttest.go
  > Author: Philo
  > Mail: 
  > Created Time: Tue 11 Feb 2020 12:46:12 PM CST
*************************************************/
package main

import (
    "fmt"
    "reflect"
)

type StDao struct {
    value   int
    val     string
}

func main() {
    var demo = StDao{1, "a"}
    t := reflect.TypeOf(demo)
    fmt.Println(t.String())
    fmt.Println(t)
    //fmt.Println(StDao.String())

    v := reflect.ValueOf(demo)
    fmt.Println(v)
    fmt.Println(v.String())
}
