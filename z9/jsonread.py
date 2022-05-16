import json

with open('data.json', 'r') as f:
    data = json.load(f)

for i in range(len(data)):
    conversion = data[i]['pair'].split('_')
    data_time = data[i]['ts'][:-1].split('T')
    offers = data[i]['directExchangeOffers']
    forex_change = 100 * (
        (float(offers['forexNow']) - float(offers['forexOld']))
        / float(offers['forexOld'])
    )
        
    print(f'{conversion[0]} to {conversion[1]} at {data_time}:')

    for offer in offers.items():    
        print(f'{offer[0]} = {offer[1]}', end = '  |  ')
    print(f'change = {format(forex_change, ".4f")}%\n')