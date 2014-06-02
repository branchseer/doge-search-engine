from flask import Flask, request, render_template
app = Flask(__name__, static_url_path='', template_folder='templates')
app.debug = True

@app.route('/')
def root():
  return app.send_static_file('index.html')
  
@app.route('/search')
def search():
  query = request.args.get('q')
  return render_template("search.html", query=query)

if __name__ == "__main__":
    app.run(host='0.0.0.0')
