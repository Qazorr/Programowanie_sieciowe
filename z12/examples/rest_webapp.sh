#!/bin/sh

# Skrypt uruchamiajÄ…cy i demonstrujÄ…cy dziaÅ‚anie aplikacji z pliku
# rest_webapp.py. Skrypt przetestowano pod Debianem 7, czyli dystrybucjÄ…
# zainstalowanÄ… w pracowniach studenckich.
#
# Przy rÄ™cznym testowaniu  webaplikacji moÅ¼esz chcieÄ‡ uÅ¼yÄ‡ "curl -v" aby
# zobaczyÄ‡ nagÅ‚Ã³wki zapytaÅ„ i odpowiedzi HTTP.


# Zainicjuj bazÄ™ z danymi osÃ³b.

rm -f osoby.sqlite

sqlite3 osoby.sqlite "
CREATE TABLE osoby (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    imie VARCHAR,
    nazwisko TEXT,
    telefon TEXT,
    adres TEXT
);
INSERT INTO osoby VALUES (1, 'Anna', 'Nowak', '+48124569988',
    'Rynek GÅ‚Ã³wny 2, 30-001 KrakÃ³w');
INSERT INTO osoby VALUES (2, 'Jan', 'Kowalski', '+48127770022',
    'ul. Podzamcze 1, 31-001 KrakÃ³w');
"

# Dla pewnoÅ›ci wypisz na ekran jej zawartoÅ›Ä‡.

echo "PoczÄ…tkowa zawartoÅ›Ä‡ bazy:"
sqlite3 --header osoby.sqlite "SELECT * FROM osoby"

# Uruchom w tle serwer z webaplikacjÄ….

env -i python3 -u rest_webapp.py > stdout.txt 2> stderr.txt &
server_pid=$!
sleep 1

# Testy aplikacji:

echo
echo "Test 1: pobieranie rekordu"
curl http://127.0.0.1:8000/osoby/1

echo
echo "Test 2: uaktualnianie rekordu"
printf "nazwisko\tadres\n" > dane.tsv
printf "Kowalska\tul. Podzamcze 1, 31-001 KrakÃ³w\n" >> dane.tsv
curl --upload-file dane.tsv \
        --header "Content-Type: text/tab-separated-values; charset=UTF-8" \
        http://127.0.0.1:8000/osoby/1
# uÅ¼ycie opcji --upload-file zmienia domyÅ›lnÄ… metodÄ™ na PUT

echo
echo "Test 3: usuwanie rekordu"
curl --request DELETE http://127.0.0.1:8000/osoby/2

echo
echo "Test 4: dodawanie nowego rekordu"
printf "imie\tnazwisko\ttelefon\tadres\n" > dane.tsv
printf "Adam\tWiÅ›niewski\t+48120124433\tul. Reymonta 4\n" >> dane.tsv
curl --request POST --upload-file dane.tsv \
        --header "Content-Type: text/tab-separated-values; charset=UTF-8" \
        http://127.0.0.1:8000/osoby

echo
echo "Test 5: pobieranie caÅ‚ej bazy"
curl http://127.0.0.1:8000/osoby

# I jeszcze upewnienie siÄ™ co do zawartoÅ›ci pliku z bazÄ….

echo
echo "ZawartoÅ›Ä‡ bazy po zmianach:"
sqlite3 --header osoby.sqlite "SELECT * FROM osoby"

# Koniec testÃ³w, moÅ¼na wyÅ‚Ä…czyÄ‡ serwer aplikacyjny.

kill $server_pid
