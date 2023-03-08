import requests
import json
from bs4 import BeautifulSoup

url = 'https://www.discogs.com/artist/359282-Budka-Suflera'
headers = {
    'User-Agent': 'LearningDiscogs',
    'Authorization': 'Discogs token='
}

budka_suflera = {
    'artist': 'https://api.discogs.com/artists/359282',
    'releases': 'https://api.discogs.com/artists/359282/releases',
}

artist = input("Jaki zespol?: ")

search = 'https://api.discogs.com/database/search'

r = requests.get(search+'?q='+artist, headers=headers)

artist = json.loads(r.text)
print(json.dumps(artist, indent=4))

# r = requests.get(budka_suflera['releases'], headers=headers)
# data = json.loads(r.text)
# pagination = data['pagination']
# releases = data['releases']

# for _ in range(pagination['pages']):
        
#     for release in releases:
#         print(release['title'])
#     print('-----------', data['pagination']['page'], '-----------')
#     if pagination['page'] != pagination['pages']:
#         r = requests.get(data['pagination']['urls']['next'], headers=headers)
#         data = json.loads(r.text)
#         releases = data['releases']
#         pagination = data['pagination']

# print(data['pagination']['page'])
