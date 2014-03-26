WHITEBOX-DES
============

Ceci est une explication simplifié et compréhensible de l'article de [Chow et Al][1] sur les whitebox et leur implémentation d'une whitebox utilisant [DES][2].
Vous pouvez trouver l'implémentation en C illustrant cet article sur ce [même repo][3].

Note : Un article en version pdf se trouve aussi dans ce dossier, pour le build : `pdflatex rapport.tex`

[1]: http://
[2]: http://
[3]: http://

Utilité
-------

Explication de DRM.


Problème
--------

Un block-cipher (comme DES) couplé d'un [mode opératoire][4], représente généralement une **blackbox**, les algorithmes sont définis publiquement.

> A cryptosystem should be secure even if everything about the system, except the key, is public knowledge.

**Principe de Kerckhoffs**.

L'attaquant possède plusieurs attaques (comme le Chosen Plaintext Attack) limitées par le fait qu'il ne possède généralement pas le programme d'encryption, ou qu'il ne possède pas le control de la machine sur lequel le programme tourne.

Le problème survient lorsque l'attaquant possède un total contrôle de la machine et de la mémoire de celle-ci. Il devient alors facile d'analyser le programme via des breakpoints, traces, ... et d'en extraire la clef.

[4]: http://

Solution
--------

C'est là qu'interviennent les **Whitebox**. Ou l'on enfreint la règle principale et l'on cherche à rendre l'extraction de la clef compliqué (rarement impossible) via obfuscation, ou obscurcissement des procédés et des fonctions utilisés dans l'algorithme.

Il faut alors complètement modifier les algorithmes, ce qui donne ce genre de schèma (tiré de l'article de Phrack)


    *********************************************
    *              state 2 (96 bits)            *
    *********************************************
       |      |      |                       |
       v      v      v          ...          v
    ?????????????????????????????????????????????
       |      |      |          ...          |
       v      v      v                       v
    *********************************************
    *              state 3 (96 bits)            *
    *********************************************


Concepts
========

/!\ **expliquer le pourquoi de chaque concept ! Sinon ca ne sert à rien de balancer un concept sans savoir pourquoi on l'utilise...**

voir l'article de Chow et Al:

Partial Evaluation and Look up tables
-------------------------------------

Puisque la clef est fixé, on remplace toutes les opérations qui sont previsibles lorsque la clef est connu. Commes les substitutions, par des look up tables.

Mixing Bijections
-----------------

Input/output Encoding
---------------------

Combined Function Encoding
--------------------------

on combine deux fonctions en une fonction, par exemple les substitutions des rounds et le xor de la clef peuvent s'écrire en une fonction.

By-Pass Encoding
----------------

Pour éviter qu'on puisse deviner à quel étape de l'algorithme on est, on aggrandit la taille de l'input que l'on passe à nos fonctions, et la taille de l'output, avec des bits inutiles.

Split-Path Encoding
-------------------


Implementation
==============

Look up tables
--------------

toutes les fonctions peuvent être écrite sous forme de Look up table, comment marchent les look up tables ?

2. les look up tables sont implémentés comme des Sbox (substitution), on prend le nombre crée par l'octet (000010 = 2) et on regarde l'entré numéro 2 dans la Tbox (pour ne pas confondre avec les Sbox de DES). Il y a donc 256 possibilités

path splitting
--------------

pour ne pas avoir d'immenses Look up tables, on split le state de 96 bits en 12 octets et on crée 12 look up tables

Matrix
------

[Link et al : Clarifying Obfuscation: Improving the Security of White-Box Encoding][1]

![matrix](https://github.com/mimoo/whiteboxDES/blob/master/rapport/images/matrices.PNG?raw=true)

[1]: http://eprint.iacr.org/2004/025.pdf

Exemple avec DES
================

DES
---

explication courte, voir notre algorithme sur github

M_1
---

combines : 
1. la permutation initiale de DES
2. la premiere expansion

puis donne les bits dans le bon ordre aux look up tables de State1->State2.

c'est une matrice 64 par 96 (transforme 64 bits en 96 bits)

STATE 1
-------

substitution + xor => lookuptable[round] precomputed with the key
Magie ! la clef vient de disparaitre.

                 32b               48b              16b
               ************** ********************* ********
    state 1:   *     L(r)   * *       X(r)        * * r(r) *
               ************** ********************* ********
                     |                |      |         |
                     |                v      |         |
                     | *********    .....    |         v
                     | * sK(r) *--> . + .    |    .-------.
                     | *********    .....    '-->(  Merge  )
                     |                |           '-------'
                     |                v               |
                     |         .-------------.        |
                     |          \     S     /         |
                     |           '---------'          |
                     |                |               |
                32b  v                v 32b     32b   v
               ************** *************** ***************
    state 2:   *    L(r)    * *    Y(r+1)   * *     R(r)    *
               ************** *************** ***************

State 1 -> State 2
------------------

8 look up tables (non lineaire ?)

4 look up tables lineaire qui nous serviront pour la suite.


STATE 2
-------

Pour ne pas qu'on puisse comprendre que seule la partie gauche est change, on prend aussi la partie gauche et la partie droite inchange dans les calculs. le by-pass ! comme ca on ne sait pas ce qu'on fait dans cet etape

               ************** *************** ***************
    state 2:   *    L(r)    * *    Y(r+1)   * *     R(r)    *
               ************** *************** ***************
                     |                  |           |
                     v                  |           |
                   .....    .--------.  |           |
                   . + .<---|    P   |<-'           |
                   .....    '--------'              |
                    |                               |
                32b '----------------------------------.
                                        |           |  |
                    .-------------------|-----------'  |
                    |               32b v              v 32b
                    |               .-------.       .------.
                    |              /  E-box  \     ( Select )
                    |  32b        '-----------'     '------'
                    |                   |              |
                    v               48b v              v 16b
               ************** ********************* ********
    state 3:   *   L(r+1)   * *       X(r+1)      * *r(r+1)*
               ************** ********************* ********

M_2
---

M_2 s'en fout de l'ordre dont les 12 look up tables lui donne les bits. n'importe quel ordre peut etre prit en compte dans M2.

M_2 est 96x96

M2 combine :
1. P-box transform (permutation)
2. xor avec Left
3. Expansion pour le prochain round (qui etait donne par M1 pour le premier round)
3. R(r) devient L(r+1)
4. extraction du vrai R(r)
5. bypass bits (a comprendre)

De-linearization

matrix of matrix

pour le bypass on veut 64 bits. C'est pourquoi on utilise les Tbox lineaire dans le state 1


M_3
---

M_3 est 96x64

combine :

1. ignore les by-pass
2. recuperer L(16)
3. inverse la derniere expansion realise par M_2
4. echange L et R (what ??? je quote "DES effectively swaps the left and right halves after the last round". On a pas fait ca dans notre implementation de DES oO).
5. Permutation finale

Variante recommande
-------------------

DES = M_4 ( DES ( M_0 ( INPUT ) ) )

en fait on remplace M_1 par M_1 ( M_0)
et M_3 par M_4 ( M_3)

CONCLUSION
==========

Kerckhoffs’ principle : security through obscurity
(keeping the design
confidential) is bad practice

SOURCES
=======

/!\ **Mieux vaudrait indiquer les sources au fur et à mesure dans l'article! C'est moche plein de sources à la fin**

Vidéos
------

https://www.youtube.com/watch?v=om5AVTqB5bA
White-Box Cryptography
Speaker: Dmitry Khovratovich
EventID: 5590
Event: 30th Chaos Communication Congress [30c3] by the Chaos Computer Club [CCC]
Location: Congress Centrum Hamburg (CCH); Am Dammtor; Marseiller Straße; 20355 Hamburg; Germany

Articles
--------

"A White-box DES Implementation for DRM Applications", Chow et al.
      http://www.scs.carleton.ca/%7Epaulv/papers/whitedes1.ps
"White-Box Cryptography" (PhD thesis), B. Wyseur
      https://www.cosic.esat.kuleuven.be/publications/thesis-152.pdf
http://www.phrack.org/issues.html?issue=68&id=8#article