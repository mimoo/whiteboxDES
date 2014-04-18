whiteboxDES
===========

This is an explanation of How Whitebox can be created,
It follows [Chow et al's paper][1] on the subject.

.tex of rapport and slides are in  `rapport/`.

the pdf version of the rapport is [here][2].

The slides of our presentation are [here][7]

You can also browse `src/` to see the whitebox we build using C and [DES][4].

It uses our previous implementation of [DES in C][5].

[1]: http://www.scs.carleton.ca/%7Epaulv/papers/whitedes1.ps
[2]: https://github.com/mimoo/whiteboxDES/blob/master/rapport/rapport.pdf
[4]: http://en.wikipedia.org/wiki/Data_Encryption_Standard
[5]: https://github.com/mimoo/DES
[7]: https://github.com/mimoo/whiteboxDES/blob/master/rapport/slides.pdf


Author
------

* David Wong
* Hugo Bonnin
* Jacques Monin
 
To Do List
----------

* Create encodings for the network of XOR tables.
* Split-Pathing for the XOR tables
* Mixing-bijections for M_0, M_1, M_2 and

How to use the C implementation
-------------------------------

* **WBDES.c** and **WBDES.h** contain the functions used in the Whitebox-DES algorithm.

* **main.c** builds the algorithm and allows you to decrypt an input file using the Whitebox DES. To encrypt you can use a simple DES encryption.

use make to build desbox.

    $ make
    
    $ whitebox --help

* **LtablesGen.c** is a Lookup table generator that takes a key and generate a `LtablesGen` executable. Use it before building the main executable

use `make` to build the lookup table generator:

    $ make Ltablesgen

    $ ./Ltablesgen key

key must be in binary format.

Tools used
----------

* [github][g]
* [trello][t]
* [flowdock][f]
* [penflip][p]

[g]: http://www.github.com
[t]: http://www.trello.com
[f]: http://www.flowdock.com
[p]: https://www.penflip.com/
