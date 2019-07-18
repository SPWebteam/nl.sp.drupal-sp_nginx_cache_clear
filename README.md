# SP Nginx cache clear

Deze module maakt het in combinatie met een lua script voor nginx mogelijk om nginx caches te legen. Hiervoor maakt de module gebruik van rules. De module voegt een "Nginx cache clear" actie toe. Als je deze actie in een ruleset gebruikt, dan moet je één of meerdere reguliere expressies opgeven. De nginx caches van pagina's waarvan de paden matchen met de reguliere expressie worden dan gewist als die actie wordt getriggered.

Bijvoorbeeld een rule om de caches van nu pagina's te wissen, en het nieuws overzicht te verversen als nu pagina's worden bewerkt.
~~~
Event:
  - After updating existing content
  - After creating new content
Conditions:
  - Content is of type: nu
Action:
  - Nginx regex cache clear: 
      - [node:url]
      - nu.*
~~~

Voor www.sp.nl is een submodule toegevoegd met voorgedefinieerde rules.