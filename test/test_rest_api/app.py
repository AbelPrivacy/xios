from flask import Flask

app = Flask(__name__)


@app.route("/request/", methods=["GET"])
def get_request():
    return "GET /requests/"


@app.route("/request/", methods=["POST"])
def post_request():
    return "POST /requests/"


if __name__ == "__main__":
    app.run(host="127.0.0.1", port="8080")
