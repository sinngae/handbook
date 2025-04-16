package internal

import (
	"context"
	"log"

	"github.com/spf13/cobra"
)

var (
	// flag value
	ctx        context.Context
	source     string
	output     string
	shareCount uint32

	// cmdRoot
	cmdRoot = &cobra.Command{
		Use:   "hh",
		Short: "hh is a tool as short hand helper for Dev",
		Long:  `hh is a tool as short hand helper for developer.`,
		PersistentPreRun: func(cmd *cobra.Command, args []string) {
			ctx = context.Background()
		},
		PersistentPostRun: func(cmd *cobra.Command, args []string) {
		},
	}
)

func init() {
	cmdRoot.PersistentFlags().StringVarP(&source, "source", "f", "demo.sql", "source file path")
	cmdRoot.PersistentFlags().StringVarP(&output, "output", "o", "temp.sql", "output file path")
	cmdRoot.PersistentFlags().Uint32VarP(&shareCount, "share", "s", 256, "table shares quantity to depart")
}

// Execute the refresh tool
func Execute() {
	if err := cmdRoot.Execute(); err != nil {
		log.Fatalln(err)
	}
}
