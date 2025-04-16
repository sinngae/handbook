package main

import (
	"fmt"
	"net/http"
)

func IndexHandler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintln(w, "hi, work")
}

func main() {
	http.HandleFunc("/", IndexHandler)

	http.ListenAndServe("127.0.0.1:8000", nil)
}
