/************************************************
  > File Name: template.go
  > Author: Philo
  > Mail: 
  > Created Time: Tue 07 Jan 2020 10:27:36 AM CST
*************************************************/
package main

import (
    "log"
    "os"
    "html/template"
)

func main() {
    const templ = `<p>A: {{.A}}</p><p>B:{{.B}}</p>`
    t := template.Must(template.New("escape").Parse(templ))
    var data struct {
        A       string
        B       template.HTML
    }

    data.A = "<b>Hi</b>"
    data.B = "<b>Hi</b>"
    if err := t.Execute(os.Stdout, data); err != nil {
        log.Fatal(err)
    }
}
