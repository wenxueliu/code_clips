# From OpenStack python-swiftclient
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
# implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Cloud Files client library used internally
"""

import socket
import sys
import logging
import warnings
from functools import wraps

from urllib import quote as _quote
from urlparse import urlparse, urlunparse
from httplib import HTTPException, HTTPConnection, HTTPSConnection
from time import sleep

from myexceptions import MYHTTPException, InvalidHeadersException

try:
    from https_connection import HTTPSConnectionNoSSLComp
except ImportError:
    HTTPSConnectionNoSSLComp = HTTPSConnection


def http_log(args, kwargs, resp, body):
    if not logger.isEnabledFor(logging.DEBUG):
        return

    string_parts = ['curl -i']
    for element in args:
        if element == 'HEAD':
            string_parts.append(' -I')
        elif element in ('GET', 'POST', 'PUT'):
            string_parts.append(' -X %s' % element)
        else:
            string_parts.append(' %s' % element)

    if 'headers' in kwargs:
        for element in kwargs['headers']:
            header = ' -H "%s: %s"' % (element, kwargs['headers'][element])
            string_parts.append(header)

    logger.debug("REQ: %s\n" % "".join(string_parts))
    if 'raw_body' in kwargs:
        logger.debug("REQ BODY (RAW): %s\n" % (kwargs['raw_body']))
    if 'body' in kwargs:
        logger.debug("REQ BODY: %s\n" % (kwargs['body']))

    logger.debug("RESP STATUS: %s\n", resp.status)
    if body:
        logger.debug("RESP BODY: %s\n", body)


def quote(value, safe='/'):
    """
    Patched version of urllib.quote that encodes utf8 strings before quoting
    """
    value = encode_utf8(value)
    if isinstance(value, str):
        return _quote(value, safe)
    else:
        return value


def validate_headers(headers):
    if headers:
        for key, value in headers.iteritems():
            if '\n' in value:
                raise InvalidHeadersException("%r header contained a "
                                              "newline" % key)
            if '\r' in value:
                raise InvalidHeadersException("%r header contained a "
                                              "carriage return" % key)


def encode_utf8(value):
    if isinstance(value, unicode):
        value = value.encode('utf8')
    return value


# look for a real json parser first
try:
    # simplejson is popular and pretty good
    from simplejson import loads as json_loads
except ImportError:
    # 2.6 will have a json module in the stdlib
    from json import loads as json_loads


def http_connection(url, proxy=None, ssl_compression=True):
    """
    Make an HTTPConnection or HTTPSConnection

    :param url: url to connect to
    :param proxy: proxy to connect through, if any; None by default; str of the
                  format 'http://127.0.0.1:8888' to set one
    :param ssl_compression: Whether to enable compression at the SSL layer.
                            If set to 'False' and the pyOpenSSL library is
                            present an attempt to disable SSL compression
                            will be made. This may provide a performance
                            increase for https upload/download operations.
    :returns: tuple of (parsed url, connection object)
    :raises MYHTTPException: Unable to handle protocol scheme
    """
    url = encode_utf8(url)
    parsed = urlparse(url)
    proxy_parsed = urlparse(proxy) if proxy else None
    host = proxy_parsed if proxy else parsed.netloc
    if parsed.scheme == 'http':
        conn = HTTPConnection(host)
    elif parsed.scheme == 'https':
        if ssl_compression is True:
            conn = HTTPSConnection(host)
        else:
            conn = HTTPSConnectionNoSSLComp(host)
    else:
        raise MYHTTPException('Cannot handle protocol scheme %s for url %s' %
                              (parsed.scheme, repr(url)))

    def putheader_wrapper(func):

        @wraps(func)
        def putheader_escaped(key, value):
            func(encode_utf8(key), encode_utf8(value))
        return putheader_escaped
    conn.putheader = putheader_wrapper(conn.putheader)

    def request_wrapper(func):

        @wraps(func)
        def request_escaped(method, url, body=None, headers=None):
            validate_headers(headers)
            url = encode_utf8(url)
            if body:
                body = encode_utf8(body)
            func(method, url, body=body, headers=headers or {})
        return request_escaped
    conn.request = request_wrapper(conn.request)
    if proxy:
        try:
            # python 2.6 method
            conn._set_tunnel(parsed.hostname, parsed.port)
        except AttributeError:
            # python 2.7 method
            conn.set_tunnel(parsed.hostname, parsed.port)
    return parsed, conn





def store_response(resp, response_dict):
    """
    store information about an operation into a dict

    :param resp: an http response object containing the response
                 headers
    :param response_dict: a dict into which are placed the
       status, reason and a dict of lower-cased headers
    """
    if response_dict is not None:
        resp_headers = {}
        for header, value in resp.getheaders():
            resp_headers[header.lower()] = value

        response_dict['status'] = resp.status
        response_dict['reason'] = resp.reason
        response_dict['headers'] = resp_headers


if __name__ == "__main__":
    url = "http://www.baidu.com" 
    (parsed, conn) = http_connection(url)
    print conn
    print parsed 
