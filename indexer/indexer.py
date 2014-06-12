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


def filename(filepath):
  return path.splitext(path.basename(filepath))[0]

def index(document_terms):
  session = Session()
  for title, url, terms, date in document_terms:
    doc = session.query(Document).filter(Document.url == url).first()
    if not doc:
      doc = Document(title = title, url = url, date = date)
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
      for filepath, content, date in documents():
        print(filepath)
        
        extension = path.splitext(filepath)[1]
        
        words = None
        title = filename(filepath)
        
        if extension in ['.html', '.htm', '.jspy']:
          html_title, words, links = tokenize_html(content)
          html_title = html_title.strip()
          if html_title:
            title = html_title
        else:
          words = tokenize_text(content)
        
        
        words = remove_stopwords(words, stopword_list)
        
        words = (stem(word) for word in words)
        
        yield title, filepath, words, date
    
    index(document_terms())
        