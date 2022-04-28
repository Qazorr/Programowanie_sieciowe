#!/bin/bash

#encoding
openssl rsautl -encrypt -inkey keys/klucz_pub_serwera.pem -pubin -in messages/wiadomosc.txt -out messages/zakodowana.bin

#decoding
openssl rsautl -decrypt -inkey keys/server.key -in messages/zakodowana.bin -out messages/odkodowana.txt