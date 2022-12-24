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

## Stratégie de conception des composants.

### Liste des composants.

1. La base de données (SQLite, MySQL, PostgreSQL, ...).
2. Le model de données (lib-ODB).
3. Le connecteur de base de données, le serveur TCP qui sert de couche d'abstraction de la base de données qui prend de l'XML en entrée et retourne les résultats en XML.
4. Le serveur Web qui sert les fichiers statiques de l'IHM et fournit une interface HTTP vers la base de données avec une API REST.
5. La documentation de l'application et des interfaces.
6. Les pages Web qui forment l'IHM pour fournir l'interface utilisateur.

### Ordonnancement des conceptions.

> **Règle numéro 1:** Les données doivent toujours être considérées comme des éléments pivot de l'application, c'est le cycle de vie de la données qui impose les comportements et les contraintes IHM.

- (2) Le model et les modificateurs d'objets persistés sont générés à partir du déclaratif des classes.
- (3) Les points d'entrée du connecteur de base de données sont générés à partir d'un fichier déclaratif et d'un script pour activer les comportements classiques (CRUD), les comportements spécifiques sont à créer et tester manuellement.
- (4) Les points d'entrée du serveur HTTP sont générés en même temps que les points d'entrée du connecteur de base de données pour les interfaces classique (CREATE = POST, READ = GET/POST, UPDATE = PUT/PATCH, DELETE = DELETE).
- (5) La documentation des API HTTP est généré à partir du fichier déclaratif qui est à l'origine des points d'entrée HTTP.
- (6) Les formulaires de l'IHM utilisent la documentation JSON des API HTTP pour leurs constructions.

> Utiliser cette génération centralisée permet de garantir l'unicité des comportements autour de la données. 

## Diagramme de déploiement

![Diagramme de composants](./components-diagram.svg)

## Conception du model

| Object-Type     | Description                                                                        | Relations                       |
| :-------------- | :--------------------------------------------------------------------------------- | ------------------------------- |
| User            | Les objets utilisateurs pour la connexion à l'application.                         | <ul><li>1 User appartient à 1 à N Group.</li><li>1 User peut avoir 0 à N User en amis.</li><li>1 User participe à 0 à N Party</li><li>1 User appartient à 0 à N Team</li></ul> |
| Group           | Les group d'utilisateurs pour les restrictions d'application.                      | <ul><li>1 Group contient 1 à N User</li></ul> |
| Dictionary      | Le dictionnaire pour regrouper les mots par langues.                               | <ul><li>1 Dictionnary contient 1 à N Word</li></ul> |
| Word            | Les mots pour le jeu du pendu.                                                     |                                 |
| Party           | Les parties jouées.                                                                |                                 |
| Message         | Les messages entre joueurs.                                                        |                                 |
| Chat            | Les chats sont les discutions entre User ou Team.                                  |                                 |
| Team            | Les regroupements de joueur pour les parties de groupe et les messages.            |                                 |


```mermaid
classDiagram
class RootModelObject
<<abstract>> RootModelObject
RootModelObject <|-- User : extends
RootModelObject <|-- Group : extends
RootModelObject <|-- Word : extends
RootModelObject <|-- Dictionary : extends
RootModelObject <|-- Party : extends
RootModelObject <|-- Message : extends
RootModelObject <|-- Chat : extends
RootModelObject <|-- Team : extends
Chat "1" *-- "0.N" Message
Chat "1" o-- "1.N" User
Chat "1" o-- "1" Party
Chat "1" o-- "1" Team
Team "1" o-- "1.N" User
Group "1" o-- "0.N" User
Dictionary "1" *-- "1.N" Word
Party "1" o-- "1.N" User
User "1" o-- "0.N" User
Party "1" o-- "1" Word
Message "1" o-- "1" User
Message "1" o-- "1.N" User
RootModelObject : # bool m_readonly
RootModelObject : # uint32_t m_id
RootModelObject : # uint32_t m_version
RootModelObject : # string m_created_by
RootModelObject : # uint32_t m_created_at
RootModelObject : # string m_updated_by
RootModelObject : # uint32_t m_updated_at
RootModelObject : + bool getReadonly()
RootModelObject : + void setReadonly(bool readonly)
RootModelObject : + uint32_t getId()
RootModelObject : + void setId(uint32_t id)
RootModelObject : + uint32_t getVersion()
RootModelObject : + void setVersion(uint32_t version)
RootModelObject : + string getCreatedBy()
RootModelObject : + void setCreatedBy(string created_by)
RootModelObject : + uint32_t getCreatedAt()
RootModelObject : + void setCreatedAt(uint32_t created_at)
RootModelObject : + string getUpdatedBy()
RootModelObject : + void setUpdatedBy(string updated_by)
RootModelObject : + uint32_t getUpdatedAt()
RootModelObject : + void setUpdatedAt(uint32_t updated_at)
RootModelObject : + ptree getErrors()
RootModelObject : + ptree toPtree()
RootModelObject : + string toXML()
RootModelObject : + string toJson()
User : - string m_login
User : - string m_password
User : - uint32_t m_last_connection
User : - string m_salt_user
User : - string m_salt_session
User : - string m_token
User : - Chat[] m_chats
User : - Group[] m_groups
User : - User[] m_friends
User : - Party[] m_parties
User : - Team[] m_teams
User : + string getLogin()
User : + void setLogin(string login)
User : + string getPassword()
User : + void setPassword(string password)
User : + uint32_t getLastConnection()
User : + void setLastConnection(uint32_t last_connection)
User : + string getSaltSession()
User : + void setSalSession(string salt_session)
User : + string getSaltUser()
User : + void setSaltUser(string salt_user)
User : + string getToken()
User : + void setToken(string token)
User : + Chat[] getChats()
User : + void setChats(Chat[] chats)
User : + Group[] getGroups()
User : + void setGroups(Group[] groups)
User : + User[] getFriends()
User : + void setFriends(User[] friends)
User : + Party[] getParties()
User : + void setParties(Party[] parties)
User : + Team[] getTeams()
User : + void setTeams(Team[] teams)
Group : - User[] m_members
Group : - string m_name
Group : + User[] getMembers()
Group : + void setMembers(User[] users)
Group : + string getName()
Group : + void setName(string name)
Dictionary : - string m_name
Dictionary : - string[] m_country_codes
Dictionary : - Word[] m_words
Dictionary : + string getName()
Dictionary : + void setName(string name)
Dictionary : + string getCountryCode()
Dictionary : + void setCountryCode(string country_code)
Dictionary : + Word[] getWords()
Dictionary : + void setName(Word[] words)
Word : - string m_name
Word : - string[] m_definitions
Word : + string getName()
Word : + void setName(string name)
Word : + string[] getDefinitions()
Word : + void setDefinitions(string[] definitions)
Party : - uint32_t m_start_date
Party : - uint32_t m_end_date
Party : - uint32_t m_score
Party : - bool m_win
Party : - User[] m_members
Party : - Chat m_chat
Party : - Word m_word
Party : + uint32_t getStartDate()
Party : + void setStartDate(uint32_t start_date)
Party : + uint32_t getEndDate()
Party : + void setEndDate(uint32_t end_date)
Party : + uint32_t getScore()
Party : + void setScore(uint32_t score)
Party : + bool getWin()
Party : + void setWin(bool win)
Party : + User[] getMembers()
Party : + void setMembers(User[] users)
Party : + Chat getChat()
Party : + void setChat(Chat chat)
Party : + Word getWord()
Party : + void setWord(Word word)
Message : - string m_content
Message : - User m_sender
Message : - User[] m_receivers
Message : - Chat m_chat
Message : + string getContent()
Message : + void setContent(string content)
Message : + User getSender()
Message : + void setSender(User sender)
Message : + User[] getReceivers()
Message : + void setReceivers(User[] receivers)
Message : + Chat getChat()
Message : + void setChat(Chat chat)
Chat : - Message[] m_messages
Chat : - User[] m_members
Chat : - string m_name
Chat : + User[] getMembers()
Chat : + void setMembers(User[] users)
Chat : + Message[] getMessages()
Chat : + void setMessages(Message[] messages)
Chat : + string getName()
Chat : + void setName(string name)
Team : - Chat m_chat
Team : - User[] m_members
Team : - string m_name
Team : + Chat getChat()
Team : + void setChat(Chat chat)
Team : + User[] getMembers()
Team : + void setMembers(User[] users)
Team : + string getName()
Team : + void setName(string name)
```

## Documentation Interface TCP Data-Management

### Description compositions des demandes TCP

### Description compositions des réponses TCP

```xml
<!-- bad create example -->
<status-code>400</status-code>
<errors>
  <error>
    <field>data.name</field>
    <message>SIZE</message>
  </error>
</errors>

<!-- conflict update example -->
<status-code>409</status-code>
<errors>
  <error>
    <field>data.id</field>
    <message>1</message>
  </error>
</errors>

<!-- success create example -->
<status-code>200</status-code>
<data>
  <users>
    <user>...</user>
  </users>
</data>
```

#### Liste des statuts des résultats des demandes.

Les statuts implémentés sont inspirés du protocole HTTP, les statuts implémentés sont les suivants :

| Code Statut | Text Status        | Description  | Verbeux |
| :---------- | :----------------- | ------------ | ------- |
| 200         | OK                 | La demande a bien été traité.                                                                           | true    |
| 400         | BAD_REQUEST        | La demande n'a pas une composition valide.                                                              | true    |
| 401         | NOT_AUTHORIZED     | L'utilisateur n'est pas identifié, l'opération ne peut être effectuée.                                  | false   |
| 403         | PERMISSION_DENIED  | Les droits de l'utilisateur connecté ne sont pas suffisants pour réaliser la demande.                   | false   |
| 404         | NOT_FOUND          | Le type d'objet demandé n'a pas été trouvé                                                                      | false   |
| 405         | METHOD_NOT_ALLOWED | Le type de demande n'est pas authorisé sur cet object                                                   | false   |
| 409         | CONFLICT           | Une demande de modification d'objet à été rejetée car le numéro de version de l'objet est obsolète      | true    |
| 500         | INTERNAL_ERROR     | Une erreur dans le traitement de la demande à eu lieu et ne peut pas être traité précisément            | false   |
| 502         | PROXY_ERROR        | La base de données n'est pas joignable                                                                  | false   |

## Stratégie des scripts.

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

### Objectifs

Les tests ont pour objectif de garantir que le fonctionnement du code est conforme à ce qui est attendu, la granularité des tests varie du test le plus précis (les tests unitaires) au test plus global (le test de fumée).

### Critères de validation d’un test

Pour être de qualité un test doit :

- Être simple à relire
- Être correctement documenté
- Avoir un attendu clair
- Avoir un périmètre défini et restreint
- Avoir des assertions utiles et susceptibles d’échouer en cas de régression ou de dysfonctionnement
- Être rejouable sans action manuelle (qu’il soit échoué ou réussi)
- Ne pas être dépendant d’un autre test
- Laisser un environnement de test propre après exécution.

### Types de test

Type de test ordonnés par priorité :

- **Test unitaire**, les nouvelles fonctionnalités développées doivent être testées en prenant en compte un maximum de cas potentiels pour un périmètre restreint.

- **Test d’intégration**, la compatibilité des différentes parties du logiciel est vérifié par les tests d’intégration. Les tests d’intégration ne vérifient pas l’intégralité des branches du codes déjà couvertes par les tests d’intégration mais le bon fonctionnement des échanges de chaque composant.

- **Test de fumée**, les fonctionnalités sont vérifiées à partir des accès utilisateurs sur le logiciel complet en réalisant des parcours utilisateurs nominaux pour vérifier la conformité du logiciel avec le cahier des charges initiales

Sujets de test :

- **Test IHM** (intégration, fumée), l’objectif du test est de garantir la conformité des comportement de l’IHM avec l’attendu, qu’il s’agisse du point de vue graphique (saisie de formulaire, transition, placement) ou réseau (entrées/sorties HTTP).

- **Test d’API** (intégration) les API exposées doivent être conformes à la documentation publiée en terme de ressources fournies et reçues. Les tests sont rédigés et maintenu à partir de la documentation produite

- **Test de performance IHM** (fumée) la réactivité de l’IHM est mesurée à partir du temps de chargement initiale (chargement des sources et des données), de la vitesse de rafraîchissement de la page (avec cache), de la réactivité de la mise à jour graphique local (sans rafraîchissement)

- **Test de performance Server** (intégration) la performance du serveur se mesure à partir du nombre de demande pouvant être traitée simultanément pour des requêtes simple, le temps de persistance d’un nombre de données (jeu de données fixe avec un nombre de relation entre objets identifiés)

### Règle communes

Les fonctionnalités doivent être testées de manière suffisantes avec un objectif de couverture de code > 80%, pour se faire la planification de TDD est à mettre en place.

Les tests doivent être utile en testant des implémentations de code représentant des fonctionnalités attendu des projets, on ne test pas les petites fonctions mais les fonctions significatives (utilisant des algorithmes complexes ou faisant appelle à d’autres fonctions) 

En prenant de la hauteur on identifie le code « mort » et on peut l’éliminer rapidement 

### Tests client

Les fonctionnalités qui ne sont pas graphiques sont à tester avec un environnement Front allégé (sans DOM)

Les spécificités du bundle doit être testées pour tous les environnements pour garantir qu'aucun comportement parasite ne sera dû à une différence d'environnement du runner de test

Les fonctionnalités graphiques sont à tester sur un résultat du code source en version de dev ou en version bundle (en version de dev de préférence pour collecter la couverture de code).

Pour les interfaces complexes (notamment la partie réseaux) il est possible de fournir des utilitaires de test pour simplifier les futures tests.

### Tests serveur

Les API REST proposées par le serveur doivent être testé avec un utilitaire facilement maintenable.

Les exceptions de mauvaise configuration et mauvais lancement doivent être testées, les traces dans la console ou les logs doivent être suffisantes pour comprendre l'origine du mauvais usage.

![Les tests unitaires de l'API HTTP du server](server-ut-tests.svg)

### Tests models

le model est testé de 3 façon : 

  - par des **tests unitaires** du connecteur de base de données.
  - par des **tests d'intégration IHM** à partir de parcours utilisateur pour tester les données sortantes en HTTP.
  - par des **tests d'intégration/interface serveur** pour tester ce qui rentre/sort en HTTP.

![Les tests unitaires du connecteur de base de données](data-management-ut-tests.svg)

## Convention de syntaxe

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

- Les fichiers ressources (png, svg, pdf, json, etc...)
- les classes CSS
- les identifiant CSS

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

#### Cas particuliers

Si possible éviter les majuscules pour les noms des fichiers (sauf quand le nom du fichier reprend le nom de son contenu)

Les fichiers de police de caractère suive le nom de la police avec son style et sa graisse,

> Exemple : pour une police « Nunito Sans » en italique avec une graisse de 300 donnerait Nunito-Sans_Italic_Light.svg
