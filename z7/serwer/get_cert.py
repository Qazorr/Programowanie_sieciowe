import ssl

hostname="localhost"
port = 2020

with open("plik_certyfikatu.crt", "w") as f:
    cert = ssl.get_server_certificate((hostname, port))
    f.write(cert)