In acest proiect 
Gwent este un joc de cărți strategic în care doi jucători își construiesc un pachet (deck) format din unități, 
cărți speciale, carti de vreme și eroi, și concurează pe o tablă împărțită în rânduri (melee, ranged, siege), pentru a obține 
un total de puncte mai mare decât adversarul în două din trei runde. Fiecare jucător începe cu o mână de cărți 
(fără tragere automată ulterioară), plasând cărți pe teren pentru a-și mari scorul sau folosind efecte speciale 
(vreme, abilități, eroi) care pot influența jocul. Rundele se termină când ambii jucători pasează, iar scorul total 
determină câștigătorul rundei. Primul care câștigă două runde câștigă jocul. Pentru abordarea celei dea 3 a teme am 
utilizat template pentru helper functions care ajuta la afisarea detallilor unei carti, avand click-ul pozitionat peste aceasta.
Iar ca design patterns am utilizat singleton in functia board iar apoi am creat classa deckbuilder ce foloseste design pattern-ul builder.
Am implementat o interfata grafica ce presupune imbunatatirea exerientei jocului. Tabla e impartita in 6 zone: 2 de close, 2 de ranged, 
2 de siege , o zona din fiecare pentru ambii jucatori. Cartile sunt randate pentru jucatorul curent cu fata in sus, avand fiecare
un design unic in functie de factiunea din care face parte, de tipul de care este: unitate, carte erou, carde de abilitate, carte
de vreme. Iar cartile inamicului sunt randate cu fata in jos. Cand player-ul curent da hover cu mouse-ul peste o carte, apare tooltipul.
In acest tooltip apar informatiile vitale despre carte si ce poate face aceasta.


Bibliografie:

https://witcher.fandom.com/wiki/Gwent -> Pentru a prelua o mare parte din regulile de joc ale acestui proiect.

Stroustrup, B. (2013). *The C++ Programming Language (4th ed.)*. Addison-Wesley.
    Practici pentru gestionarea memoriei cu std: unique_ptr și polimorfism.

Gamma, E., et al. (1994). Design Patterns: Elements of Reusable Object-Oriented Software. Addison-Wesley.
    Modele aplicate: State (pentru fazele de joc), Factory (crearea de cărți), Observer (evenimente de joc).

Microsoft Docs (2023). C++ Standard Library Reference.
    https://learn.microsoft.com/en-us/cpp/standard-library/cpp-standard-library-reference?view=msvc-170
    Referinte pentru utilizarea std::vector, std::map, si algorithms.

Nystrom, R. (2014). Game Programming Patterns. Genever Benning.
    https://gameprogrammingpatterns.com/
    Folosit pentru sistemul turn-based și secvențierea efectului cardului.

Google Test (2023). C++ Unit Testing Framework.
    https://github.com/google/googletest
    Pentru a verifica interactiunile cartilor si regulile jocului.


https://github.com/mcmarius/poo/blob/master/README.md
https://github.com/mcmarius/poo/tree/master/libs


Acest cod se va rula cu ajutorul comenzii: cmake .. ,iar apoi cu make in directorul build
Iar apoi se va rula executabilul cu: ./gwent