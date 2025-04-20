from sqlalchemy import create_engine

engine = create_engine(
    "sqlite:///test_db.db"
)

from sqlalchemy import Integer, Column, String, DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import Session
from datetime import datetime

Base = declarative_base()

class request(Base):
    __tablename__ = "request"

    def __init__(self, request_id, http_method):
        self.request_id = request_id
        self.http_method = http_method
        self.timestamp = datetime.now()
        super().__init__()

    def __repr__(self):
        return f"request: {self.request_id}, {self.http_method}, {self.timestamp}"

    id = Column(Integer, primary_key=True)
    http_method = Column(String)
    timestamp = Column(DateTime)

Base.metadata.create_all(engine)

with Session(engine) as session:
    session.add(request(0, "GET"))
    session.add(request(1, "POST"))
    session.add(request(2, "GET"))
    session.commit()

# with Session(engine) as session:
#     from sqlalchemy import select
#     stmt = select(request)

#     for request in session.scalars(stmt):
#         print(request)
