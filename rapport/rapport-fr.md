# INTRODUCTION WHITEBOX

## utilité 

How to build the rapport: `pdflatex rapport.tex`

explication de DRM

## problème

explication de comment recup la clef dans la mémoire

## solution

whitebox

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

# EXPLICATION CONCEPT

expliquer le pourquoi de chaque concept ! Sinon ca ne sert à rien de balancer un concept sans savoir pourquoi on l'utilise...

## Partial Evaluation and Look up tables

Puisque la clef est fixé, on remplace toutes les opérations qui sont previsibles lorsque la clef est connu. Commes les substitutions, par des look up tables.

## Mixing Bijections

## Input/output Encoding

## Combined Function Encoding

on combine deux fonctions en une fonction, par exemple les substitutions des rounds et le xor de la clef peuvent s'écrire en une fonction.

## By-Pass Encoding

Pour éviter qu'on puisse deviner à quel étape de l'algorithme on est, on aggrandit la taille de l'input que l'on passe à nos fonctions, et la taille de l'output, avec des bits inutiles.

## Split-Path Encoding



# EXPLICATION DE COMMENT LES IMPLEMENTER

## Look up tables

toutes les fonctions peuvent être écrite sous forme de Look up table, comment marchent les look up tables ?

# EXEMPLE AVEC DES

## DES

explication courte, voir notre algorithme sur github

## M1

## STATE 1

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

## Look up tables


## STATE 2

pour ne pas qu'on puisse comprendre que seule la partie gauche est change, on prend aussi la partie gauche et la partie droite inchange dans les calculs. le by-pass ! comme ca on ne sait pas ce qu'on fait dans cet etape

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

## M2

## M3


# CONCLUSION

Kerckhoffs’ principle : security through obscurity
(keeping the design
confidential) is bad practice

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