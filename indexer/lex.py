from bs4 import BeautifulSoup
import re

def tokenize_text(text):
  return re.findall(re.compile('\w+'), text)
  
def tokenize_html(html):
  soup = BeautifulSoup(html)
  words = tokenize_text(soup.get_text().lower())
  links = list()
  
  for link in soup.find_all('a'):
    links.append((link.text, link.get('href')))
    
  return words, links

if __name__ == '__main__':
  words, links = tokenize_html(open('../pages/2013-spring/index.html').read());
  for text, url in links:
    print(text)