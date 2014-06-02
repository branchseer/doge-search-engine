import sys
sys.path.append('..')

from db.sqlite import Session
from db.models import Term, Document, TermFrequency
from indexer.stemming import stem


def search(query):
  session = Session()
  try:
    query = stem(query)
    term = session.query(Term).filter(Term.text == query).first()
    if not term:
      yield from ()
    else:
      for freq in term.frequencies:
        yield (freq.document, freq.frequency)
  finally:
    session.flush()
    session.commit()
  
  
if __name__ == '__main__':
  results = search('computer')
  print(list(results))