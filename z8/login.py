import requests

login_url = "http://sphinx.if.uj.edu.pl/techwww/httptest/login"
private_url = "http://sphinx.if.uj.edu.pl/techwww/httptest/private"
credentials = {"login" : "test", "password" : "0123", "submit" : "submit"}

if __name__ == "__main__":
    rpost = requests.post(login_url, data=credentials)
    print(rpost.content.decode('utf-8'), end="")
    print(f"Cookies = {rpost.cookies}")
    rget = requests.get(private_url, cookies=rpost.cookies)
    print(rget.content.decode('utf-8'), end="")