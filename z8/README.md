## Na dzisiejszych zajęciach zapoznamy się z protokołem HTTP.

Na dzisiejszych zajęciach zapoznamy się z protokołem HTTP.
  * Zapoznaj się z ogólną strukturą wiadomości HTTP:
    ```
    A message consists of header fields and, optionally a body. 
    The body is simply a sequence of lines containing ASCII characters. 
    It is separated from the headers by a null line (i.e., a line with nothing preceding the CRLF). 
    ```
    (Źródło: [RFC 822](https://tools.ietf.org/html/rfc822)) \
    oraz z oraz z bardziej szczegółową definicją ze specyfikacji HTTP ([RFC 2616](https://datatracker.ietf.org/doc/html/rfc2616#section-4), sekcje 4-6).

  * Uruchom Netcat-a w trybie TCP (komunikacja HTTP jest oparta na TCP) na pewnym porcie: