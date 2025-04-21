from sqlalchemy import BLOB, Column, DateTime, String, create_engine
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import Session

engine = create_engine("sqlite:///test_db.db")

from datetime import datetime
from uuid import uuid4

Base = declarative_base()


class request(Base):
    __tablename__ = "request"

    request_id = Column(BLOB, primary_key=True)
    http_method = Column(String)
    timestamp = Column(DateTime)

    def __init__(self, http_method):
        self.request_id = uuid4().bytes
        self.http_method = http_method
        self.timestamp = datetime.now()
        super().__init__()

    def __repr__(self):
        return f"request: {self.request_id.hex()}, {self.http_method}, {self.timestamp}"

    def __str__(self):
        return f"request: {self.request_id.hex()}, {self.http_method}, {self.timestamp}"


Base.metadata.drop_all(engine)
Base.metadata.create_all(engine)

with Session(engine) as session:
    session.begin()
    session.add(request("DEMO-GET"))
    session.add(request("DEMO-POST"))
    session.add(request("DEMO-GET"))
    session.commit()

# with Session(engine) as session:
#     from sqlalchemy import select
#     stmt = select(request)

#     for request in session.scalars(stmt):
#         print(request)
