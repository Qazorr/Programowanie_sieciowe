#!/usr/bin/python3

import os

keys = [
    'GATEWAY_INTERFACE', 
    'SERVER_PROTOCOL', 
    'SERVER_PORT',
    'CONTENT_TYPE',
    'QUERY_STRING',
    'REQUEST_METHOD',
    'SCRIPT_NAME'
]

print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Hello from Python</title>")
print("</head>")
print("<body>")
print("<h2>Hello from Python!</h2>")
print("</body>")
print("<ul>")
for key in os.environ.keys():
    if key in keys:
        print("<li><span style='color:green'>%30s </span> : %s </li>" % (key,os.environ[key]))
print("</ul>")
print("</html>")