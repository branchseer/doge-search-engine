from flask import Flask, request, render_template

import sys
sys.path.append('..')
from db.sqlite import engine
from db.models import Base
Base.metadata.create_all(engine)

app = Flask(__name__, static_url_path='', template_folder='templates')
app.debug = True

@app.route('/')
def root():
  return app.send_static_file('index.html')

from search import search
@app.route('/search')
def handle_search():
  query = request.args.get('q')
  results = search(query)
  return render_template("search.html", query=query, results=list(results))

if __name__ == "__main__":
    app.run(host='0.0.0.0')
