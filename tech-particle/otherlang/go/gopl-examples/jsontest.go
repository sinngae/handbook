/************************************************
  > File Name: jsontest.go
  > Author: Philo
  > Mail: 
  > Created Time: Sun 15 Dec 2019 10:30:54 AM CST
******************************************/
package main

import (
    "encoding/json"
    "fmt"
    "log"
)

type Movie struct {
    Title   string
    Year    int
}

var movies = []Movie{
    {Title: "a movie", Year: 1942},
    {Title: "b movie", Year: 8327},
}

var batchStr = "[1, 2, 3]"

func main() {
    data, err := json.MarshalIndent(movies, "", "    ")
    //data, err := json.Marshal(movies)
    if err != nil {
        log.Fatalf("json marshaling failed: %s", err)
    }
    fmt.Printf("%s\n", data)

    var movies2 []Movie
    if err := json.Unmarshal(data, &movies2); err != nil {
        log.Fatalf("json unmarshal failed:%s", err)
    }
    fmt.Println(movies2)

    var idBatch []int64
    if err := json.Unmarshal([]byte(batchStr), &idBatch); err != nil {
        log.Fatalf("json unmarshal failed:%s", err)
    }
    fmt.Println(idBatch)
}
