GET_sphinx:
	curl http://sphinx.if.uj.edu.pl/techwww/httptest/test
POST_sphinx:
	curl -X POST http://sphinx.if.uj.edu.pl/techwww/httptest/test -H "Content-Type: application/json"

curl_retrieve:
	@echo "Logging to server"
	curl -d "login=test&password=0123" -X POST http://sphinx.if.uj.edu.pl/techwww/httptest/login -c cookies.txt
	@echo 
	@echo "Getting /private data"
	curl -X GET http://sphinx.if.uj.edu.pl/techwww/httptest/private -b cookies.txt