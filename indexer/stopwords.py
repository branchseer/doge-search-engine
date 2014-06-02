
def stopwords(stopwords_file='stopwords.txt', delimiter=' '):
  file = open(stopwords_file,'r',encoding= 'utf-8')
  file_content = file.read()
  stopwords_list = file_content.split(delimiter)
  return stopwords_list

def remove_stopwords(words, stopwords):
  return (word for word in words if word not in stopwords)
  
if __name__ == '__main__':
  print(list(remove_stopwords(['a','b','c','d','e'],stopwords())))