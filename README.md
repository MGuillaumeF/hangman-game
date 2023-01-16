# Hangman Game

[![HANGMAN-GAME - DEFAULT](https://github.com/MGuillaumeF/hangman-game/actions/workflows/default.yml/badge.svg?branch=main)](https://github.com/MGuillaumeF/hangman-game/actions/workflows/default.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=MGuillaumeF_hangman-game&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=MGuillaumeF_hangman-game)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=MGuillaumeF_hangman-game&metric=coverage)](https://sonarcloud.io/summary/new_code?id=MGuillaumeF_hangman-game)
[![Code Smells](https://sonarcloud.io/api/project_badges/measure?project=MGuillaumeF_hangman-game&metric=code_smells)](https://sonarcloud.io/summary/new_code?id=MGuillaumeF_hangman-game)
[![wakatime](https://wakatime.com/badge/user/9f76e922-98e1-4ef0-b832-f1f6bb21d4c3/project/ce7dd7e0-ee64-42f5-8498-b9e4dc5161ac.svg)](https://wakatime.com/badge/user/9f76e922-98e1-4ef0-b832-f1f6bb21d4c3/project/ce7dd7e0-ee64-42f5-8498-b9e4dc5161ac)

## Description de l'application

L'application est un "jeux du pendu".

## Description des composants de l'application

### Le serveur HTTP

#### Le rôle du composant

-   servir l'application web.
-   gérer les accès clients.
-   accéder à la base de données depuis l'application web.

#### Conception

-   Langages: C++, XML, JSON
-   Librairies: Boost
-   Patterns: REST API
-   Bundler: CMake
-   Packager: CPack
-   Docs: Doxygen, Graphviz, Swagger/OpenAPI
-   Gestionnaire de paquet: vcpkg
-   Plateforme Cibles/Architecture Cibles: Windows/x86, Windows/x64, MacOSX/x64, Linux/x86, Linux/x64, Linux/arm64
-   Cible de Compilation: librairie statique

### Le serveur TCP

#### Le rôle du composant

-   accéder à la base de données depuis le serveur HTTP.
-   valider les données.
-   importer/exporter des données.

#### Conception

-   Langages: C++, XML
-   Librairies: Boost, libODB, ODB
-   Patterns: Model, ORM
-   Bundler: CMake
-   Packager: CPack
-   Docs: Doxygen, Graphviz, Merise
-   Gestionnaire de paquet: vcpkg
-   Plateforme Cibles/Architecture Cibles: Windows/x86, Windows/x64, MacOSX/x64, Linux/x86, Linux/x64, Linux/arm64
-   Cible de Compilation: librairie statique

### Le broker STOMP

#### Le rôle du composant

-   envoyer des notifications de synchronisation de mise à jour d'objet
-   envoyer des notifications d'alerte aux utilisateurs

#### Conception

-   Langages: C++, XML, JSON, TypeScript
-   Librairies: Boost, stomp-broker-js, ws
-   Bundler: CMake
-   Packager: CPack
-   Docs: Doxygen, Graphviz
-   Gestionnaire de paquet: vcpkg
-   Plateforme Cibles/Architecture Cibles: Windows/x86, Windows/x64, MacOSX/x64, Linux/x86, Linux/x64, Linux/arm64
-   Cible de Compilation: librairie statique

### L'application Web

#### Le rôle du composant

-   accéder au jeu à travers un navigateur.
-   accéder à la messagerie du jeu.

#### Conception

-   Langages: HTML, TypeScript, SASS, JSON
-   Librairies: React, Cypress
-   Patterns: Objet, FunctionComponents, ReactHooks
-   Bundler: WebPack
-   Packager: CPack
-   Gestionnaire de paquet: npm
-   Cible de Compilation: fichiers statiques HTML, CSS, JavaScript
-   Compatibilité: Navigateur Internet Explorer > 11

### Le client TCP

#### Le rôle du composant

Accéder aux informations de la base de données (lecture/écriture) à partir de requête TCP.

#### Conception

-   Langages: C++, XML
-   Librairies: Boost
-   Patterns: Objet, Model
-   Bundler: CMake
-   Packager: CPack
-   Docs: Doxygen, Graphviz, Merise
-   Gestionnaire de paquet: vcpkg
-   Plateforme Cibles/Architecture Cibles: Windows/x86, Windows/x64, MacOSX/x64, Linux/x86, Linux/x64, Linux/arm64

### Le client STOMP

#### Le rôle du composant

-   envoyer des demandes d'observation d'objet
-   envoyer des arrêts d'observation d'objet
-   recevoir des notifications de synchronisation de mise à jour d'objet
-   recevoir des notifications d'alerte aux utilisateurs

#### Conception

-   Langages: TypeScript, JSON
-   Librairies: stomp/stompjs, ws
-   Patterns: Singleton
-   Bundler: WebPack
-   Packager: CPack
-   Gestionnaire de paquet: npm
-   Cible de Compilation: embarqué dans l'application web

## Compilation de boost avec clang

```
bootstrap --with-toolset=clang-win
b2 --build-dir=C:\Boost toolset=clang-win --build-type=complete -a
b2 toolset=clang-win
```

[Lien vers l'analyse SonaCloud](https://sonarcloud.io/project/overview?id=MGuillaumeF_hangman-game)
