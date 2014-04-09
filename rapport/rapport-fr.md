WHITEBOX-DES
============

Ceci est une explication simplifié et compréhensible de l'article de [Chow et Al][1] sur les whitebox et leur implémentation d'une whitebox utilisant [DES][2].
Vous pouvez trouver l'implémentation en C illustrant cet article sur ce [même repo][3].

Note : Un article en version pdf se trouve aussi dans ce dossier, pour le build : `pdflatex rapport.tex`

[1]: http://
[2]: http://
[3]: http://

Définition
----------

White-Box Cryptography is an obfuscation
technique intended to implement cryptographic primitives in such a way, that
even an adversary who has full access to the implementation and its execution
platform, is unable to extract key information.
--Chow et al.nnn

Problème
--------

Un block-cipher (comme DES) couplé d'un [mode opératoire][1], représente généralement une **blackbox**. Les algorithmes sont définis publiquement, seule la clef est méconnue.

> A cryptosystem should be secure even if everything about the system, except the key, is public knowledge.

**Principe de Kerckhoffs**.

![Alice et Bob](alice_bob1.png)

L'attaquant possède plusieurs attaques (comme le Chosen Plaintext Attack) limitées par le fait qu'il ne possède généralement pas le programme d'encryption, ou qu'il ne possède pas le control de la machine sur lequel le programme tourne.

Le problème survient lorsque l'attaquant possède un total contrôle de la machine et de la mémoire de celle-ci. Il devient alors facile d'analyser le programme via des breakpoints, traces, ... et d'en extraire la clef.

[1]: http://

Utilité
-------

quelques exemples ici : http://summerschool08.iaik.tugraz.at/slides/Brecht_wbc1_crete_final.pdf

Quelles sont les cas où l'on s'écarte du modèle de la figure 1 et on laisse à l'attaquant potentiel un control total de la machine où le programme est exécuté ? Il en existe deux assez connus :

### DRM

le décodeur est sur le PC client.

### Mobile-Agent

Un programme qui va de serveur en serveur, qui peut parfois embarquer des données importantes comme des clefs pour pouvoir intérargir dans des protocoles cryptographiques.p

Solution
--------

C'est là qu'interviennent les **Whitebox**. Où l'on enfreint le principle de Kerchkhoffs et l'on cherche à rendre l'extraction de la clef plus compliquée via obfuscation, ou obscurcissement des procédés et des fonctions utilisés dans l'algorithme.

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
===================

/!\ **expliquer le pourquoi de chaque concept ! Sinon ca ne sert à rien de balancer un concept sans savoir pourquoi on l'utilise...**

voir l'article http://eprint.iacr.org/2013/455.pdf, ils parlent de 3 main steps: "Design Principles"
by : Another Nail in the Coffin of White-Box AES Implementation
Tancrède Lepoint and Matthieu Rivain

## Partial Evaluation.

Embed the key in an operation

## Tabularizing

Transforming all the components of the block cipher (even the linear
transformations) into look-up tables

## Randomization and Delinearization

f1(f2(f3))) =>
f1 = h(f1)
f2 = g(f2(h^-1))
f3 = f3(g^-1)

random bijections g and h




voir l'article de Chow et Al:

## Partial Evaluation and Look up tables

Puisque la clef est fixé, on remplace toutes les opérations qui sont previsibles lorsque la clef est connu. Commes les substitutions, par des look up tables.

## Mixing Bijections

A mixing bijection is a bijective at which attempts to maxi-mize the dependency 
of each output bit on all input bits. (Clearly, it is invertible and thei 
nverse is also an AT.)
In des, for example, the permutations, represented as AT s, have very sparse
matrices (one or two 1-bits per row or column). In order to diffuse information
over more bits, we can represent such a permutation P by JoK, where K is a 
mixing bijection and J=PK^⁻1, thereby replacing a sparse matrix with two
non-sparse ones (which is advantageous in subsequent de-linearizing encoding
steps)

## Input/output Encoding

## Combined Function Encoding

on combine deux fonctions en une fonction, par exemple les substitutions des rounds et le xor de la clef peuvent s'écrire en une fonction.

## By-Pass Encoding

Pour éviter qu'on puisse deviner à quel étape de l'algorithme on est, on aggrandit la taille de l'input que l'on passe à nos fonctions, et la taille de l'output, avec des bits inutiles.

## Split-Path Encoding



Implementation
==============
Look up tables
--------------
toutes les fonctions peuvent être écrite sous forme de Look up table, comment marchent les look up tables ?

2. les look up tables sont implémentés comme des Sbox (substitution), on prend le nombre crée par l'octet (000010 = 2) et on regarde l'entré numéro 2 dans la Tbox (pour ne pas confondre avec les Sbox de DES). Il y a donc 256 possibilités

Le pincipe est de calculer toutes les possibilités et de n'avoir qu'à accéder au Look up tables en utilisant l'input comme entrée de la table.

path splitting
--------------
pour ne pas avoir d'immenses Look up tables, on split le state de 96 bits en 12 octets et on crée 12 look up tables

Matrix
------
![matrix](http://i.imgur.com/xODKAA3.png)


De-linearization
----------------

"The delinearization step referred to by Chow et al.
and Jacob et al. prevents an adversary from viewing
the original contents of each table. Tables are delin-
earized by creating random permutations to rename
their contents; for example, the elements of a table
of 8-bit values would be renamed with a permuta-
tion of [0 . . . 28 − 1]. The inverse of this permutation
would be used to reindex the following table, which
would subsequently have its contents renamed. For
a system like DES, which can be represented entire-
ly with ATs and table lookups, this process can be
carried out on the entire implementation once the
ATs have been tabularized."
"We chose to divide input into 8-bit subvectors to match the
8-bit output of the T-boxes, and we divided the out-
put into 4-bit subvectors to keep the addition tables
from growing out of hand."
matrix of matrix" Link et al.


Exemple avec DES
================

DES
---

explication courte, voir notre algorithme sur github

M1
--

combines : 
1. la permutation initiale de DES
2. la premiere expansion

puis donne les bits dans le bon ordre aux look up tables de State1->State2.

c'est une matrice 64 par 96 (transforme 64 bits en 96 bits)


State 1 -> State 2
------------------

plein d'images à recup ici: http://www.iacr.org/workshops/fse2007/slides/rump/white.pdf

substitution + xor => lookuptable[round] precomputed with the key
Magie ! la clef vient de disparaitre.


Nous devons convertir ce fonctionement :

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

En celui-ci :

               *********************************************
   state 1:    *          state 1 (12 x 8 = 96 bits)       *
               *********************************************
                  |      |      |                       |
                  v      v      v                       v
               .-----..-----..-----.                 .-----.
               | T0  || T1  || T2  |       ...       | T11 |
               '-----''-----''-----'                 '-----'
                  |      |      |                       |
                  v      v      v                       v
               *********************************************
   state 2:    *              state 2 (96 bits)            *
               *********************************************

Pour ce faire, nous allons calculer 12 Look up Tables qui prendront 8 bits chacun ce qui recouvrira les 96 bits d'input.

Il y a :

8 look up tables non lineaires qui permettent le xor avec la clef et la substitution

4 look up tables lineaires qui nous serviront à by-passer les bits qui ne subissent pas de calculs.




STATE 2
-------

/!\ WHAT ?
Pour ne pas qu'on puisse comprendre que seule la partie gauche est change, on prend aussi la partie gauche et la partie droite inchange dans les calculs. le by-pass ! comme ca on ne sait pas ce qu'on fait dans cet etape

Voilà le fonctionnement initial qu'on va vouloir changer :

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

Commme pour l'étape précédente, nous voulons transformer les différentes opérations de cette étape en look up tables.

Pour ce faire, nous allons modéliser les différentes opérations en matrice puis la décomposer puisque la combinaison de la Permutation et du xor est une fonction affine.

Ces calculs vont être modélisé par M2 :

M_2 s'en fout de l'ordre dont les 12 look up tables lui donne les bits. n'importe quel ordre peut etre prit en compte dans M2.

M_2 est 96x96

M2 combine :
1. P-box transform (permutation)
2. xor avec Left
3. Expansion pour le prochain round (qui etait donne par M1 pour le premier round)
4. R(r) devient L(r+1)
5. extraction du vrai R(r)
6. bypass bits (a comprendre)


Pour le bypass on veut 64 bits. C'est pourquoi on utilise les Tbox lineaire dans le state 1

Pour utiliser un nombre satisfaisant de look up tables, nous allons devoir décomposer M2.

Par exemple, nous allons décomposer une matrice 16*16 en sous-matrices de 8x4. 

           .----.     .---------.---------.     .----.
           |    |     |    A    |    B    |     |    |
           | Y0 |     .---------.---------.     | X0 |
           |    |     |    C    |    D    |     |    |
           .----.  =  .---------.---------.  x  .----.
           |    |     |    E    |    F    |     |    |
           | Y1 |     .---------.---------.     | X1 |
           |    |     |    H    |    I    |     |    |
           '----'     '---------'---------'     '----'

Chaque sous-matrice va être la source d'une nouvelle look up table.

Cette look up table va être créée par le multiplication d'une sous-matrice avec toutes les possibilités d'input.

Elle aura donc 2^8 = 256 entrées possibles et 2^4 = 16 sorties possibles.

## M3

M_3 est 96x64

combine :

1. ignore les by-pass
2. recuperer L(16)
3. inverse la derniere expansion realise par M_2
4. echange L et R si l'on veut décoder (what ??? je quote "DES effectively swaps the left and right halves after the last round". On a pas fait ca dans notre implementation de DES oO).
5. Permutation finale

## Variante recommande

DES = M_4 ( DES ( M_0 ( INPUT ) ) )

en fait on remplace M_1 par M_1 ( M_0)
et M_3 par M_4 ( M_3)

# CONCLUSION

Kerckhoffs’ principle : security through obscurity
(keeping the design confidential) is bad practice

# SOURCES

## vidéos
https://www.youtube.com/watch?v=om5AVTqB5bA
White-Box Cryptography
Speaker: Dmitry Khovratovich
EventID: 5590
Event: 30th Chaos Communication Congress [30c3] by the Chaos Computer Club [CCC]
Location: Congress Centrum Hamburg (CCH); Am Dammtor; Marseiller Straße; 20355 Hamburg; Germany

## articles

"A White-box DES Implementation for DRM Applications", Chow et al.
      http://www.scs.carleton.ca/%7Epaulv/papers/whitedes1.ps
"White-Box Cryptography" (PhD thesis), B. Wyseur
      https://www.cosic.esat.kuleuven.be/publications/thesis-152.pdf
http://www.phrack.org/issues.html?issue=68&id=8#article
