from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import Column, Integer, String, MetaData, ForeignKey, DateTime
from sqlalchemy.orm import relationship

Base = declarative_base()

class Term(Base):
    __tablename__ = 'terms'
    id = Column(Integer, primary_key=True)
    text =  Column(String(50))
    frequencies = relationship("TermFrequency", backref="term")
    
class Document(Base):
  __tablename__ = 'documents'
  id = Column(Integer, primary_key=True)
  url = Column(String(500))
  terms = relationship("TermFrequency", backref="document")
  title = Column(String(200))
  date = Column(DateTime())
  
class TermFrequency(Base):
  __tablename__ = 'term_document_frequencies'
  id = Column(Integer, primary_key=True)
  term_id = Column(Integer, ForeignKey('terms.id'))
  document_id = Column(Integer, ForeignKey('documents.id'))
  frequency = Column(Integer, default=1)
  
      
if __name__ == '__main__':
  from sqlite import engine, Session
  Base.metadata.create_all(engine)
  session = Session()
  