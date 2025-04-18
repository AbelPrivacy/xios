import sys

sys.path.insert(0, '../../venv/')

from flask import Flask
import ssl
from db import db




app = Flask(__name__)


@app.route("/request/", methods=['GET'])
def get_request():
    return "GET /requests/"

@app.route("/request/", methods=['POST'])
def post_request():
    return "POST /requests/"

ssl_context = ssl.create_default_context(ssl.Purpose.CLIENT_AUTH)
ssl_context.load_cert_chain(certfile="certs/cert.pem", keyfile="certs/key.pem")
# ssl_context.set_ciphers("TLS_AES_256_GCM_SHA384")

app.run(host="127.0.0.1", port="8080", ssl_context=("../certs/cert.pem", "../certs/key.pem"))