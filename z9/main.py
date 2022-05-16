import json
import sys
import time
import requests
import argparse
from bs4 import BeautifulSoup
import os

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

iterations = 0
exchange_amount = 3

if __name__ == "__main__":
    os.system('clear')
    
    parser = argparse.ArgumentParser(description='Exchange currencies')
    parser.add_argument('-exchange', dest='exchange', type=str, default='PLN', help="displays exchange rate to $code and from $code (default: PLN)")
    parser.add_argument('-it', dest='iterations', type=int, default=1e3, help='amount of updates done')
    parser.add_argument('-codes', nargs="+", default=["EUR", "USD"], help='codes which we want to exchange to $code (default: EUR, USD)')
    args = parser.parse_args()

    print(args.exchange, args.iterations)

    # if len(args) == 0:
    #     print(
    #         """Run with code of the currency e.g \033[32mEUR\033[0m
    #             Can be run with: 
    #             --help (displays available currency codes)
    #             -exchange (displays exchange rate to $code and from $code, e.g -PLN will show exchange rate to PLN and from PLN)
    #             -($number) (amount of updates done by the programme, e.g -3 )
    #         """
    #     )
    
    for i in range(iterations):
        try:
            r = requests.get(api)
            r.raise_for_status()
        except requests.exceptions.HTTPError as e:
            raise SystemExit(e)
        soup = BeautifulSoup(r.content, 'html.parser')

        data = json.loads(soup.text)

        for j in range(len(data)):
            conversion = data[j]['pair'].split('_')
            if conversion[1] == 'PLN':
                data_time = data[j]['ts'][:-1].split('T')

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
        
        show_bar()
        print()