package internal

import (
	"fmt"

	"github.com/sinngae/handtool/cmd/hh/internal/internal/schema"

	"github.com/sinngae/handtool/cmd/hh/internal/internal/usecase/depart"

	"github.com/spf13/cobra"
)

var cmdDepart = &cobra.Command{
	Use:   "depart",
	Short: "depart sql",
	Run: func(cmd *cobra.Command, args []string) {
		depart.Depart(&schema.DepartCfg{
			Source:     source,
			Output:     output,
			ShareCount: shareCount,
		})
		fmt.Printf("%s Done\n", cmd.Use)
	},
}

func init() {
	cmdRoot.AddCommand(cmdDepart)
}
