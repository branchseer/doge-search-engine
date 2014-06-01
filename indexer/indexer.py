from docs import documents
from lex import tokenize_html, tokenize_text
from stopwords import stopwords, remove_stopwords
from stemming import stem

from os import path
import sys
sys.path.append('..')

from db.sqlite import engine, Session
from db.models import Base, Term, Document, TermFrequency

Base.metadata.create_all(engine)


def index(document_terms):
  session = Session()
  for url, terms in document_terms:
    doc = session.query(Document).filter(Document.url == url).first()
    if not doc:
      doc = Document(url = url)
      session.add(doc)
    
    for term_text in terms:
      term = session.query(Term).filter(Term.text == term_text).first()
      if not term:
        term = Term(text = term_text)
        session.add(term)
        
      frequency = session.query(TermFrequency).\
        filter(TermFrequency.document == doc, TermFrequency.term == term).first()
      if not frequency:
        frequency = TermFrequency(document = doc, term = term)
        session.add(frequency)
      else:
        frequency.frequency += 1
  
  session.flush()
  session.commit()

if __name__ == '__main__':
    stopword_list = stopwords()
    
    def document_terms():
      for filepath, content in documents():
        print(filepath)
        
        extension = path.splitext(filepath)[1]
        words = tokenize_html(content)[0] \
          if extension in ['.html', '.htm'] \
          else tokenize_text(content)
        
        words = remove_stopwords(words, stopword_list)
        
        words = (stem(word) for word in words)
        
        yield filepath, words
    
    index(document_terms())
        