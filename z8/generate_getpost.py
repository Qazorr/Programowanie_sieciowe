import requests

url = "http://sphinx.if.uj.edu.pl/techwww/httptest/test"
header = {'Content-Type': 'application/json'}

if __name__ == "__main__":
    print("Generating GET request")
    r = requests.get(url)
    print(r.content)
    print(r.content.decode('utf-8'))

    print("Generating POST request")
    r = requests.post(url, headers=header)
    print(r.content)
    print(r.content.decode('utf-8'))