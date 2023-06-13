# Les usages de Manifests dans le projet

## Le rôle général d'un manifest

Un manifest est un fichier d'identité de l'application,
il est nécessaire aux différents outils pour garantir le fonctionnement d'une installation ou d'une production de l'application.
on y retrouve différentes informations :

- le nom de l'application
- la version de l'application
- la description de l'application
- les dépendances de l'application
- la version des dépendances de l'application
- la license de l'application
- l'auteur de l'application

## Le manifest vcpkg.json

Le manifest de vcpkg permet de définir la liste des dépendances C++
Quand un manifest est présent au moment dans le dossier courant au moment de l'appel de la commande cmake de génération de la solution, la commande d'installation vcpkg est appelé (avec le triplet par défaut)

## Les manifests package.json

Les manifests package.json sont nombreux sur le projet :

- /client
- /tools/cli-tools
- /data-management/tools/model-generator

Les package.json séparent les dépendances de développement des dépendances de production, il est possible de l'installer que les dépendances de production avec la commande `npm install --production`.
Les package.json peuvent contenir des informations supplémentaires comme des blocs de configuration comme la cible de compatibilité navigateur.
Les package.json peuvent contenir une liste de commande script disponible qui peuvent être lancées avec la commande `npm run <nom_de_la_commande>`.
