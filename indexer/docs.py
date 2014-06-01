from os import path, walk


def documents(page_folder = path.join('..', 'pages')):
  for dirpath, dirnames, filenames in walk(page_folder):
    for filename in filenames:
      extension = path.splitext(filename)[1]
      
      if extension in ['.html', '.htm', '.txt']:
        filepath = path.join(dirpath, filename)
        file = open(filepath)
        file_content = file.read()
        file.close()
        yield filepath[len(page_folder) + 1:], file_content

if __name__ == '__main__':
  for filepath, content in documents():
    print(filepath, ':', content[:10])
