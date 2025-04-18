from  sqlite4  import  SQLite4

db = SQLite4('./test_db.db')

def init_db():
    with current_app.open_resource("schema.sql") as f:
        db.executescript(f.read().decode("utf8"))
