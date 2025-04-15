#import sys

#sys.path.insert(0, '/Users/kylemcgrath/Documents/GitHub/nginx-with-wolfssl/venv/lib/python3.13/site-packages/')

from flask import Flask
from db import db




app = Flask(__name__)


@app.route("/request/", methods=['GET'])
def get_request():
    return "GET /requests/"

@app.route("/request/", methods=['POST'])
def post_request():
    return "POST /requests/"

app.run(host="127.0.0.1", port="8080", ssl_context=("../certs/cert.pem", "../certs/key.pem"))