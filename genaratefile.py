def generate_big_random_letters(filename,size,numgb):
    """
    generate big random letters/alphabets to a file
    :param filename: the filename
    :param size: the size in bytes
    :return: void
    """
    import random
    import string

    #chars = ''.join([random.choice(string.letters) for i in range(size)]) #1i


    with open(filename, 'w') as f:
        for j in range(numgb):
            print "j = ", j
            for i in range(size):
                if(i%(1024*1024) == 0):
                    print i
                chars = ''.join([random.choice(string.letters) for i in range(random.randrange(1,500))])
                f.write(chars)
                if i != size-1:
                    f.write("\n")
    pass


def readfile():
    filepath = 'out.txt'
    with open(filepath) as fp:
       line = fp.readline()
       cnt = 1
       while line:
          print("Line {}: {}".format(cnt, line.strip()))
          line = fp.readline()
          cnt += 1
    

if __name__ == '__main__':
    a = 1024*1024
   
    print a
    generate_big_random_letters("in.txt",a,1)
    #readfile()
