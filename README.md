# LibAAF

> *LibAAF is currently a work in progress and is not production-ready yet.*


LibAAF is a C coded library for AAF file handling. The AAF beeing a quite complex file format, the main goal here is to get an easy-to-implement, audio oriented, FOSS solution.


The library is composed of three parts :

* **LibCFB** : Low-level AAF's *Compound File Binary* handling
* **AAFCore** : AAF Class/Object manipulations
* **AAFIface** : Abstraction layer for easy data access.

For standard AAF operations like essences/tracks/clips retrieval, the user should only use the **AAFIface**'s functions.

## Support

LibAAF only supports the reading of AAF files, it cannot write ones.

LibAAF was widely tested with :

* Avid Media Composer 8.4.5 - 8.8.3
* ProTools 10.3.10
* Logic Pro 9.1.7
* Fairlight Evo 2.5.1 - 4.1.75


|                                 |   |                                    |
|---------------------------------|:-:|------------------------------------|
| Composition Name                | x | *Full support*                     |
| Track Names                     | x | *Full support*                     |
| Clip Names                      | x | *Retrieved from source file names* |
| Original essence file names     | x | *Full support*                     |
| PCM Embedded Essences           | x | *Full support*                     |
| WAVE Embedded Essences          | x | *Only PCM audio*                   |
| AIFF Embedded Essences          | x | *Only PCM audio*                   |
| AES3 Embedded Essences          | - | *Missing from the specs*           |
| Multi-channel Essences          | - | *TODO*                             |
| Fades in/out - XFades           | x | *Full support*                     |
| Legacy Fades in/out - XFades    | - | *TODO*                             |
| Clip based Gain (single value)  | x | *Full support*                     |
| Clip based Gain (automation)    | x | *Full support*                     |
| Track based Gain (single value) | - | *TODO*                             |
| Track based Gain (automation)   | - | *TODO*                             |
| Track based PAN                 | - | *TODO*                             |

## Tools

Three programs are available with LibAAF :

* **AAFInfo** : Allows to parse any AAF file and provides a way to dig into them.
* **AAFExtract** : Allows to extract the essences contained in the AAF file.
* **AAF2Ardour** : A very dirty proof of concept which allows to create an Ardour session from an AAF file.

## Compile

LibAAF has no particular dependencies.

Just run `make`
