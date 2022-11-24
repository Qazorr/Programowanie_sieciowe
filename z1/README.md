# 4.03.2022

## [Program 1.c](1.c)

- [x] Napisz (albo odszukaj w archiwum zadań domowych z pierwszego roku studiów) program w C deklarujący tablicę int `liczby[50]` i wczytujący do niej z klawiatury kolejne liczby.  
Wczytywanie należy przerwać gdy użytkownik wpisze zero lub po wczytaniu 50 liczb.
- [x] Dodaj do powyższego programu pętlę for idącą przez wszystkie wczytane liczby i drukującą te z nich, które są większe od 10 i mniejsze od 100
- [x] Powyższy program prawdopodobnie używał indeksowania tablic. \
Jeśli tak, to przerób go aby korzystał ze wskaźników i aby nigdzie w kodzie nie występowało `liczby[i]`.
- [x] Przerób program tak, aby kończył działanie również wtedy, kiedy zabraknie na standardowym wejściu liczb do wczytania (wystąpi warunek "end-of-file", `EOF`). 
  :bulb: `EOF` wystąpi kiedy np. przekierujemy na standardowe wejście naszego programu liczby zapisane wcześniej w pliku tekstowym, np. 
    ```bash
        ./moj_program.x < plik_z_liczbami.txt
    ```
    
  :bulb: lub kiedy wejście do programu stanowi potok (pipe) z innego programu, np.
    ```bash
        printf "1 2 3 4 5" | ./moj_program.x
    ```


:question: Jak zasymulować `EOF` wpisując liczby z klawiatury?
:question: Zwróc uwagę na dokumentację odpowiednich funkcji jak scanf czy fgets. 
:question: Co robią w przypadku wystąpienia warunku `EOF`?

## [Program 2.c](2.c) 

- [x] Opracuj funkcję testującą czy przekazany jej bufor zawiera tylko i wyłącznie drukowalne znaki ASCII,tzn. bajty o wartościach z przedziału domkniętego [32, 126]. Funkcja ma mieć następującą sygnaturę: 
    ```c
        bool drukowalne(const void *buf, int len)
    ```
    :exclamation: Pamiętaj o włączeniu nagłówka `<stdbool.h>`, bez niego kompilator nie rozpozna ani nazwy typu bool ani nazw stałych `true` i `false`.
- [x] Zaimplementuj dwa warianty tej funkcji, w pierwszym wariancie funkcja ma pobierać kolejne bajty z bufora przy pomocy operacji indeksowania tablic, w drugim ma używać wskaźnika przesuwającego się z bajtu na bajt.
- [x] A teraz opracuj wersję, która jako argument dostaje łańcuch w sensie języka C, czyli ciąg niezerowych bajtów zakończony bajtem równym zero (ten końcowy bajt nie jest uznawany za należący do łańcucha). Ta wersja funkcji powinna mieć taką sygnaturę: 
    ```c 
    bool drukowalne(const char * buf)
    ```
- [x] Tu też zaimplementuj dwa warianty: 
  1. używający indeksowania
  2. używający przesuwającego się wskaźnika.

##  [Program 3.c](3.c)

- [x] W dokumentacji POSIX API znajdź opisy czterech podstawowych funkcji plikowego wejścia-wyjścia, tzn:
  1. [`open`](https://man7.org/linux/man-pages/man2/open.2.html)
  2. [`read`](https://man7.org/linux/man-pages/man2/read.2.html)
  3. [`write`](https://man7.org/linux/man-pages/man2/write.2.html)
  4. [`close`](https://man7.org/linux/man-pages/man2/close.2.html)
 
:question: Czy zgadzają się one z tym, co pamiętasz z przedmiotu [„Systemy operacyjne”](http://th.if.uj.edu.pl/~placzek/dydaktyka/SO/)?
:question: Jakie znaczenie ma wartość `0` zwrócona jako wynik funkcji `read`?
- [x] Zaimplementuj program kopiujący dane z pliku do pliku przy pomocy powyższych funkcji.
    :notebook: Zakładamy, że nazwy plików są podawane przez użytkownika jako argumenty programu (tzn. będą dostępne w tablicy argv). 
    :exclamation: Zwróć szczególną uwagę na obsługę błędów każde wywołanie funkcji we-wy musi być opatrzone testem sprawdzającym, czy zakończyło się ono sukcesem, czy porażką.

    :bulb: Funkcje POSIX zwracają `-1` aby zasygnalizować wystąpienie błędu, i dodatkowo zapisują w globalnej zmiennej **errno** kod wskazujący przyczynę wystąpienia błędu (na dysku nie ma pliku o takiej nazwie, brak wystarczających praw dostępu, itd.). 
    :bulb: Polecam Państwa uwadze pomocniczą funkcję **perror**, która potrafi przetłumaczyć ten kod na zrozumiały dla człowieka komunikat i wypisać go na ekranie.
- [x] Modyfikacja powyższego zadania. 
    :notebook: Zakładamy, że kopiowany **plik jest plikiem tekstowym**. 
    :notebook: Linie są zakończone **bajtami o wartości 10** (znaki `LF`, w języku C zapisywane jako `'\n'`). 
    :notebook: Podczas kopiowania należy pomijać **parzyste linie** (tzn. w pliku wynikowym mają się znaleźć pierwsza, trzecia, piąta linia, a druga, czwarta, szósta nie)
- [x] **(nieobowiązkowe)** Popraw powyższy program tak, aby i znaki `'\n'`, i dwubajtowe sekwencje złożone ze znaku `'\r'` i następującego po nim znaku `'\n'` były traktowane jako terminatory linii

## Dodatkowe pliki:
  - [Plik tekstowy do programu 3 zawierający dane](source.txt)
  - [Plik tworzony przez program do którego zapisywane są przekonwertowane dane](destination.txt)
  - [Makefile do tworzenia plików wykonywalnych](Makefile):
    - Wywołanie w konsoli:
        ```bash
        make
        ``` 