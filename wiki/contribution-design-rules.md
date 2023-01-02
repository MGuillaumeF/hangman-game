# Règles de contribution au projet

## Objectif du document

Le présent document a pour rôle de définir les règles à suivre pour participer au projet.

## Règles de définition des tâches

Le "Projet" GitHub sert à référencer toutes les tâches à effectuer sur le projet,
Les tâches sont regroupées dans des "Milestones" qui sont les épiques à réaliser.

La notion d' "User Story" n'existe pas sur ce model car les tâches sont à granularité fine, car l'intégration d'une tâche doit se faire rapidement.

### Le template d'écriture des tickets

Les descriptions des tâches doivent respecter le format suivant :

```markdown
**Objectif**:

**Contraintes Imposées**:

**Norme de référence**:

**Conception préliminaire**:

**Maquettes**:

**Livrables attendus**:

**Documentations attendus**:

**Tests attendus**:

```

### Les critère de passage de la tâche à "prêt à commencer"

Pour qu'une tâche soit prête à commencer elle doit:

- avoir une description complète qui respecte le template.
- avoir le repository associé à la tâche.
- avoir un "épique" associé à la tâche.
- avoir le statut "TO DO".
- ne pas avoir de blocant.

### Les critères de clôture de la tâche

- les livrables doivent être réalisés.
- la documentation doit être rédigée.
- la chaîne d'intégration continu doit être passée avec succès.
- l'analyse sonar doit être revue.

### Les livrables attendus

Les livrables sont les éléments nouveaux à produire pendant la tâche, il peut s'agir d'un ou plusieurs morceau de code, d'un script, d'une ressource à créer ou encore d'une documentation.

### L'importance de la documentation

Le projet est une veille technologique, la documentation est un aspect primordial car c'est elle qui permet de transmettre le résultat de la veille technologique pour la répercuter sur un autre projet.

Chaque tâche crée doit notifier clairement la documentation associée, des tâches peuvent également être exclusivement documentaires pour ne pas impacter le déroulement de la progression de l'application.

### Le rôle des tests

Les tests ont plusieurs rôle, le principal rôle et d'éviter les régressions dû à des mises à jour de version des dépendances.

> Voir la documentation des tests pour plus de détails.

### Les critères de priorisation des tâches

Les tâches sont triées par priorité `Critical`, `Highest`, `High`, `Low` et `Lowest`.

Les tâches `Critical` sont les tâches qui ont des impacts bloquants une ou plusieurs autres tâches, elles ont généralement un impact significatif sur le projet et sont donc a traiter au plus tôt quand c'est possible.

Les autres niveaux sont un ordre de priorisation par la plus value de la tâche pour l'application.

> La priorité n'est qu'un classement indicatif, dans le contexte de la réalisation d'une application de veille technologique, les tâches sont réalisées par opportunité, le temps consacré à la progression du projet est très réduit, les temps libre de courte periode sont alloués aux tâches prêtes et facilement réalisable. Quand des temps libres sont conséquent c'est l'ordre de priorité qui s'applique. 

## Règles de gestion des versions

La gestion de version de l'application se fait avec git, le système d'hébergement choisi pour le repository est GitHub. 

> La stratégie Git mise en place est souple et n'exige pas une attention particulière pour maintenir un historique lisible car les dépendances et le formatage automatique ne sont pas reliés à un ticket du projet.

### L'usage de git en locale

En local l'usage de git se fait sur la stratégie du rebase fast forward et du squash commit.

Des hooks sont mis en place:

- pre-commit : pour formater le code avant le commit (clang-format pour le c++ et prettier pour le reste)
- commit-msg : pour placer le numéro du ticket GitHub et sont titre en haut du commit message. Exemple : `[1213] Mettre en place le composant graphique xxx`

### L'usage de git avec GitHub

Avec GitHub certains fonctionnements sont disponibles mais à utiliser avec parcimonie comme la mise à jour de demande de tirage qui engendre un commit de fusion et non rebase.

#### les autorisations d'écriture dans l'intégration continu

Les dépendances se mettent automatiquement à jour quand l'intégration continue d'une demande de tirage se termine avec succès.

Les dépendances qui se mettent à jour sont:

- les plug-ins GitHub actions
- les dépendances npm

Dans les demandes de tirage les fichiers sont automatiquement formatés avec les mêmes configurations qu'en local

> Voir la documentation de l'intégration continue pour plus de détail.
