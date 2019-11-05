package main

import (
	"fmt"
	"io/ioutil"
	//"io"
	"net/http"
	"os"
	"strings"
)

func main() {
	for _, url := range os.Args[1:] {
		fmt.Println("XXXXXXXXXXXXXX   0000  ", url)
		if ! strings.HasPrefix(url, "http://") {
			url = "http://" + url
		}
		resp, err := http.Get(url)
		if err != nil {
			fmt.Fprintf(os.Stderr, "fetch:%v\n", err)
			os.Exit(1)
		}
		fmt.Println(resp.Status, resp.StatusCode, resp.Proto)

		//io.Copy(os.Stdout, resp.Body)
		/*b*/_, err0 := ioutil.ReadAll(resp.Body)
		resp.Body.Close()
		if err0 != nil {
			fmt.Fprintf(os.Stderr, "fetch: reading %s: %v\n", url, err0)
			os.Exit(1)
		}
		//fmt.Printf("%100s", b)
	}
}
