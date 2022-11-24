# 20.05.2021

Na dzisiejszych zajęciach realizujemy kompletny program wykorzystujący REST API serwisu [Discogs](https://www.discogs.com/)
- [x] Zarejestruj się jako deweloper w serwisie [Discogs](https://www.discogs.com/) ponieważ tylko zarejestrowane aplikacje mogą korzystać z [zasobu search](https://www.discogs.com/developers#page:database,header:database-search) umożliwiającego wyszukiwanie po nazwie
- [x] Napisz program, który w oparciu o informacje z Discogs sprawdza, czy członkowie (aktualni bądź byli) podanego zespołu grali razem w jakichś innych zespołach. Wyświetl nazwy tych innych zespołów oraz nazwiska muzyków. Program powinien obsługiwać dwa rodzaje argumentów podawanych podczas wywołania (do wyboru przez użytkownika):
    1) ID zespołu w bazie Discogs (znalezione np.  poprzez stronę WWW)
    2) Tekstowa nazwa zespołu (uwaga: program nie powinien być wrażliwy np. na wielkość liter)

    Dla przykładu: **Budka Suflera (ID=359282)**: według Discogs grali w niej **Mieczysław Jurecki
(ID=702387)** i **Marek Stefankiewicz (ID=532854)**, którzy grali też w **Perfekcie (ID=669348)**.
Zadbaj aby drukowane wyniki były ładnie posortowane po nazwie zespołu, oraz aby w przypadku więcej niż dwóch osób grających razem w innym zespole kompresować wyniki (np. w wypadku czterech osób grających w innym zespole X należy je wypisać jako pojedynczą czwórkę, a nie jako sześć par).

- [x] Pamiętaj o obsludze błędów związanych z czynnikami takimi jak:
    - niepoprawne dane wejściowe podane przez użytkownika
    - autoryzacja jako zarejestrowany deweloper
    - nieudane zapytania HTTP
    - parsowanie danych otrzymanych z serwera

**To zadanie jest ważne — zaimplementowany program proszę przesłać przez formularz zadania w MS Teams do czwartku 2. czerwca.**
### Program

Program nalezy uruchomic nazwą bądź ID zespołu w Discogs `(flaga --name lub --id)`
Do programu należy dodać własny token uwierzytelniający `(--token $(TOKEN) lub --conf $(CONFIGURATION_FILE))`

Dodatkowo:
program można uruchomić z flagą `-h`, wyświetlona jest wtedy pomoc (dodatkowe informacje o flagach)
program można uruchomic z flagą `-b` lub `--bar` (wymagana biblioteka alive_progress) aby wyświetlić pasek postępu

Przykładowe uruchomienie:
    `python3 main.py --name Budka Suflera --conf configuration.json`
        lub
    `python3 main.py --name Budka Suflera --conf configuration.json --bar`