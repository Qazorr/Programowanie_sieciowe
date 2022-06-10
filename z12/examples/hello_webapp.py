#!/usr/bin/python3
# -*- coding: UTF-8 -*-

'''
PrzykÅ‚ad banalnej aplikacji WWW zgodnej ze specyfikacjÄ… WSGI (Python Web
Server Gateway Interface).

Oryginalna wersja specyfikacji, przeznaczona dla Pythona 2, jest dostÄ™pna
pod <https://www.python.org/dev/peps/pep-0333>. Wersja uwzglÄ™dniajÄ…ca zmiany
wprowadzone w Pythonie 3 jest pod <https://www.python.org/dev/peps/pep-3333>.

NajwaÅ¼niejsze informacje:
- punktem wejÅ›cia do aplikacji WSGI moÅ¼e byÄ‡ funkcja albo klasa
- serwer HTTP wywoÅ‚uje funkcjÄ™ (albo konstruktor) przekazujÄ…c jako pierwszy
  argument sÅ‚ownik ze Å›rodowiskiem WSGI, a w nim m.in. nagÅ‚Ã³wki zapytania
- jeÅ›li zapytanie HTTP miaÅ‚o nie tylko nagÅ‚Ã³wki, ale rÃ³wnieÅ¼ tzw. ciaÅ‚o, czyli
  treÅ›Ä‡ o dÅ‚ugoÅ›ci podanej w nagÅ‚Ã³wku Content-Length, to bÄ™dzie ona dostÄ™pna
  w strumieniu environment['wsgi.input']
- drugim argumentem jest referencja do funkcji pozwalajÄ…cej wyspecyfikowaÄ‡
  status oraz nagÅ‚Ã³wki generowanej odpowiedzi HTTP
- funkcja-aplikacja zwraca wÅ‚aÅ›ciwÄ… treÅ›Ä‡ odpowiedzi jako swÃ³j wynik; musi to
  byÄ‡ obiekt pozwalajÄ…cy siÄ™ iterowaÄ‡ (a wiÄ™c np. tablica), iterowane elementy
  sÄ… jeden za drugim odsyÅ‚ane klientowi HTTP

Informacje specyficzne dla Pythona 3:
- Å‚aÅ„cuchy Pythona sÄ… unikodowe, podobnie jak w Javie
- wg specyfikacji HTTP nagÅ‚Ã³wki zapytaÅ„ i odpowiedzi sÄ… ciÄ…gami bajtÃ³w
  w kodowaniu ISO-8859-1, moÅ¼na wiÄ™c je jednoznacznie skonwertowaÄ‡ do/z
  postaci unikodowych Å‚aÅ„cuchÃ³w, i dla wygody jest to automatycznie robione
- treÅ›Ä‡ po nagÅ‚Ã³wkach moÅ¼e byÄ‡ tekstem w dowolnym kodowaniu albo wrÄ™cz danymi
  binarnymi, programista musi je przetwarzaÄ‡ jako ciÄ…gi bajtÃ³w
- typ Pythona o nazwie bytes reprezentuje ciÄ…gi bajtÃ³w
- jeÅ›li w zmiennej s jest Å‚aÅ„cuch, to moÅ¼na go zamieniÄ‡ na bajty albo przez
  rzutowanie bytes(s, 'UTF-8'), albo przez wywoÅ‚anie s.encode('UTF-8')
- bajty b konwertujemy na Å‚aÅ„cuch przez str(b, 'UTF-8') lub b.decode('UTF-8')
- specjalna notacja b'znaki ASCII' pozwala wyspecyfikowaÄ‡ staÅ‚Ä… typu bytes

W poniÅ¼szym kodzie uÅ¼ywana jest funkcja repr. Jako argument moÅ¼na jej podaÄ‡
dowolnÄ… wartoÅ›Ä‡ lub obiekt; zwraca tekstowÄ… reprezentacjÄ™ argumentu.
'''

def hello_app(environment, start_response):
    headers = [
        ('Content-Type', 'text/plain; charset=UTF-8'),
        ('Server', 'HelloApp/1.0'),
    ]
    start_response('200 OK', headers);

    envdump = 'Åšrodowisko WSGI:\n{\n'
    for k in sorted(environment.keys()):
        envdump += repr(k) + ': ' + repr(environment[k]) + ',\n'
    envdump += '}\n'

    return [ b'Hello, world!\n\n', envdump.encode('UTF-8') ]

# PoniÅ¼szy fragment pozwala uruchomiÄ‡ zdefiniowanÄ… powyÅ¼ej aplikacjÄ™ wewnÄ…trz
# deweloperskiego serwera HTTP/WSGI po wydaniu z linii poleceÅ„ komendy
#     python3 hello_webapp.py
# Aby sprawdziÄ‡ czy aplikacja dziaÅ‚a otwÃ³rz w przeglÄ…darce adres
#     http://localhost:8000/
# MoÅ¼esz teÅ¼ zamiast przeglÄ…darki uÅ¼yÄ‡ innego narzÄ™dzia, wydaj np. polecenie
#     curl -v http://localhost:8000/
#
# JeÅ›li nie chcemy aby Å›rodowisko linuksowej powÅ‚oki "przeciekaÅ‚o" do
# Å›rodowiska WSGI moÅ¼na uruchomiÄ‡ serwer za poÅ›rednictwem programu env,
# ktÃ³ry potrafi je wyzerowaÄ‡:
#     env -i python3 hello_webapp.py
#
if __name__ == '__main__':
    from wsgiref.simple_server import make_server
    port = 8000
    httpd = make_server('', port, hello_app)
    print('Listening on port %i, press ^C to stop.' % port)
    httpd.serve_forever()
