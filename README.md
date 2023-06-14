# LibAAF

> *LibAAF is currently a work in progress and is not production-ready yet.*


LibAAF is a C coded library for AAF file handling. The AAF beeing a quite complex file format, the main goal here is to get an easy-to-implement, audio oriented, FOSS solution.


The library is composed of three parts :

* **LibCFB** : Low-level AAF's *Compound File Binary* handling
* **AAFCore** : AAF Class/Object manipulations
* **AAFIface** : Abstraction layer for easy data access.

One should only use the **AAFIface** interface to manipulate files and retrieve data.

## Support

LibAAF only supports the reading of AAF files, it cannot write ones.

LibAAF was widely tested with :

* Avid Media Composer 8.4.5 - 8.8.3 - 18.12.5
* Adobe Premiere Pro (CC 2018) 12.0 - 12.1.1
* ProTools 10.3.10
* Logic Pro 9.1.7
* Fairlight Evo/Dream 2.5.1 - 4.1.75


|                                         |   |                                        |
|-----------------------------------------|:-:|----------------------------------------|
| Composition Name                        | x | *Full support*                         |
| Track Names                             | x | *Full support*                         |
| Clip Names                              | x | *built upon source file names*         |
| Original essence file names             | x | *Full support*                         |
| PCM Embedded Essences                   | x | *Full support*                         |
| WAVE Embedded Essences                  | x | *Full support*                         |
| AIFF Embedded Essences                  | x | *Full support*                         |
| AES3 Embedded Essences                  | - | *Missing from the specs*               |
| MXF support                             | - | **TODO**                               |
| Multi-channel Essences                  | - | **TODO** (*AAFClassID_MultipleDescriptor*) |
| Multi-channel Tracks (stereo, 5.1, 7.1) | x | *ProTools AAF with **Export Stereo, 5.1 and 7.1 tracks as multi-channel** enabled.* |
| Fades in/out - XFades                   | x | *Full support*                         |
| Legacy Fades in/out - XFades            | - | *useless ? obsolete since at least AAFv1.1 - 2005* |
| Clip based Gain (fixed/varying)         | x | *Full support*                         |
| Track based Gain (fixed/varying)        | x | *Full support*                         |
| Track based PAN (fixed/varying)         | x | *Full support*                         |

<!-- ## Tools

Two programs are available with LibAAF :

* **AAFInfo** : Parses any AAF file and provides a way to dig into them.
* **AAFExtract** : Extracts audio essences contained in an AAF file.

To build those, just run `make` -->

## Build

```
cd build
cmake ..
make
sudo make install
```

## Ardour Support

There is and [Ardour fork](https://github.com/agfline/ardour_aaf_support) that includes an experimental [tool](https://github.com/agfline/ardour_aaf_support/tree/master/session_utils/new_aaf_session.cc) to create ardour sessions based on AAF files.

You can build it using the regular ardour compilation process.

Building this tool requires libaaf to be installed.
