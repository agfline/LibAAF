# LibAAF {#mainpage}

LibAAF is a C coded library for parsing **Advanced Authoring File Format**. It is released under the **GNU AGPLv3**.

The AAF specification can be found at https://www.amwa.tv/developers.shtml

LibAAF source code can be downloaded at https://github.com/agfline/LibAAF

LibAAF is composed of 3 parts : **LibCFB**, **AAFCore** and **AAFIface**.

---

## LibCFB

The LibCFB is a library written on this occasion, to access the low-level structure of the AAF.

The AAF is actualy built upon the Microsoft's *Compound File Binary File Format* (a.k.a *Structured Storage File Format*), which is closely related to the FAT file system, with its FAT table, Sectors and all. The LibCFB should only be accessed by the AAFCore, there's no need for the user to directly talk to it.

The specifications of the low-level structure can be found at https://www.amwa.tv/projects/MS-03.shtml

## AAFCore

Then like in any file system, we have access to a tree of nodes (directories and files), starting with the **Root Entry** node. Each directory is an Object representing an AAF Class. Each directory has a file called **properties** that holds the members of the Class. A Class can inherit other Classes members.

Some special file nodes named with the suffix **index** hold either a StrongReferenceSet or a StrongReferenceVector. Those are lists of references to other nodes name with the same prefix and with the suffix ***{n}***, ***n*** being an index number. The prefix of the name is hold by a Class member. The only difference between Sets and Vectors is that Set Objects are unordered but can be referenced by an unique ID, while Vectors Objects are ordered and can only be referenced by their index number.




