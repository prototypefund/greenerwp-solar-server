package main

import "log"
import "os"
import "fmt"

const SockAddr = "/run/energyd.sock"

func main() {
	if len(os.Args) < 2 || os.Args[1] != "status" && os.Args[1] != "daemon" {
		log.Fatal("Usage: energyd [daemon|status]")
	}
	if os.Args[1] == "status" {
		client()
	} else {
		server()
	}
	os.Exit(0)
}

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s\n", err.Error())
		os.Exit(1)
	}
}
