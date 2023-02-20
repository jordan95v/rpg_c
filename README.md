# Roguelike C

Jeu fait pour un projet à l'**ESGI**, c'est un roguelike où on combat des poulets pour gagner des pièces.<br>
Ce projet utilise les libraires **SDL2**, **SDL2-image** et **SDL2-ttf**.

- [Roguelike C](#roguelike-c)
  - [Makefile](#makefile)
    - [Installation](#installation)
    - [Démarrer le jeu](#démarrer-le-jeu)
    - [Désintaller](#désintaller)
  - [Organisation](#organisation)
    - [Qui a fait quoi ?](#qui-a-fait-quoi-)
    - [Comment on s'est organisé ?](#comment-on-sest-organisé-)
  - [Organisation du code](#organisation-du-code)
  - [Fonctionnalités](#fonctionnalités)
  - [Les difficultés rencontrés](#les-difficultés-rencontrés)
  - [Lien avec notre formation](#lien-avec-notre-formation)

## Makefile

### Installation
Pour installer le jeu, faites les commandes suivantes:

```bash
user@debian: ~/Workspace/c$ sudo -S make install
user@debian: ~/Workspace/c$ make
```

### Démarrer le jeu
Et le jeu est prêt à être lancer avec la commande suivante:

```bash
user@debian: ~/Workspace/c$ ./prog
```

### Désintaller
Si vous voulez désintaller le jeu, faite la commande suivante:

```bash
user@debian: ~/Workspace/c$ sudo make uninstall
```

## Organisation

### Qui a fait quoi ?

| Jordan                               | Quentin                           | Oussoumane                           |
| ------------------------------------ | --------------------------------- | ------------------------------------ |
| Affichage de la map                  | Création du menu                  | Map aléatoire                        |
| Automatisation de la création de map | Code permettant d'afficher le PDF | Gestion du game over                 |
| Gestion des collisions               | Gestion du marchand               | Affichage de l'interface utilisateur |
| Gestion des combats                  | Graphismes: Création de map       | Changement de map                    |
| Gestion des mouvements ennemies      | Gestion de la bande son           | Gestion de la bande son              |
| Graphismes: Recherche des assets     |
| Création de l'object Map             |
| Gestion des évenements du joueur     |

### Comment on s'est organisé ?

Sessions quotidiennes en vocal sur **Discord** + **Live Share**.

## Organisation du code

L'organisation du code est faite de manière a contenir dans les dossier suivants:
- **assets/**: Ce dossier contient tout ce qui à trait aux graphismes du projet.
- **maps/**: Ce dossier contient les fichiers texte permettant de générer les différents map.
- **./**: Le reste du dossier contient les modules nécéssaires au fonctionnement du projet.

Le cours sur la programmation modulaire nous a permis de mieux organiser, et de structurer notre code en séparant les différents éléments dans des **modules**. De plus, le cours sur les **structure** nous à grandement aider, pour la gestion des personnages ainsi que pour la gestion de la map.

Nous avons aussi créer un Makefile, qui nous permet **d'installer / désinstaller** les librairies nécéssaires au bon fonctionnement du projet. En plus de pouvoir **compiler** notre programme, et de **nettoyer** le dossier.

## Fonctionnalités

Dans notre projet, nous proposons diverses **fonctionnalités**, telles que:
- Génération d'une map aléatoire.
- Interaction avec le marchand pour acheter une clé.
- Ouverture de coffre.
- Algorithme d'affichage des cartes.
- Système de combat.
- Déplacement du personnage avec les collisions.
- Changement de carte.

Nous avons utilisé les structures, et la programmation modulaire pour optimiser notre code.
Nous avons fait des fonctions pour rester **DRY**. (Don't Repeat Yourself)
## Les difficultés rencontrés

- Le manque de documentation sur **SDL2**.
- Libération de la mémoire lors du **changement** de map.

## Lien avec notre formation

L'expérience du projet annuel de 1ère année, nous a permis de mieux nous **organiser**, de ne pas attendre le dernier moment pour enfin commencer à se pencher sur le développement du projet. De plus les différents projets menés au sein de l'**ESGI** nous ont permis de se répartir les différentes tâches **efficacement** selon le niveau des membres de l'équipe.
