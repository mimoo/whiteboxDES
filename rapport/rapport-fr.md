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

## Partial Evaluation and Look up tables

Puisque la clef est fixé, on remplace toutes les opérations qui sont connu lorsque la clef est connu. Commes les substitutions, par des look up tables.

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