#!/usr/bin/env python
# encoding: utf-8


import sys
import argparse
import json
import httplib
import urllib2


class RestApi(object):

    def __init__(self, server, port):
        self.server = server
        self.port = port

    def get(self, path):
        #ret = self.rest_call(path, {}, 'GET')
        #return ret[2]
        f = urllib2.urlopen('http://'+self.server+':'+str(self.port)+path)
        ret = f.read()
        return json.loads(ret)

    def set(self, path, data):
        ret = self.rest_call(path, data, 'POST')
        return ret[0] == 200

    def remove(self, objtype, data):
        #ret = self.rest_call(data, 'DELETE')
        return ret[0] == 200

    def rest_call(self, path, data, action):
        headers = {
            'Content-type': 'application/json',
            'Accept': 'application/json',
            }
        body = json.dumps(data)
        conn = httplib.HTTPConnection(self.server, self.port)
        conn.request(action, path, body, headers)
        response = conn.getresponse()
        ret = (response.status, response.reason, response.read())
        conn.close()
        print str(ret[2])
        return ret

    def GET(self, path):
        ret = self.get(path)
        return ret

    def POST(self, path, data):
        return self.rest_call(path, data, "POST")

    def PUT(self, path, data):
        return self.rest_call(path, data, "PUT")

    def DELETE(self, path):
        data = {}
        return self.rest_call(path, data, "DELETE")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='process args')
    parser.add_argument('--ip', default='localhost', help="--ip 192.168.1.10")
    parser.add_argument('--port', default=8080, help="--port 8080")
    args = parser.parse_args(sys.argv[1:])

    #......
