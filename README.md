# Rapport projet XML

# Groupe 8

Pierre EBRAHIM

Swann HERRERA

# xml-editor

Un editeur de XML en C avec GTK qui prend en compte les DTD

## Utilisation

`make build` Build exe

`make start` run exe. This one needs argument inputFile

exemple:

```ZSH
make start inputFile=./dtd-exemple.xml
```

`make dev` run build and start

```ZSH
make dev inputFile=./dtd-exemple.xml
```

## Introduction

![Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/XML-app.png](Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/XML-app.png)

![Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled.png](Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled.png)

La demande consiste en la réalisation d’une application avec le langage C, permettant de
verifier que le XML correspond à la DTD.
Ce travail a été réalisé par un groupe de deux personnes. Nous ne nous connaissions pas
avant cette collaboration.

## Une analyse rapide de l'application

Il existe quelque fonction très importante dans l'application, tout d'abord le main qui est la fonction d'entré dans le programme, ce main fait appel a d'autre fonction qui sont aussi des fonctions de haut niveau

![Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%201.png](Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%201.png)

Parse DTD (**parse_dtd**) : est la fonction qui va Crée un ensemble de structure pour modelisé la DTD.
Cette Structure retient son parent et garde un tableau de ses enfants qui sont du même type qu'elle.
elle a aussi une liste chainé d'attributs, on stock aussi le compte des attributs. 

le caractère d’occurrence et le flag d'occurence nous permettent de géré l'occurence des elements.

![Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%202.png](Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%202.png)

Les Attributs de la DTD sont stocker dans une structure appeler **DTD_attribute** qui est une liste simplement chainé

![Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%203.png](Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%203.png)

Les valeur du type et de la valeur de l'attribut pour la dtd sont représenté par des **enums**

![Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%204.png](Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%204.png)

L'autre point important est la structure **XML_Element**, elle contient les information consertant la balise XML elle stock aussi les attribut sous forme de liste chainé.

elle a un niveau de profondeur, et une valeur booleen pour géré les balises autofermante.

![Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%205.png](Rapport%20projet%20XML%2051eadfb16e3945a2bfe957a3ab6e7261/Untitled%205.png)

Cette fois pour les informations de l'attribut on utilise une autre structure.

La fonction qui parse le XML s'appel **parse_xml** elle fait appel a de nombreuse autre fonction comme par exemple **make_attribute**, **get_content**...

Il existe une autre fonction très importante, c'est **check_dtd_correspond_to_xml** elle parcours la structure XML et la structure DTD pour verifié que le XML correspond bien a la DTD.

Afin de vérifier l'occurrence d'un enfant au sein d'un fichier XML on traduit les charactère d'occurrence avec un flag comme suit :

[Utilisation du flag d'occurence](https://www.notion.so/0ad0002785da4b0bac4bbee725886962)

Ainsi lorsque le premier bit est à 0 on a forcement un élément et si il est à 1 on peut ne pas avoir d'élément. Également pour le second bit si il est à 0 on ne peut avoir de multiples enfants au contraire si il est à 1 on peut avoir une multitude d'élément. 

Ce procéder permet dans le cas où l'on aurait une définition d'occurrence pour des enfants tel que 
<!ELEMENT classrooms (classroom?)+> à l'aide d'opération binaire on peut donc assemblé les flags des char '?' et '+' qui nous donne le flag du char '*'.

### installation de l'application sous linux

Ouvrir un terminal aller dans votre repertoire préféré

**git clone** [https://github.com/SwannHERRERA/xml-editor.git](https://github.com/SwannHERRERA/xml-editor.git)
**cd** xml-editor

**./setup.sh** (utilisation d'APT donc de préférence utilisé une distribution debian ou basé sur debian)

**make build**

**make start**

## **Utilisation**

### Partie 1 - 3

**make build** construit un exécutable de l'application

**make start** lance l'exécutable celui ci a besoin d'un argument **inputFile** lui indiquant le fichier XML

sous forme d'un chemin soit relatif soit absolut 

exemple

```bash
make build
make start inputFile=./xml-test/basic.xml
```

---

### Partie 4

**Make build**

**Make start** 

ouvrir un fichier grâce a la barre d'action (la bar ALT)

Click droit → vérifié

## Difficultés technique

### Partie 1

Problème:
Les balises !ELEMENT contenant des enfants avec un charactère '|' comme séparateur ne sont pas parsé, tout élément suivant ce charactère ne sera pas récupérer car la fonction s'arrête à tout charactère non valide à un nom de balise.
Les balises contenant des sous bloc de parenthèse tel que :
<!ELEMENT classrooms (#PCDATA | (sutdent+, teacher))*> ne sont pas parsé car trop complexe.
Les types de balise #ANY et #EMPTY ne sont pas prise en compte.

### Partie 2

Problème:
Lors du parse des AttributeValue une difficulté a été rencontré lors de la vérification d'une chaine de caractère avec un tableau de type const char *names[] = {"Index0", "Index1", "Index2"}; l'index du premier et du deuxième élément était inversé, afin de résoudre le problème les valeur ont été inversé afin de correspondre à l'index des valeurs de l'enum.
La valeur du #FIXED n'est pas récupérer dans la DTD.

### Partie 3

Problème:
Etant donné que le parse de la DTD est fait de manière récursive les enfants au plus bas niveau sont parsé en premier, ainsi si nous avons un enfant référençant un parent, ce qui semble être un cas valide dans un XML, la fonction récursive va bouclé à l'infini ce qui causera un segfault.
Tentative de résolution:
Afin de résoudre le problème il aurait fallut itérer sur tout les parents du nouvel élément et vérifier si le nom de l'enfant du nouvel élément correspond à l'un des parents, ainsi si l'élément existe déjà il aurait fallut renvoyer un pointeur sur celui-ci. Cette solution à était abandonné car le fait de définir le parent avant de parser les enfants aurait été très complexe dans la modification. Et par faute de temps cela a été abandonné à profit d'autre problème.

### Partie 4

L'autocomplétions n'a pas été implémentés car cette fonctionnalité de GTK n'est pas suffisamment documenté. 

## Bilan

Le fait que nous ne nous connaissions pas n’a pas facilité la prise de contact au début de
notre travail.
Malgré ce re confinement, la quasi totalité de notre projet a été réalisée grâce à la
disponibilité et la bonne entente de notre duo.
L’absence de rencontre physique de l’ensemble de la classe, due au confinement, nous a
privé de la co-formation et échanges qui nous enrichi toujours lorsque nous partageons notre
travail.
Habituellement, cette rencontre naturelle, en présentiel, nous permet également d’avoir une
lecture des consignes et de se les confirmer.

Etant entrés trop rapidement dans le code et n’ayant pas analysé suffisamment en détail les
spécificités de DTD, nous avons réalisé tardivement que la gestion des types d’attributs
seraient aussi complexe.
Y revenir au moment où nous avons fait ce constat, nous aurait fait perdre trop de temps.
De ce fait, le type des attributs n’a pas été totalement géré.

La DTD exigeait qu’une valeur soit fixée. Contrairement aux attributs, nous en avions
conscience dès le départ. Néanmoins, devant la charge de travail nous avons fait le choix de
ne pas fixer de valeur.

Notre impossibilité d’utiliser les librairies externes ne nous facilite en rien l’accès aux DTD
externes sur le réseau.
Par ailleurs, aller sur le réseau pour récupérer une DTD externe implique de gérer différents
protocoles.
Ce sont les raisons pour lesquelles nous n’avons pas géré les DTD externes sur le réseau.

Sur le plan humain, mais aussi pédagogique, il nous semble important de préciser que ce
travail nous a permis de faire connaissance, même si ce n’est que par écrans interposés.
Depuis septembre, le nombre de nouveaux arrivés dans cette classe est considérable, mais a
aucun moment nous n’avons pu nous rencontrer à cause de la pandémie Covid 19.
Le fait d’avoir réalisé ce projet à deux, nous a permis de faire un peu connaissance, de se
sentir solidaires et, de s’entraider même sur des aspects étranger à ce projet.