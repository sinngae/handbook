package depart

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"

	"github.com/sinngae/handtool/cmd/hh/internal/internal/schema"
)

func Depart(cfg *schema.DepartCfg) {
	srcFile, err := os.Open(cfg.Source)
	if err != nil {
		log.Fatalf("open source file[%s] failed", cfg.Source)
		return
	}
	defer srcFile.Close()

	outFile, err := os.OpenFile(cfg.Output, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	if err != nil {
		log.Fatalf("open out file[%s] failed", cfg.Output)
		return
	}
	defer outFile.Close()

	scanner := bufio.NewScanner(srcFile)
	scanner.Split(ScanSql)
	buf := make([]byte, 4096)
	scanner.Buffer(buf, bufio.MaxScanTokenSize)
	for scanner.Scan() {
		body := scanner.Text() + ";"
		idx := strings.Index(body, "CREATE")
		idx2 := strings.Index(body, "create")
		log.Printf("%s, %d, %d\n", body, idx, idx2)
		if idx2 > idx {
			idx = idx2
		}
		if idx != 0 { // 非 create 语句 // XXX error ?
			continue
		}
		idxTag := "00000000"
		if strings.Contains(body, idxTag) {
			for tabIdx := 0; tabIdx < 1024; tabIdx++ {
				tabIdxStr := fmt.Sprintf("%08d", tabIdx)
				bodyTmp := strings.Replace(body, idxTag, tabIdxStr, 1)
				bodyTmp = fmt.Sprintf("-- idx %s\n%s\n\n", tabIdxStr, bodyTmp)
				_, err := outFile.Write([]byte(bodyTmp))
				if err != nil {
					log.Fatal(err)
				}
			}
		}
	}
	if scanner.Err() != nil {
		log.Printf("scanner err:%v\n", scanner.Err())
	}
}

// dropCR drops a terminal \r from the data.
// from go src code
func dropCR(data []byte) []byte {
	if len(data) > 0 && data[len(data)-1] == '\r' {
		return data[0 : len(data)-1]
	}
	return data
}
