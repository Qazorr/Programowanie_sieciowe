package main

import (
	"fmt"
	"log"
	"net"
	"os"
)

func error_check(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func main() {
	if len(os.Args) != 2 {
		fmt.Print("Zle wykonanie, podaj port.\n")
		os.Exit(-1)
	}
	port := os.Args[1]
	ln, err := net.Listen("tcp", ":"+port)
	error_check(err)
	defer ln.Close()

	for {
		conn, err := ln.Accept()
		error_check(err)
		fmt.Fprintf(conn, "Hello World\r\n")
	}
}
