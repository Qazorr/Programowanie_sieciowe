package main

import (
	"fmt"
	"log"
	"net"
	"os"
	"strconv"
)

func error_check(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func respond(conn *net.UDPConn, addr *net.UDPAddr) {
	var mess []byte = []byte("Hello world\r\n")
	_, err := conn.WriteToUDP(mess, addr)
	error_check(err)
}

func main() {
	if len(os.Args) != 2 {
		fmt.Print("Zle wykonanie, podaj port.\n")
		os.Exit(-1)
	}
	port, err := strconv.Atoi(os.Args[1])
	error_check(err)
	ip := "127.0.0.1"

	addr := net.UDPAddr{
		Port: port,
		IP:   net.ParseIP(ip),
	}

	ln, err := net.ListenUDP("udp", &addr)
	error_check(err)

	mess := make([]byte, 1024)

	for {
		_, client, err := ln.ReadFromUDP(mess)
		error_check(err)
		respond(ln, client)
	}
}
