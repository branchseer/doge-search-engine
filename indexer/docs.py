import sys
from os import path, walk

page_folder = path.join('..', 'pages')

def documents():
  for dirpath, dirnames, filenames in walk(page_folder):
    for filename in filenames:
      extension = path.splitext(filename)[1]
      
      if extension == '.html' or extension == '.txt':
        filepath = path.join(dirpath, filename)
        file = open(filepath)
        file_content = file.read()
        file.close()
        yield filepath[len(page_folder) + 1:], file_content

if __name__ == '__main__':
  for filepath, content in documents():
    print(filepath, ':', content[:10])
