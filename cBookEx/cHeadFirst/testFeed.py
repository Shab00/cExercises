import feedparser

url = "https://cointelegraph.com/rss/tag/bitcoin"
feed = feedparser.parse(url)

print("Number of entries:", len(feed.entries))
for entry in feed.entries:
    print(entry.title)
