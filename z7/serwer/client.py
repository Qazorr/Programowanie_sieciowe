import requests

host = "http://127.0.0.1:2020"
certificate = "certs/plik_certyfikatu.crt"

r = requests.get(host, cert=certificate)
print(r.content)