# Copyright (C) 2011 by D+D Griffiths
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
import urllib.request
import os
import re
import sys
import unicodedata
import getopt
from xml.dom import minidom

def usage():
    print("Usage:\npython rssgossip.py [-uh] <search-regexp>")

try:
    opts, args = getopt.getopt(sys.argv[1:], "uh", ["urls", "help"])
except getopt.GetoptError as err:
    print(str(err))
    usage()
    sys.exit(2)

include_urls = False
for o, a in opts:
    if o == "-u":
        include_urls = True
    elif o in ("-h", "--help"):
        usage()
        sys.exit()
    else:
        assert False, "unhandled option"

if len(args) == 0:
    usage()
    sys.exit(2)

searcher = re.compile(args[0], re.IGNORECASE)
feed_urls = os.environ.get('RSS_FEED', '').split()
for url in feed_urls:
    try:
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        with urllib.request.urlopen(req) as feed:
            dom = minidom.parse(feed)
            for node in dom.getElementsByTagName('title'):
                txt = node.firstChild.data if node.firstChild else ""
                if searcher.search(txt):
                    txt = unicodedata.normalize('NFKD', txt).encode('ascii', 'ignore').decode('ascii')
                    print(txt)
                    if include_urls:
                        p = node.parentNode
                        link_nodes = p.getElementsByTagName('link')
                        if link_nodes and link_nodes[0].firstChild:
                            link = link_nodes[0].firstChild.data
                            print("\t%s" % link)
    except Exception as err:
        print(f"Error processing {url}: {err}")
        sys.exit(1)
