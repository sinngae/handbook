/************************************************
  > File Name: issue.go
  > Author: Philo
  > Mail: 
  > Created Time: Tue 26 Nov 2019 01:16:53 PM CST
*************************************************/
// Issues prints a table of Github issues matching the search terms
package main

import (
    "fmt"
    "log"
    "os"
    "time"
    "encoding/json"
    "net/http"
    "net/url"
    "strings"

    //"test/github"
)
const IssuesURL = "https://api.github.com/search/issues"

type IssuesSearchResult struct {
    TotalCount int `json:"total_count"`
    Items []*Issue
}

type Issue struct {
    Number int
    HTMLURL string `json:"html_url"`
    Title string
    State string
    User *User
    CreatedAt time.Time `json:"created_at"`
    Body string // in markdown format
}

type User struct {
    Login string
    HTMLURL string `json:"html_url"`
}

func SearchIssues(terms []string) (*IssuesSearchResult, error) {
    fmt.Println("XXXXXXXXXXXX   0000")
    q := url.QueryEscape(strings.Join(terms, " "))
    resp, err := http.Get(IssuesURL + "?q=" + q)
    if err != nil {
        return nil, err
    }
    fmt.Println("XXXXXXXXXXXX   0001")

    if resp.StatusCode != http.StatusOK {
        resp.Body.Close()
        return nil, fmt.Errorf("search query failed: %s", resp.Status)
    }
    fmt.Println("XXXXXXXXXXXX   0002")

    var result IssuesSearchResult
    if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
        resp.Body.Close()
        return nil, err
    }
    fmt.Println("XXXXXXXXXXXX   0003")

    resp.Body.Close()
    return &result, nil
}

func main() {
    log.Println("hi, err, start ...")
    result, err := SearchIssues(os.Args[1:])
    if err != nil {
        log.Fatal(err)
    }
    fmt.Printf("%d issue:\n", result.TotalCount)
    for _, item := range result.Items {
        fmt.Printf("#%-5d %9.9s %.55s\n", item.Number, item.User.Login, item.Title)
    }
}
