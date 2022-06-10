#!/usr/bin/python3
# -*- coding: UTF-8 -*-

'''
Aplikacja WSGI implementujÄ…ca najwaÅ¼niejsze czÄ™Å›ci opisywanej na wykÅ‚adzie
usÅ‚ugi REST dajÄ…cej dostÄ™p do bazy z danymi osÃ³b.

Uwaga: kod dydaktyczny bez peÅ‚nej obsÅ‚ugi bÅ‚Ä™dÃ³w i sytuacji nadzwyczajnych.

Aplikacja nie potrafi sama stworzyÄ‡ swojej bazy danych, trzeba to zrobiÄ‡
przed jej uruchomieniem. Skrypt rest_webapp.sh pokazuje jak.
'''

plik_bazy = './osoby.sqlite'

import re, sqlite3

class OsobyApp:
    def __init__(self, environment, start_response):
        '''
Konstruktor wywoÅ‚ywany przez serwer WSGI. Jak kaÅ¼dy konstruktor tworzy nowy
obiekt, nastÄ™pnie zapamiÄ™tuje w jego polach przekazane przez serwer argumenty
i inicjuje pola na odpowiedÅº.
'''
        self.env = environment
        self.start_response = start_response
        self.status = '200 OK'
        self.headers = [ ('Content-Type', 'text/html; charset=UTF-8') ]
        self.content = b''

    def __iter__(self):
        '''
Metoda obsÅ‚ugujÄ…ca proces iterowania po stworzonym obiekcie. Serwer WSGI
wymaga aby w Å›rodku byÅ‚a co najmniej jedna instrukcja "yield" zwracajÄ…ca
ciÄ…g bajtÃ³w do odesÅ‚ania klientowi HTTP.
'''
        try:
            self.route()
        except sqlite3.Error as e:
            s = 'SQLite error: ' + str(e)
            self.failure('500 Internal Server Error', s)
        n = len(self.content)
        self.headers.append( ('Content-Length', str(n)) )
        self.start_response(self.status, self.headers)
        yield self.content

    def failure(self, status, detail = None):
        '''
Metoda wstawiajÄ…ca do pÃ³l obiektu status bÅ‚Ä™du oraz dokument HTML
z komunikatem o jego wystÄ…pieniu.
'''
        self.status = status
        s = '<html>\n<head>\n<title>' + status + '</title>\n</head>\n'
        s += '<body>\n<h1>' + status + '</h1>\n'
        if detail is not None:
            s += '<p>' + detail + '</p>\n'
        s += '</body>\n</html>\n'
        self.content = s.encode('UTF-8')

    def route(self):
        '''
PierwszÄ… rzeczÄ…, ktÃ³rÄ… aplikacja musi zrobiÄ‡ po odebraniu zapytania, jest
sprawdzenie nazwy metody HTTP oraz nazwy zasobu. Jest to konieczne aby siÄ™
zorientowaÄ‡ o co klient prosi i wywoÅ‚aÄ‡ odpowiedni fragment kodu realizujÄ…cy
jego zlecenie. Jest to tzw. routing zapytania.

W niniejszej aplikacji routing jest realizowany czÄ™Å›ciowo w tej metodzie,
a czÄ™Å›ciowo w metodach handle_table() i handle_item().
'''
        if self.env['PATH_INFO'] == '/osoby':
            self.handle_table()
            return
        m = re.search('^/osoby/(?P<id>[0-9]+)$', self.env['PATH_INFO'])
        if m is not None:
            self.handle_item(m.group('id'))
            return
        self.failure('404 Not Found')

    def handle_table(self):
        '''
ObsÅ‚uga zapytaÅ„ odnoszÄ…cych siÄ™ do tabeli "osoby" traktowanej jako caÅ‚oÅ›Ä‡.
MoÅ¼na jÄ… pobraÄ‡, albo moÅ¼na dodaÄ‡ do niej nowy wiersz.
'''
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select()
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'POST':
            colnames, vals = self.read_tsv()
            q = 'INSERT INTO osoby (' + ', '.join(colnames) + ') VALUES ('
            q += ', '.join(['?' for v in vals]) + ')'
            id = self.sql_modify(q, vals)
            colnames, rows = self.sql_select(id)
            self.send_rows(colnames, rows)
        else:
            self.failure('501 Not Implemented')

    def handle_item(self, id):
        '''
ObsÅ‚uga zapytaÅ„ odnoszÄ…cych siÄ™ do konkretnego wiersza w tabeli "osoby".
MoÅ¼na go pobraÄ‡, zmodyfikowaÄ‡, albo usunÄ…Ä‡.
'''
        if self.env['REQUEST_METHOD'] == 'GET':
            colnames, rows = self.sql_select(id)
            if len(rows) == 0:
                self.failure('404 Not Found')
            else:
                self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'PUT':
            colnames, vals = self.read_tsv()
            q = 'UPDATE osoby SET '
            q += ', '.join([c + ' = ?' for c in colnames])
            q += ' WHERE id = ' + str(id)
            self.sql_modify(q, vals)
            colnames, rows = self.sql_select(id)
            self.send_rows(colnames, rows)
        elif self.env['REQUEST_METHOD'] == 'DELETE':
            q = 'DELETE FROM osoby WHERE id = ' + str(id)
            self.sql_modify(q)
        else:
            self.failure('501 Not Implemented')

    def read_tsv(self):
        f = self.env['wsgi.input']
        n = int(self.env['CONTENT_LENGTH'])
        raw_bytes = f.read(n)
        lines = raw_bytes.decode('UTF-8').splitlines()
        colnames = lines[0].split('\t')
        vals = lines[1].split('\t')
        return colnames, vals

    def send_rows(self, colnames, rows):
        s = '\t'.join(colnames) + '\n'
        for row in rows:
            s += '\t'.join([str(val) for val in row]) + '\n'
        self.content = s.encode('UTF-8')
        self.headers = [ ('Content-Type',
                'text/tab-separated-values; charset=UTF-8') ]

    def sql_select(self, id = None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        query = 'SELECT * FROM osoby'
        if id is not None:
            query += ' WHERE id = ' + str(id)
        crsr.execute(query)
        colnames = [ d[0] for d in crsr.description ]
        rows = crsr.fetchall()
        crsr.close()
        conn.close()
        return colnames, rows

    def sql_modify(self, query, params = None):
        conn = sqlite3.connect(plik_bazy)
        crsr = conn.cursor()
        if params is None:
            crsr.execute(query)
        else:
            crsr.execute(query, params)
        rowid = crsr.lastrowid   # id wiersza wstawionego przez INSERT
        crsr.close()
        conn.commit()
        conn.close()
        return rowid

if __name__ == '__main__':
    from wsgiref.simple_server import make_server
    port = 8000
    httpd = make_server('', port, OsobyApp)
    print('Listening on port %i, press ^C to stop.' % port)
    httpd.serve_forever()
