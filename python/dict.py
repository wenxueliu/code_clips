# -*- coding: utf-8 -*-
#!/usr/bin/env python
#----------------------------------------------------------------
# Introduction: youdao dictionary
import re
import time
import thread
import urllib
import urllib2
from urllib import quote


class SpiderYoudao:
    def __init__(self):
        self.run = True

    def get_word(self):
        word = raw_input("输入单词: ")
        return word

    def search(self, word, dict):
        youdao_url ="http://dict.youdao.com/search?len=eng&q="+quote(word)+"&keyfrom=dict.top"
        youdao_page = self.get_page(youdao_url)

        items_match = '<div.*?class="trans-container">(.*?)<div id="webTrans" class="trans-wrapper trans-tab">'
        trans_items = re.findall(items_match, youdao_page, re.S)

        item_match = re.compile(r's?<li>.*?</li>s?')
        word_match = re.compile('s?<.*?>')

        for d in dict:
            if d == "youdao":
                print "\n有道词典:"
                for item in trans_items:
                    trans_iter = item_match.finditer(item)
                    for iter in trans_iter:
                        print word_match.sub("", iter.group())
            print "\n"
            if d == "21st":
                print "\n21世纪大辞典:"
                d21st_match = re.compile(r's?<span.*?class="def">.*?</span>')
                d21st_iter = d21st_match.finditer(youdao_page)
                for iter in d21st_iter:
                    print word_match.sub("", iter.group())
            print "\n"


    def get_page(self, url):
        user_agent = 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:32.0) Gecko/20100101 Firefox/32.0'
        headers = { 'User-Agent' : user_agent }
        req = urllib2.Request(url, headers = headers)
        resp = urllib2.urlopen(req)
        page = resp.read().decode("utf-8")
        return page


    def start(self):
        while self.run:
            choice = raw_input('...任意键继续("q"退出):')
            if choice !="q":
                dict = ["youdao", "21st"]
                self.search(self.get_word(), dict)
            else:
                self.run = False



if __name__ == '__main__':
    mydict = SpiderYoudao()
    mydict.start()
