from os import path, walk
import time
from datetime import datetime

def documents(page_folder = path.join('..', 'pages')):
  for dirpath, dirnames, filenames in walk(page_folder):
    for filename in filenames:
      extension = path.splitext(filename)[1]
      
      if extension in ['.html', '.htm', '.txt']:
        filepath = path.join(dirpath, filename)
        file = open(filepath, 'r', encoding='gbk')
        file_content = file.read()
        file.close()
        yield filepath[len(page_folder) + 1:], file_content, datetime.fromtimestamp(path.getmtime(filepath))

if __name__ == '__main__':
  for filepath, content, date in documents():
    print(filepath, '-', date, ':', content[:10])
