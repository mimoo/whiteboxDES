whiteboxDES
===========

This is an explanation of How Whitebox can be created,
It follows [Chow et al's paper][1] on the subject.

To see it, browse `rapport/`. You can find a Markdown version in [french][2] or [english][3].

And a LaTeX/pdf version in [french][6].

You can also browse `src/` to see the example we build using C and [DES][4].

It uses our previous implementation of [DES in C][5].

[1]: http://www.scs.carleton.ca/%7Epaulv/papers/whitedes1.ps
[2]: https://github.com/mimoo/whiteboxDES/blob/master/rapport/rapport-fr.md
[3]: https://github.com/mimoo/whiteboxDES/blob/master/rapport/rapport-en.md
[4]: http://en.wikipedia.org/wiki/Data_Encryption_Standard
[5]: https://github.com/mimoo/DES
[6]: https://github.com/mimoo/whiteboxDES/blob/master/rapport/rapport.tex


Author
------

* David Wong
* Hugo Bonnin
* Jacques Monin

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

* [github][1]
* [trello][2]
* [flowdock][3]
* [penflip][4]

[1]: http://www.github.com
[2]: http://www.trello.com
[3]: http://www.flowdock.com
[4]: https://www.penflip.com/
