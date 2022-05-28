Program nalezy uruchomic nazwą bądź ID zespołu w Discogs (flaga --name lub --id)
Do programu należy dodać własny token uwierzytelniający (--token $(TOKEN) lub --conf $(CONFIGURATION_FILE))

Dodatkowo:
program można uruchomić z flagą -h, wyświetlona jest wtedy pomoc (dodatkowe informacje o flagach)
program można uruchomic z flagą -b lub --bar (wymagana biblioteka alive_progress) aby wyświetlić pasek postępu

Przykładowe uruchomienie:
    python3 main.py --name Budka Suflera --conf configuration.json
        lub
    python3 main.py --name Budka Suflera --conf configuration.json --bar