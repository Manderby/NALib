
NALib Readme
This file is part of NALib, a collection of C and C++ source code
intended for didactical purposes. Full license notice at the bottom.
====================


Table of Content:
-----------------
1.   Short Introduction
2.   Creation and deletion of memory and structs
2.3    DEPRECATED - Callbacks for container structs
3.   Runtime System
4.   C90 variable declarations
5.   Inline implementations
6.   Opaque types
7.   Symbol naming
8.   Default header and footer




1. Short Introduction
---------------------
- Simply include the whole NALib folder into your project.
- NALib just needs the standard libraries.
- Any documentation can be found within the source files or in this file.

If you are curious on how NALib works, start at the most basic files:
- NAPointer.h     Functions to work with pointers
- NASystem.h      Functions, Macros and Definitions, System-Dependancy
- NABinaryData.h  Memory manipulations

For more information, visit
http://manderc.com/nalib/index.php

Have fun, Tobias Stamm.






2 Creation and deletion of memory and structs
---------------------------------------------
You can use the default malloc and free functions to create memory blocks
as you like. NALib nontheless provides its own allocation methods (which are
basically mappings to malloc and free):

- naMalloc            Allocates the given number of bytes.
- naMallocIfNull      Same as naMalloc but the space will only be allocated
                      if the pointer provided is NA_NULL.
- naMallocPageAligned Allocates the given number of bytes but the first byte
                      will be aligned to a page boundary.
- naAlloc             Allocates space for the given NALib structure. Use it
                      for example like this: NAArray* a = naAlloc(NAArray);
- naFree              Frees the memory allocated with above functions.


Additionally, starting with NALib version 10, you can use the following
two functions (See section 3 for more information):

- naNew:        Uses the NARuntime system to quickly provide a pointer.
- naDelete:     Deletes a pointer allocated with naNew.


Other than these basic allocation methods, NALib offers creation and deletion
functions for many structs.

The naming scheme guides you:


- naMakeXXX:    Returns the struct as a value. Only very basic structs (pod)
                will provide such functions.
- naFillXXX:    Expects the first argument to be a pointer to the struct.
                Again, only basic structs (pod) will offer such functions.
- naInitXXX:    Expects a pointer to an existing block in memory. Returns the
                pointer again. See section 2.1 for more information.
- naClearXXX:   Expects a pointer to a struct in which the memory shall be
                whiped. The pointer itself is not deallocated though. See
                section 2.2 for more information.
- naNewXXX:     Starting with NALib version 10, certain structs like NAString
                have been restricted to be managed by the runtime system. These
                structs do not provide naInitXXX_functions but naNewXXX-
                functions instead.


Simple structs like NAPos are very basic structs and are generally called
pod (plain old data). Such structs do not need special care when creating
or deleting them. But more complex structs do.

Complex structs must be initialized before being able to use them and must
be destroyed when not needed anymore. As NALib is a C library, there are no
constructors or destructors like in typical OOP languages and consequently,
such initialization must be done manually by the programmer.

In NALib, construction-functions always start with naInitXXX and desctruction-
functions always start with naClearXXX. naInitXXX-functions usually assume
the given struct to be uninitialized and will overwrite all values. naClearXXX-
functions expect a pointer to an initialized struct and will perform all
necessary destructions. Beware that naClearXXX will only erase the contents but
not the pointer itself.

The struct itself must always be allocated outside of the naInitXXX
function. According to the functions listed in section 1, you have the
following scenarios:


A) Use the pointer to an existing struct:
NAArray myarray;
naInitArray(&myarray);
naClearArray(&myarray);


B) Allocate with malloc, naMalloc or naAlloc and delete with free or naFree:
NAArray* myarray1;
NAArray* myarray2;
NAArray* myarray3;
myarray1 = naInitArray(malloc(sizeof(NAArray)));
myarray2 = naInitArray(naMalloc(sizeof(NAArray)));
myarray3 = naInitArray(naAlloc(NAArray));
naClearArray(myarray1);
naClearArray(myarray2);
naClearArray(myarray3);
free(myarray1);
naFree(myarray2);
naFree(myarray3);


C) Allocate with naNew and delete with naDelete:
NAArray* myarray;
myarray = naInitArray(naNew(NAArray));
naDelete(myarray);


The cases listed here are usual scenarios. But in certain cases, a programmer
might want to choose differently, for example to save time. If you choose so,
just make sure you know what you are doing.

Do not use the free-functions on stack variables!






2.3 Callbacks for container structs
-----------------------------------
-- LEGACY AND DEPRECATED - WILL BE REMOVED IN ONE OF THE NEXT VERSIONS --
Some container structs like NAArray or NAGrowingSpace provide naCreateXXX,
naClearXXX and naDestroyXXX functions with an additional parameter:
A constructor or destructor callback with the following signature:

typedef void* (*NAConstructor)(void *);
typedef void  (*NADestructor) (void *);

The parameter is usually called "constructor" or "destructor" and allows
you to provide a function pointer to a construction or destruction function
which will be called for every element in the container. Of course, this is
only necessary if your elements actually need some sort of construction or
destruction.

Creating a datastructure with a constructor should always! be paired with
clearing or destroying the datastructure with the corresponding desctructor.

If you just need to do some harmeless initialization and no real construction
(memory allocation and such), you might want to create the struct plainly and
then run a foreach-function which accepts a mutator callback:

typedef void  (*NAMutator) (void *);

Beware: Do not mix these function pointers!

Constructor example: You store complex objects in a NAGrowingSpace whereas
each object must perform certain allocations before it can properly be
used. The NAGrowingSpace structure will call the appropriate constructor
for all new elements whenever the space grows.

Destructor example: You store complex objects in an NAArray whereas the fields
of the objects need to be deallocated properly upon destruction, you do not
have to iterate over the whole array and call your destructor manually for
every element. All the proper destruction of your elements will be taken care
of.

You are nontheless free to use these callbacks. When the according parameter
is NA_NULL, no construction or destruction will be performed on the single
elements. Note that when using the callback argument, your element constructor
or destructor will be called by a function call which can be very costly for
a lot of small elements.

IMPORTANT:
Beware that your constructor or destructor will always be called with a
POINTER to the stored content. If for example, you have an array of integers,
your callback will get an "int *". If your array stores a pointer to int,
your callback will get an "int* *".

The return value of the constructor callback will be ignored.

In order to provide a clean API, the input- and return-parameter of your
callback can be a pointer to any type you desire. But you must properly
cast the function pointer when providing it to the naCreateXXX, naDestroyXXX
or naClearXXX function with (NAConstructor) or (NADesctuctor). If you provide
an incompatibel function pointer type, the implementation will likely crash.
Especially note that attributes like __fastcall will not work!

Note that you can also use any of the functions of NALib as callback
functions as long as they only accept the correct parameters. Choose the
correct one! And beware the pointer!





3 Runtime System
-----------
Starting with NALib version 10, there exists a small Runtime System which can
be used if desired. Currently, it mainly provides inifinitely large pools for
NALib structures for quick allocation and pointer management.

The author decided to add such a system because of the NAString struct which
becomes way easier to manage when strings are always provided as pointers.
Therefore, the naInitString-functions have been replaced by naNewString-
functions which do not accept an input pointer anymore but instead always
return a pointer allocated with naNew.

Warning: NALib version 10 only provides the runtime system for NAString and
NAPointer.

In order to manage the pointers, an instance of NARuntime must be created
before any naNew function can be called. When NDEBUG is undefined, a friendly
warning tells you to start the runtime system. You can start it by a simple
call to

naStartRuntime();

When you no longer need the runtime system, you can stop it using

naStopRuntime();

The runtime system will be enhanced in future versions to other NALib structs
and probably even custom types. Other enhancements of the runtime system may
be possible like deallocation pools or even some sort of garbage collection
or (in the far future) just-in-time compilation.





4. C90 variable declarations
----------------------------
Sometimes in the code, you will find some declarations commented with the
following sentence:

Declaration before implementation. Needed for C90

This has been introduced due to the standard C90 which paritally still is
used by some compilers. This standard states that each variable MUST be
declared before it is used. Later standards and most modern compilers do not
really need this anymore but when you try to compile a static or dynamic
library on certain compilers, you will most probably run into the problem
that the declarations must be present.

NALib is prepared for that. So every variable is always declared before
any use in any implementation.

In more modern compilers, you would declare the variables at the position
where they are first used or are otherwise meaningfully positioned in an
appropriate way.





5. Inline Implementations
-------------------------
Unfortunately, inline- and template-functions must be present to the compiler
during compilation. This means, every declaration must be followed by a
definition in the same file scope.

One way to achieve this is to simply make no declarations at all but instead
implement every function directly in the header file. This makes it hard though
to create a useful interface. Header files do have (among other things) the
purpose of providing an API to the programmer. And such an API preferably is
written in a compact, readable way.

Another way is to write declarations and move all definitions to the end of a
header file and hence keep separate sections: API above, implementation below.

Yet another way is to copy all implementation code to a separate file and
include that file at the end of the header file. Some suffixes known are
.hpp .ii .inl .tpl .hxx. But there is no standard.

The author has decided to include them at the end of the header files. This
makes the files longer but everything is in one place, the IDEs have no
trouble finding the symbols and the debugger works well as well.




6. Opaque Types
---------------
All around NALib you find declarations like these:

typedef struct NAArray NAArray;

This line tells the compiler that NAArray shall be used as a type. After this
line, the compiler expects a declaration somewhere and allows the programmer to
use the type by just using its name. The actual declaration of this type can
be written anywhere but in NALib is always given immediately after that line
by the following lines starting with something like

struct NAArray{
  ...
}

These lines usually should be written in an implementation file corresponding
to the type. Doing so, they become opaque to the user after compilation,
meaning the data organisation is unknown.

This normally is a good thing. For end products, this is a desirable behaviour,
such that reengineering or misuse of undocumented functionality becomes much
more difficult. The downfall though is that even the programmers have no access
to the declaration. The type defined by the typedef-line can only be used as
pointer or by a specific set of API calls given by the programmer of the type.

It is no problem to store pointers to any type in C and C++ but storing and
accessing the type itself as a value or subpart of another type requires the
compiler to know the exact declaration of that type. Other modern languages
like Java or C# do not suffer from this limitation as basically, every type
is accessed by a pointer. The potential of C and C++ though gets limited
fundamentally, as types can not contain reference-types whose declaration is
unknown.

To not limit the functionality, all types are directly declared after its
typedef. This allows NALib to inline lots of code and hence become very fast.
The programmer though has to take the responsibility to not access any of the
fields of an opaque type directly. Always use the API!

Note that if you want to create a library where the types become opaque but
can nontheless be accessed by your own implementation, collect all opaque type
declarations into one header file which will be included by the implementation
files and must be present during compilation. During distribution, remove that
header file from the package. It's tricky but it works.






7. Symbol naming
----------------
In NALib, a (more or less) consistent naming scheme is applied. Every symbol
consists at least of a suffix and an identifier.

Suffix:
- types always have the prefix      NA    and are MixedCase
- macros always have the prefix     NA_   and are UPPERCASE
- functions always have the prefix  na    and are MixedCase
- variables always have the prefix  na_   and are lowercase

Types are always typedef'd, meaning you won't have to write the struct keyword
all the time when declaring variables. Also enums are typedef'd with a clear
name. Examples: NAInt, NAArray, NATextEncoding

Some macros have just one identifier after the prefix. These macros denote
key values often used in all of the source code. Examples: NA_NULL, NA_RESTRICT

Some other macros have multiple identifiers after the prefix. The first
identifier denotes the topic which the macros are used for. The remaining
identifiers are a more and more defined description of what the macro describes.
Examples: NA_SYSTEM_WINDOWS, NA_ENDIANNESS_BIG

Functions have a description of what they do after the prefix. The first word
after the prefix is usually a verb, such as Convert, Create or Get. After that
comes usually a type upon which the function operates. For example Array or
String but also something like i8 or 32, denoting an integer with 8 bits or an
array with 32 bits. After that, a more concrete description of what the function
does or expects as parameters is given. Like for example WithSize, FromFilename.

Variables and printf arguments are rarely used in NALib. They have an
appropriate description after the prefix.




8. Default header and footer
----------------------------
At the top and at the bottom of each file, a short comment and license notice
about NALib can be found.

Aside from that, all header files are encapsulated by standard include-once
directives:

#ifndef NA_XXX_INCLUDED
#define NA_XXX_INCLUDED
...
#endif // NA_XXX_INCLUDED

Which allows you to simply include all header files as many times as you want
without bumping into redefinitions.

Further more, all header files are encapsulated by the standard c-linkage
keyword:

#ifdef __cplusplus 
  extern "C"{
#endif
...
#ifdef __cplusplus 
  } // extern "C"
#endif

Which allows you to include all header files both in C and C++ without linker
errors.




====================
Copyright (c) NALib, Tobias Stamm, Manderim GmbH

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the source-code.

In case the source-code of this software is inaccessible to the end-user,
the above copyright notice and this permission notice shall be included
in any source-code which is dependent on this software and is accessible
to the end-user.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
