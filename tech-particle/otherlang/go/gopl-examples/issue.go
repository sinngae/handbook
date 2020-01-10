/************************************************
> File Name: issue.go
> Author: Philo
> Mail: 
> Created Time: Tue 26 Nov 2019 01:16:53 PM CST
*************************************************/
// Issues prints a table of Github issues matching the search terms
// go build issue.go
// ./issue repo:golang/go is:open json decoder
// ./issueshtml repo:golang/go commenter:gopherbot json encoder >issues.html

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

    //"text/template"
    "html/template"

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
    //log.Println("XXXXXXXXXXXX   0000")
    q := url.QueryEscape(strings.Join(terms, " "))
    resp, err := http.Get(IssuesURL + "?q=" + q)
    if err != nil {
        return nil, err
    }
    //log.Println("XXXXXXXXXXXX   0001")

    if resp.StatusCode != http.StatusOK {
        resp.Body.Close()
        return nil, fmt.Errorf("search query failed: %s\n", resp.Status)
    }
    //log.Println("XXXXXXXXXXXX   0002")

    var result IssuesSearchResult
    if err := json.NewDecoder(resp.Body).Decode(&result); err != nil {
        resp.Body.Close()
        return nil, err
    }
    //log.Println("XXXXXXXXXXXX   0003")

    resp.Body.Close()
    return &result, nil
}

const temp0 = `{{.TotalCount}} issues:
{{range .Items}}------------------------------------
Number: {{.Number}}
User:   {{.User.Login}}
Title:  {{.Title | printf "%.64s"}}
Age:    {{.CreatedAt | daysAgo}} days
{{end}}`

func daysAgo(t time.Time) int {
    return int(time.Since(t).Hours() / 24)
}

func main() {
    //log.SetFlags(log.Lshortfile | log.LstdFlags)
    //log.Println("hi, err, start ...")
    result, err := SearchIssues(os.Args[1:])
    if err != nil {
        log.Fatal(err)
    }
    //log.Printf("%d issue:\n", result.TotalCount)
    /*for _, item := range result.Items {
        log.Printf("#%-5d %9.9s %.55s\n", item.Number, item.User.Login, item.Title)
    }*/

    /*report, err := template.New("report").
        Funcs(template.FuncMap{"daysAgo": daysAgo}).
        Parse(temp0)
    if err != nil {
        log.Fatal(err)
    }*/
    /*report := template.Must(template.New("report").
        Funcs(template.FuncMap{"daysAgo": daysAgo}).
        Parse(temp0))

    if err := report.Execute(os.Stdout, result); err != nil {
        log.Fatal(err)
    }*/

    // html template
    var issueList = template.Must(template.New("issuelist").Parse(`
    <h1>{{.TotalCount}} issues</h1>
    <table>
      <tr style='text-align: left'>
        <th>#</th>
        <th>State</th>
        <th>User</th>
        <th>Title</th>
      </tr>
      {{range .Items}}
      <tr>
        <td><a href='{{.HTMLURL}}'>{{.Number}}</a></td>
        <td>{{.State}}</td>
        <td><a href='{{.User.HTMLURL}}'>{{.User.Login}}</a></td>
        <td><a href='{{.HTMLURL}}'>{{.Title}}</a></td>
      </tr>
      {{end}}
    </table>
    `))
    if err := issueList.Execute(os.Stdout, result); err != nil {
        log.Fatal(err)
    }
}
