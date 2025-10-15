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
