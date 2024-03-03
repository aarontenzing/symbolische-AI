# Branch And Bound

## Totale afstand van deeloplossing

**Methode**: finalDistance(node)

Om de eerste kostfunctie te testen moet deze methode in de if-loop uit commentaar gezet worden en de lijn daaronder moet in commentaar staan. 
    
Deze lijn uit commentaar: 'if finalDistance(cnode) <= ub: # Eerste kostfunctie'
Deze lijn in commentaar: 'if bestCost(cnode, opt) <= ub: # Tweede kostfunctie'

## Beste-kost ondergrens

**Methode**: bestCost(cnode, options)

- Om de tweede kostfunctie te testen moet deze methode in de if-loop uit commentaar gezet worden en de lijn daarboven moet in commentaar staan. 

Deze lijn in commentaar: 'if finalDistance(cnode) <= ub: # Eerste kostfunctie'
Deze lijn uit commentaar: 'if bestCost(cnode, opt) <= ub: # Tweede kostfunctie'

**Opmerking**:  
    
- Als we de functie implementeren zoals in die in de slides staat, merkten we op dat het enkel werkt wanneer we slechts één kortste afstand gebruiken. 
- Als we effectief de twee korste afstanden nemen bekomen we een lowerbound die vaak te groot is, waardoor we niet de optimale oplossing vinden. 

- We hebben echter gemerkt dat als je de afstanden van de deeloplossing berekent via de methode "Distance(node)", en dus niet de laatste afstand er bij optelt en in dit geval wel effectief de twee korste afstanden neemt, bekom je dan wel de juist oplossing.

- Bij de methode bestCost(cnode,options)

Deze lijn in commentaar: 'lb += 2*finalDistance(cnode)'
Deze lijn uit commentaar: '# lb += 2*Distance(cnode) # Alternatief'

Deze lijn in commentaar: 'lb += distances[0] + distances[1]'
Deze lijn uit commentaar: '# lb += distances[1] + distances[2] # Alternatief'