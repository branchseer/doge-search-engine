from os import path, makedirs
import requests
from bs4 import BeautifulSoup
from urllib.parse import urljoin

def crawl(url, depth=0, urlfilter=lambda url: True, maxDepth=float("infinity"), linkset=set(), session=requests.Session()):
  content = None
  try:
    content = requests.get(url).text
  except Exception:
    yield from ()
  else:
    soup = BeautifulSoup(content)
    links = (urljoin(url, link.get('href')) for link in soup.find_all('a'))
    yield url, content, links

    links = set(link for link in links if link != url and link not in linkset)
    if depth <= maxDepth:
      linkset |= links
      for link_url in links:
        if urlfilter(link_url):
          yield from crawl(link_url, depth+1, filter, maxDepth, linkset)

if __name__ == '__main__':
    def filter(url):
      return url.startswith('http://www.seu.edu.cn/s')
    
    base_path = 'pages'
    crawler_session = requests.Session()
    crawler_session.headers['User-Agent'] = 'doge-crawler'
    for url, content, links in crawl('http://www.seu.edu.cn/s/132/main.jspy', urlfilter=filter, session=crawler_session):
      print(url)
      filepath = path.join(base_path, url[url.find('//') + 2:])
      filedir = path.dirname(filepath)
    
      if not path.exists(filedir):
        makedirs(filedir)
      with open(filepath, 'w') as file:
        file.write(content)
      