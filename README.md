# LibAAF

> *LibAAF is currently a work in progress and is far from beeing production ready.*


**LibAAF** is a C coded library for AAF file handling. The AAF beeing a quite complex file format, the main goal here is to get an easy-to-implement FOSS solution.


The library is composed of three parts :

* **LibCFB** for the low-level AAF's *Compound File Binary* handling
* **AAFCore** for AAF Class/Object manipulations
* **AAFIface** for easy data access.

Since AAF files can store the same data in many different ways, the **AAFIface** interface is there to simplify the way those data are presented to the user. Therefore, for standard AAF operations like essences retrieval, the user should only use the **AAFIface**'s functions.

Although **AAFCore** should implement the AAF standard correctly, **AAFIface** currently only supports parsing of audio data and essences. As a second step, it should be able to handle video too.

Finaly, LibAAF can only read files for now. In the end it should be able to write files too.

## Tools

There are three programs to help the developement and to provide library usage examples.

* **AAFInfo** : Allows to parse any AAF file and provides a way to dig into them.
* **AAFExtract** : Allows to extract the essences contained in the AAF file.
* **AAF2Ardour** : A very dirty proof of concept which allows to create an Ardour session from an AAF file.

## Compile

LibAAF makes use of no particular library. 

You can just run `make`

## Contribute

Since AAF was made to handle any data in any ways, sometimes even one single software outputs the same data differently on two exports. That's why there are a lot of AAF incompatibility between softwares and software versions.

Therefore we need a lot of material, despite the standard specifications, to implement all those cases and make LibAAF compatible with as much softwares as possible.

Currently, LibAAF was widely tested with :

* Avid Media Composer
* Fairlight (poorly implemented, by the way..)

**So feel free to send any AAF file you have from any software, this will be extremely useful.** Espacialy with audio segment's gain, pan, track's gain, pan, volume.
