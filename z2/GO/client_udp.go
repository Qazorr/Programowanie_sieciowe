package main

import (
	"bufio"
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
	if len(os.Args) != 3 {
		fmt.Print("Zle wykonanie, podaj ip i port.\n")
		os.Exit(-1)
	}
	ip := os.Args[1]
	port := os.Args[2]

	conn, err := net.Dial("udp", ip+":"+port)
	error_check(err)
	defer conn.Close()

	fmt.Fprintf(conn, "")

	mess := make([]byte, 1024)

	_, err = bufio.NewReader(conn).Read(mess)
	error_check(err)
	fmt.Printf("%s", mess)
}
