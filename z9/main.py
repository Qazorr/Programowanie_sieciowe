import json
import sys
import time
import requests
import argparse
from bs4 import BeautifulSoup

url = "https://internetowykantor.pl/kurs-euro/"
api = "https://klient.internetowykantor.pl/api/public/marketBrief"

def show_bar(show = True):
    if show:
        from alive_progress import alive_bar
        with alive_bar(1000, force_tty=True, title='Waiting for update') as bar:
            for _ in range(1000):
                time.sleep(0.01)
                bar()
    else:
        print("Waiting 10s for data update")
        time.sleep(10)
    # os.system('clear')

codes = [
    'AUD', 'BGN', 'CAD', 'CHF', 'CZK', 

    'DKK', 'EUR', 'GBP', 'HKD', 'HRK', 
    'MXN', 'NOK', 'NZD', 'RON', 'RUB', 
    'SEK', 'SGD', 'TRY', 'USD', 'ZAR'
]

if __name__ == "__main__":
    #parsing the main arguments    
    parser = argparse.ArgumentParser(description='Exchange currencies')
    parser.add_argument('-to', dest='c_to', type=str, default='PLN', help="displays exchange rate to $(code) and from $(code) (default: PLN)")
    parser.add_argument('-i', '--iterations', dest='i', type=int, default=1000, help='amount of updates done')
    parser.add_argument('-from', dest='c_from', nargs="+", default=codes, help='codes to exchange to $(code) (default: look --codes)')
    parser.add_argument('-c', '--codes', action="store_true", help="display possible codes")
    parser.add_argument('-b', '--bar', action="store_true", help="show progress bar (alive_progress library needed)")
    args = parser.parse_args()

    #print possible codes
    if args.codes:
        print("Possible codes (remember some of them could not have the conversion to other):")
        for code in codes:
            print(code, end=" ")    
        print()
        sys.exit()    

    #print bad -from arguments
    for code in args.c_from:
        if code not in codes:
            print(f"Bad argument -from: {code}")

    #main loop
    for i in range(args.i):
        converted = 0
        try:
            r = requests.get(api)
            r.raise_for_status()
        except requests.exceptions.HTTPError as e:
            raise SystemExit(e)
        soup = BeautifulSoup(r.content, 'html.parser')

        data = json.loads(soup.text) #get the api json with exchanges

        for j in range(len(data)): #get every json object
            conversion = data[j]['pair'].split('_') #conversion e.g ['EUR', 'PLN'], from->to
            if conversion[0] in args.c_from:
                if conversion[1] == args.c_to:
                    data_time = data[j]['ts'][:-1].split('T') #get time from api (GMT time)

                    offers = data[j]['directExchangeOffers']
                    forex_change = 100 * (
                        (float(offers['forexNow']) - float(offers['forexOld']))
                        / float(offers['forexOld'])
                    )
                    offers.pop('forexOld') 
                    offers.pop('buyOld') 
                    offers.pop('sellOld')
                    
                    print(f'{conversion[0]} to {conversion[1]} on [{data_time[0]} {data_time[1]}]: ', end='| ')

                    for offer in offers.items():    
                        print(f'{offer[0]} = {format(offer[1], ".4f")}', end = ' | ')
                    print(f'change = {format(forex_change, ".4f")}%')
                    converted += 1
        if(converted == 0): #the arguments produced 0 queries 
            print("NO MATCHES, TRY DIFFERENT ARGUMENTS")
            sys.exit()
        show_bar(args.bar) #show progress bar (alive_progress needed)
        print()