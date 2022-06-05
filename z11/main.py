import argparse
import sys
from time import sleep as sleep
import requests
import json

artists_url = 'https://api.discogs.com/artists/'
search_url = 'https://api.discogs.com/database/search'

headers = {
    'User-Agent': 'LearningDiscogsAPI',
    'Authorization': ''
}

request_limit = 60
time_to_wait = 5
timeout = 5

# colors
class c:
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    CYAN = '\033[34m'
    MAGENTA = '\033[35m'
    BOLD = '\033[1m'
    BLINK = '\033[5m'
    RESET = '\033[0m'

def safe_get(url, try_no=1, tries=3):
    try:
        r = requests.get(url, headers=headers, timeout=timeout)
        r.raise_for_status()
        global request_limit
        request_limit = r.headers['X-Discogs-Ratelimit-Remaining']
        return r
    except requests.exceptions.HTTPError as e:
        if '429' in str(e):
            print(f'{c.BLINK}Request limit exceeded. {try_no} wait procedure started...{c.RESET}')
            sleep(time_to_wait)
            print(f'\rWait procedure ended.')
            if try_no != tries:
                try_no += 1
                return safe_get(url, try_no)
            else:
                print(f'There were no requests left, please try again later...')
                sys.exit(0)
        else:
            raise SystemExit(e)
    except requests.ConnectionError as e:
        print("Please check your internet connection.")
        sys.exit(0)
    except Exception as e:
        print(f"Something went wrong... error={e}")
        sys.exit(0)

def find_id_from_name(name):
    r = safe_get(search_url+'?q='+name+'&type=artist')
    artist = json.loads(r.text)
    if len(artist['results']) > 0:
        artist_id = artist['results'][0]['id']
        artist_name = artist['results'][0]['title']
        return artist_id, artist_name
    else:
        print(f'No band named {c.MAGENTA}{c.BOLD}{name}{c.RESET} found')
        sys.exit()

def get_member_info(member, info):
    member_url = member['resource_url']
    member_request = safe_get(member_url)
    groups = json.loads(member_request.text)['groups']
    for group in groups:
        if group['id'] not in info:
            info[group['id']] = [group['name']]
        info[group['id']].append('='.join([str(member['id']), member['name']]))

def show_members(members):
    print(f'Band {c.MAGENTA}{c.BOLD}{args.name}{c.RESET} (id={c.RED}{args.id}{c.RESET}) members: ')
    for i, member in enumerate(members):
        print('[', i, '] ', c.GREEN, member['name'], c.RESET, sep='')
    print()

def print_output(info):
    if len(info) != 0:
        for group_id, items in info.items():
            for i in range(1, len(items)):
                print(c.GREEN + items[i].split('=')[1] + c.RESET, end = '')
                if i != len(items)-1: print(',', end=' ')
            print(f' also played in {c.YELLOW}{items[0]}{c.RESET} {c.RED}(id={group_id}){c.RESET}')
    else:
        print(f"{c.RED}There are no other common bands for those members.{c.RESET}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Find groups that artists have played in from the specified group')
    parser.add_argument('--name', dest='name', metavar='N', nargs="+", type=str, default=[], help="find from name (default: %(default)s)")
    parser.add_argument('--id', dest='id', type=int, default=-1, help="find from id (default: %(default)s)")
    parser.add_argument('--token', dest='token', type=str, default='', help='authorization token for the Discogs API (default: %(default)s)')
    parser.add_argument('--conf', dest='conf_file', type=str, default='', help='specify json configuration file with token (default: %(default)s)')
    parser.add_argument('-b', '--bar', dest='bar', action="store_true", help="show progress bar (alive_progress library needed) (default: %(default)s)")
    args = parser.parse_args()
    args.name = ' '.join(args.name)

    #! get token
    if args.token != '':
        headers['Authorization'] = 'Discogs token='+args.token
        print(f'{c.YELLOW}Token loaded via {c.BOLD}--token{c.RESET}', sep='')
    elif args.conf_file != '':
        with open(args.conf_file, 'r') as f:
            headers['Authorization'] = 'Discogs token='+json.load(f)[0]['token']
        print(c.YELLOW, f'Token loaded via {c.BOLD}--conf{c.RESET}', c.RESET, sep='')
    else:
        print(f'{c.RED}Specify authentication token {c.BOLD}(run with -h or --help for further info){c.RESET}')
        sys.exit(0)

    #! get band ID or NAME
    if args.id != -1:
        print(f'{c.YELLOW}ID {c.RED}{args.id}{c.RESET} loaded', sep='', end='\n\n')
    elif args.name != '':
        args.id, args.name = find_id_from_name(args.name)
        print(f'{c.YELLOW}Artist name {c.MAGENTA}{c.BOLD}{args.name}{c.RESET}{c.YELLOW} loaded{c.RESET}', sep='', end='\n\n')
    else:
        print(f'{c.RED}Specify group NAME or ID {c.BOLD}(run with -h or --help for further info){c.RESET}')
        sys.exit(0)


    #! get artist api page
    artist_page_request = safe_get(artists_url+str(args.id))
    #! members info
    artist_json = json.loads(artist_page_request.text)
    if(args.name == ''):
        args.name = artist_json['name'] 
    members = artist_json['members']
    show_members(members)

    info = {}

    #! calculate the other bands common members
    if args.bar:
        from alive_progress import alive_bar
        with alive_bar(len(members), bar='circles', dual_line=True, title='Member info') as bar:
            for member in members:
                member_id = member['id']
                member_name = member['name']
                request_remaining = f'[{c.RED if int(request_limit) == 1 else c.YELLOW}{c.BOLD}Requests remaining={int(request_limit)-1}]{c.RESET}'
                bar.text = f'=> Loading member {c.GREEN}{member_name}{c.RESET} ({c.RED}id={member_id}{c.RESET}) info... {request_remaining}'
                get_member_info(member, info)
                bar()
    else:
        for member in members:
            member_id = member['id']
            member_name = member['name']
            print(f"Loading member {c.GREEN}{member_name}{c.RESET} ({c.RED}id={member_id}{c.RESET}) info...")
            get_member_info(member, info)    
        print()

    info.pop(args.id) #? we don't want the initial band
    info = {k: v for k, v in info.items() if len(v) > 2} #? erase groups with 1 member from original band
    info = dict(sorted(info.items(), key=lambda x:x[1])) #? sort by the band name
    print_output(info)