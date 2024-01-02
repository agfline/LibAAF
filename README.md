# LibAAF
> [!WARNING]
>
> While major version number remains 0, API MUST NOT be considered stable. Minor version number should reflect API changes during that time.
>
> Releases are made everytime libaaf code is updated into Ardour source tree.


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
* ProTools 10.3.10 - 2023.12
* Logic Pro 9.1.7 - 10.7.4
* Davinci Resolve 17.4.6 - 18.5
* Fairlight Evo/Dream 2.5.1 - 4.1.75


|                                                |   |                                        |
|------------------------------------------------|:-:|----------------------------------------|
| Composition Name                               | x | *Full support*                         |
| Track Names                                    | x | *Full support*                         |
| Clip Names                                     | x | *built upon source file names*         |
| Original essence file names                    | x | *Full support*                         |
| PCM Embedded Essences                          | x | *Full support*                         |
| WAVE Embedded Essences                         | x | *Full support*                         |
| WAVE External Essences                         | x | *Full support*                         |
| AIFF Embedded Essences                         | x | *Full support*                         |
| AIFF External Essences                         | x | *Full support*                         |
| AES3 Embedded Essences                         | - | *Missing from the specs / Not encountered yet* |
| MXF External Essences                          | x | *Full support*                         |
| Multi-channel clip out of a multi-channel file | x | *Full support*                         |
| Multi-channel clip out of multiple mono files  | x | *Full support*                         |
| Fades in/out - XFades                          | x | *Full support*                         |
| Legacy Fades in/out - XFades                   | - | *useless ? obsolete since at least AAFv1.1 - 2005* |
| Clip based Gain (fixed/varying)                | x | *Full support*                         |
| Track based Gain (fixed/varying)               | x | *Full support*                         |
| Track based PAN (fixed/varying)                | x | *Full support*                         |


## Build

```
cd build
cmake ..
make
```

## Ardour Support

LibAAF was added to Ardour version 8.2
