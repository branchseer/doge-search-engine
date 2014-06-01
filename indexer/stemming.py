from Stemmer import Stemmer
stemmer = Stemmer('english')
  
def stem(word):
  return stemmer.stemWord(word)

def stemWords(words):
  return stemmer.stemWords(words)

if __name__ == '__main__':
    print(stem('expecting'))
