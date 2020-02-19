/************************************************
  > File Name: format.go
  > Author: Philo
  > Mail: 
  > Created Time: Fri 14 Feb 2020 11:12:39 AM CST
*************************************************/
package main

import (
    "reflect"
    "strconv"
    "fmt"
)

func Any(value interface{}) string {
    return formatAtom(reflect.ValueOf(value))
}

func formatAtom(v reflect.Value) string {
    switch v.Kind() {
    case reflect.Invalid:
        return "invalid"
    case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64:
        return strconv.FormatInt(v.Int(), 10)
    case reflect.Uint, reflect.Uint8, reflect.Uint16, reflect.Uint32, reflect.Uint64, reflect.Uintptr:
        return strconv.FormatUint(v.Uint(), 10)
        // ... floating-point and complex cases omitted for brevity ...
    case reflect.Bool:
        return strconv.FormatBool(v.Bool())
    case reflect.String:
        return strconv.Quote(v.String())
    case reflect.Chan, reflect.Func, reflect.Ptr, reflect.Slice, reflect.Map:
        return v.Type().String() + " 0x" + strconv.FormatUint(uint64(v.Pointer()), 16)
    default: // reflect.Array, reflect.Struct, reflect.Interface
        return v.Type().String() + " value"
    }
}

func Display(name string, x interface{}) {
    fmt.Printf("Dispaly %s (%T):\n", name, x)
    display(name, reflect.ValueOf(x))
}

func display(path string, v reflect.Value) {
    switch v.Kind() {
    case reflect.Invalid:
        fmt.Printf("%s = invalid\n", path)
    case reflect.Slice, reflect.Array:
        for i := 0; i < v.Len(); i++ {
            display(fmt.Sprintf("%s[%d]", path, i), v.Index(i))
        }
    case reflect.Struct:
        for i := 0; i < v.NumField(); i++ {
            fieldPath := fmt.Sprintf("%s.%s", path, v.Type().Field(i).Name)
            display(fieldPath, v.Field(i))
        }
    case reflect.Map:
        for _, key := range v.MapKeys() {
            display(fmt.Sprintf("%s[%s]", path, formatAtom(key)), v.MapIndex(key))
        }
    case reflect.Ptr:
        if v.IsNil() {
            fmt.Printf("%s = nil\n", path)
        } else {
            display(fmt.Sprintf("(%s)", path), v.Elem())
        }
    case reflect.Interface:
        if v.IsNil() {
            fmt.Printf("%s = nil\n", path)
        } else {
            fmt.Printf("%s.type = %s\n", path, v.Elem().Type())
            display(path + ".value", v.Elem())
        }
    default: // basic types, channels, func
        fmt.Printf("%s = %s\n", path, formatAtom(v))
    }
}

func main() {
    type myinf interface {
        test() int
    }
    type myst struct {
        a int
        b string
        inf myinf
    }
    var a = myst{ 1, "adfas", nil}
    fmt.Println(Any(a))
    fmt.Println(Any(a.a))
    fmt.Println(Any(a.b))
    fmt.Println(Any(a.inf))

    Display("", a)
}
