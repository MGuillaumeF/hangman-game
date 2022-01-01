# Règles de développement

## Gestion de la qualité 

### Métriques à suivre :

- Couverture de code
- Duplication de code
- Les options de compilation doivent être actives (typescript et c++)
- Le code doit être uniforme (formateurs prettier et clang-format)
- La qualité doit correspondre aux recommandations du langage (linter eslint et clang-tidy+cppcheck)
- Les conceptions et rapports doivent être conservés
- L’analyse de la qualité doit être rapidement démontrable (automatisation et agrégation des résultats sur SonarCloud).
- Les composants et les fonctions doivent être typées et avoir des performances acceptables

## Stratégie des scripts

Les scripts ont pour objectif d’automatiser les tâches récurrentes ou complexes.

Les scripts doivent remplir la liste d’obligations suivante :

- Le script doit pouvoir être utilisé sur tous les plus grand OS
- Le script doit utiliser le moins de ressource possible
- Le script doit pouvoir être utilisable par CLI simplement
- Le script doit pouvoir être importé par un autre script
- Le script doit fournir un mode d’emploi
- Le script doit être configurable
- Le script doit être testé (s’il comporte de l’algorithmique)
- Le script doit être documenté
- Le script doit retourner un code d’erreur en cas d’échec
- Le script doit pouvoir passer sous silence son code d’erreur
- Le script doit pouvoir fournir des logs en quantité suffisante
- Le script doit fournir un modificateur de verbosité des logs
- Le script doit être commenté en quantité suffisante

## Stratégie de log

Les logger doivent remplir la liste d’obligations suivante :

- Le logger doit être extensible
- Le logger doit fournir des appenders en quantité suffisante
- Le logger doit fournir un parser générique extensible
- Le logger doit être configurable

> Le format de log générique est le suivant :
>
> - La date : `YYYY-MM-DD hh:mm:ss.mmm`
> - Le niveau du log : `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`
> - Le nom du thème (nom de script/nom de fonction, nom de partie) :
> - Le protocole : `[HTTP]`, `[TCP]`, `[SMTP]`, `[IO]`
> - Le message : format libre

## Organisation des tests

### Règle communes

Les fonctionnalités doivent être testées de manière suffisantes avec un objectif de couverture de code > 80%, pour se faire la planification de TDD est à mettre en place.

Les tests doivent être utile en testant des implémentations de code représentant des fonctionnalités attendu des projets, on ne test pas les petites fonctions mais les fonctions significatives (utilisant des algorithmes complexes ou faisant appelle à d’autres fonctions) 

En prenant de la hauteur on identifie le code « mort » et on peut l’éliminer rapidement 

### Tests client

Les fonctionnalités qui ne sont pas graphiques sont à tester avec un environnement JavaScript allégé

Les fonctionnalités graphiques sont à tester sur un résultat du code source en version de dev ou en version bundle.

Pour les interfaces complexes (notamment la partie réseaux) il est possible de fournir des utilitaires de test pour simplifier les futures tests

### Tests serveur

Les API REST proposées par le serveur doivent être testé avec un utilitaire facilement maintenable

Les exceptions de mauvaise configuration et mauvais lancement doivent être testées

### Tests models

le model est testé de 3 façon : 

  - par le client pour tester ce qui sort en HTTP
  - par le serveur pour tester ce qui rentre en HTTP
  - par un utilitaire pour comparer les flux client et serveur

## Convention de syntaxe : 

### Convention de composition

Les fichiers, les variables, méthodes, classes, interfaces ne doivent pas contenir :
  - d’accent
  - d’espace
  - de caractère spéciaux

Les variables, méthodes, classes, interfaces doivent commencer par une lettre.

### Les préfix

  - _ avec un underscore on déclare que l’attribue ou la fonction ne doit pas être utilisée.
  - E avec un « E » majuscule devant le nom des énumérés
  - I avec un « I » majuscule devant le nom des interfaces

### Les suffixes

  - _t Pour les déclarations de types c++

### Les extensions

  - pour les images on privilégie les .svg et les .png
  - pour les fichiers HTML on utilise uniquement.html et non pas .htm
  - pour les fichiers de style on privilégie les .scss
  - pour les fichiers TypeScript on utilise l’extension .tsx pour les fichiers contenant de la notation TSX, sinon .ts
  - pour les fichiers markdown on utilise l’extension .md 

On n’utilise pas de JavaScript sauf pour les configurations ne supportant le TypeScript

### Convention de casse

#### SNAKE CASE

Les fichiers ressources (png, svg, pdf, json, etc...)

#### SCREAMING SNAKE CASE

- Les constantes de configuration 
- Les constantes exportées
- Les clés de traduction
- Les énumérer 

#### CAMEL CASE

- Les variables
- Les fonctions 

#### UPPER CAMEL CASE

- Les fichiers des classes
- Les classes
- Les interfaces
- Les composants React

#### Cas particuliers :

Si possible éviter les majuscules pour les noms des fichiers (sauf quand le nom du fichier reprend le nom de son contenu)

Les fichiers de police de caractère suive le nom de la police avec son style et sa graisse,

> Exemple : pour une police « Nunito Sans » en italique avec une graisse de 300 donnerait Nunito-Sans_Italic_Light.svg
