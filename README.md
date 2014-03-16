whiteboxDES
===========

This is an implementation in C of a Whitebox using [DES][1] (Data Encryption Standard)  

It follows [Brecht Wyseur's Phd thesis][2] on the subject.

It uses our previous implementation of [DES in C][3].

[1]: http://en.wikipedia.org/wiki/Data_Encryption_Standard
[2]: https://www.cosic.esat.kuleuven.be/publications/thesis-152.pdf
[3]: https://github.com/mimoo/DES


Author
------

* David Wong
* Hugo Bonnin
* Jacques Monin

Files
----

* **WBDES.c** and **WBDES.h** contain the functions used in the Whitebox-DES algorithm.

* **main.c** builds the algorithm and allows you to encrypt/decrypt an input file using the Whitebox DES.

use make to build desbox.

    $ make
    
    $ whitebox --help

* **LtablesGen.c** is a Lookup table generator that takes a key and generate a `LtablesGen` executable. Use it before building the main executable

use `make` to build the lookup table generator:

    $ make ltablesgen

    $ ./ltablesgen

