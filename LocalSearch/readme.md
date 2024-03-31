# Beschrijving programma

Het programma werd gecompileerd via gcc.
Het programma kan gerund worden door volgende lijnen in een terminal in te geven
met gepaste argumenten:

    gcc prog.c -o prog
    ./prog <input_file> <solution_file> <time_limit> <random_seed> <num_threads>

## (Meta)heuristiek
Het programma wordt standaard gerund via het local search algoritme.
Op lijn 408 van prog.c staat dit nu zo.
Als men wenst het demon algoritme te testen moet lijn 408 in commentaar gezet worden
en lijn 409 uit commentaar gehaald worden.

## Zoekmethoden
De verschillende zoekmethoden staan op lijn 339-341 gedefinieerd.
Standaard staat de swap-methode aan. Een andere methode kan gekozen worden
door de respectievelijke lijn uit commentaar te halen en de andere twee lijnen in commentaar te zetten.