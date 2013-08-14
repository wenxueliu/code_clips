#! /usr/bin/env python
# -*- coding:utf-8 -*-

import string, re

charToSoundex = {
            "A":"9", 
            "B":"1",
            "C":"2",
            "D":"3",
            "E":"9",
            "F":"1",
            "G":"2",
            "H":"9",
            "I":"9",
            "J":"2",
            "K":"2",
            "L":"4",
            "M":"5",
            "N":"5",
            "O":"9",
            "P":"1",
            "Q":"2",
            "R":"6",
            "S":"2",
            "T":"3",
            "U":"9",
            "V":"1",
            "W":"9",
            "X":"2",
            "Y":"9",
            "Z":"2"}
        
def soundex(source):
    "convert string to Soundex equivalent"

    #====
    #allChars = string.uppercase + string.lowercase
    #if not re.compile("^[A-Za-z]+$").search(source):
    #== faster than regluar expression
    if (not source) and (not source.isalpha()):
        return "0000"
    #====

    #convert to digits and delete duplicate
    source = map(lambda s:s.upper(),source)
    digits = list(source[0])
    #[ digits.append(charToSoundex[s]) for s in source[1:] ]
    digits.extend( map(lambda s:charToSoundex[s],source[1:]) )

    #delete duplicate element
    #digist1 = {}.fromkeys(digist).keys() # not order preserving
    #digist1 = list(set(digist)) # not order preserving
    digits1 = sorted(set(digits), key=digits.index) #order preserving

    digits2 = filter(lambda s: s!='9', digits1)

    while len(digits2) < 4:
        digits2.append('0')
    return ''.join(digits2[:4])

if __name__ == "__main__":
    from timeit import Timer
    names = ('Wow', 'Pilgrim')
    print soundex('Wow')
    print soundex('Pilgrim')
    for name in names:
        print "next"
        statement = "soundex('%s')" %name
        t = Timer(statement, "from __main__ import soundex")
        print name.ljust(15), soundex(name), min(t.repeat())


    



