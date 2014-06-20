#! /usr/bin/env python
# -*- coding: -*-

import re

def s1_plural(noun):
    """
        the first stage
    """
    if re.search('[sxz]$', noun):
        return re.sub('$', 'es', noun)
    elif re.search('[^aeioudgkprt]h$', noun):
        return re.sub('$', 'es', noun)
    elif re.search('[^aeiou]y$', noun):
        return re.sub('y$', 'ies', noun)
    else:
        return noun + 's'

'''
the second stage
'''
def match_sxz(noun):
    return re.search('[sxz]$', noun)
def apply_sxz(noun):
    return re.sub('[sxz]$', 'es', noun)

def match_h(noun):
    return re.search('[^aeioudgkprt]h$', 'es', noun)
def apply_h(noun):
    return re.sub('$', 'es', noun)

def match_y(noun):
    return re.search('[^aeiou]y', noun)
def apply_y(noun):
    return re.sub('y$', 'ies', noun)

def match_default(noun):
    return 1
def apply_default(noun):
    return noun + 's'

rules = ((match_sxz, apply_sxz),
         (match_h, apply_h),
         (match_y, apply_y),
         (match_default, apply_default))

def plural(noun):
    for matchesRule, applyRule in rules:
        if matchesRule(noun):
            return applyRule(noun)

'''
the third stage
'''

rules = \
        (
         (
          lambda word: re.search('[sxz]$',  word)
          lambda word: re.sub('[sxz]$', 'es', word)
         ),
         (
          lambda word: re.search('[^aeioudgkprt]h$', 'es', noun)
          lambda word: re.sub('$', 'es', word)
         ),
         (
          lambda word: re.search('[^aeiou]y', noun)
          lambda word: re.sub('y$', 'ies', word)
         ),
         (
          lambda word: re.search('$', word)
          lambda word: re.search('$', 's', word)
         ),
        )


def plural(noun):
    for matchesRule, applyRule in rules:
        if matchesRule(noun):
            return applyRule(noun)

'''
the fourth stage
'''

def buildMatchAndApplyFunction((pattern, search, replace))
    matchFunction = lambda word: re.search(pattern, word)
    applyFunction = lambda word: re.sub(search, replace, word)
    return (matchFunction, applyFunction)

patterns = \
           (
            ('[sxz]$', '$', 'es'),
            ('[^aeioudgkprt]h$', '$', 'es'),
            ('(qu|[^aeiou])y', 'y$', 'ies'),
            ('$', '$', 's')
           )

rules = map(buildMatchAndApplyFunction, patterns)
    

'''
the fifth stage
'''
'''
data.txt
[sxz]$              $       es
[^aeioudgkprt]h$    $       es
[^aeiou])y          y$      ies
$                   $       s
'''

def buildRule((pattern, search, replace)):
    return lambda word: re.search(pattern, word) and \
        re.sub(search, replace, word)

def plural(noun, language='en'):
    lines = file('rules.%s' % language).readlines()
    patterns = map(string.split, lines)
    rules = map(buildRule, patterns)
    for rule in rules:
        result = rule(noun)
        if result: return result


def rules(language):
    for line in file('rules.%s' %language):
        pattern, search, replace = line.split()
        yield lambda word: re.search(pattern, word) and \
                        re.sub(search, replace, word)
        
def plural(noun, language='en'):
    for applyRule in rules(language):
        result = applyRule(noun)
        if result: return result
