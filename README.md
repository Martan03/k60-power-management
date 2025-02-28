# k60-power-management

K60 Sub-Family Power Management.

## Description

The program implements power mode switching on FITkit3 containing K60
Sub-Family core. All the details are then described in Czech in the
`doc/doc.tex` file.

## Assignment (in Czech)

### Požadavky na řešení:
1) **povinné** - pro mikrokontrolér zvolené platformy shrňte klíčové informace
o jím podporovaných režimech činnosti (Run, Wait, Stop, Very Low Leakage
apod.), o spotřebě mikrokontroléru v těchto režimech, o možných přechodech mezi
nimi, o jejich ceně (latence apod.) a mechanismech těchto přechodů; shrňte
informace o klíčových prostředcích (CPU, sběrnice, napájení, paměti, periferie
\- RTC, ADC, GPIO - atd.) mikrokontroléru a jejich vlastnostech (ON, OFF,
volitelně ON/OFF, nízký příkon či jiné omezení) v mikrokontrolérem
podporovaných režimech činnosti; zvolte si alespoň tři režimy činnosti (např.
Run, Wait, Stop) a alespoň dva prostředky (např. CPU, GPIO) a vytvořte
jednoduchou vestavnou aplikaci implementující přepínání mezi zvolenými režimy,
s demonstrací dopadu přepnutí na činnost zvolených prostředků (vykonávání
programu, vliv na vstup/výstup apod.),
2) **volitelné** - implementace bez využití abstrakční vrstvy (z SDK apod.),
implementace probuzení z režimu s velmi nízkou spotřebou, velmi hlubokého
spánku atp., implementace netriviální aplikace s řízením spotřeby apod.

**Je nutné zdokumentovat:** Obecné požadavky viz kap. 2 (Požadavky na
dokumentaci) Průvodce projektem předmětu IMP; specifické požadavky: přehledově
slovní a tabulkové shrnutí informací o režimech činnosti (spotřeba, přechody a
jejich cena) a o prostředcích mikrokontroléru a jejich vlastnostech v
jednotlivých režimech, detailněji pak o zvolených režimech a prostředcích; z
hlediska aplikace klíčové vybavení (např. použité interní/externí periferie),
jejich propojení a interakce, ovládání, popis chování/činnosti a způsob
implementace vytvořené aplikace; použité moduly, registry atp. a nastavení
mikrokontroléru, využité (netýká se částí běžně, tj. bez další instalace,
dostupných ve zvoleném IDE) definice, části API použitých knihoven apod.,
informační zdroje (stěžejní odkazy na technickou dokumentaci, vývojářská fóra,
příklady z SDK, Github atp.) využité při řešení a části řešení, které byly
odněkud (nezapomeňte doplnit odkaz na příslušný zdroj) převzaty.

## Links

- **Author:** [Martan03](https://github.com/Martan03)
- **GitHub repository:** [k60-power-management](https://github.com/Martan03/k60-power-management)
- **Author website:** [martan03.github.io](https://martan03.github.io)
