import sys
sys.path.append('..')

from db.sqlite import Session
from db.models import Term, Document, TermFrequency
from indexer.stemming import stem
from indexer.lex import tokenize_text


def search(query):
  session = Session()
  try:
    keywords = tokenize_text(query)
    terms = ((keyword, session.query(Term).filter(Term.text == stem(keyword)).first()) for keyword in keywords)
    terms = [term for term in terms if term[1] is not None]
    
    
    if len(terms) == 0:
      yield from ()
    else:
      documents = {}
      for keyword, term in terms:
        for freq in term.frequencies:
          termTuple = (keyword, freq.frequency)
          if freq.document.url not in documents:
            documents[freq.document.url] = (freq.document, [termTuple])
          else:
            documents[freq.document.url][1].append(termTuple)
      yield from sorted(documents.values(), key=lambda doc: len(doc[1]), reverse=True)
  
  finally:
    session.flush()
    session.commit()
  
  
if __name__ == '__main__':
  results = search('computer')
  print(list(results))
