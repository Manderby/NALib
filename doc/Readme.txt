
NALib Readme
This file is part of NALib, a collection of C source code
intended for didactical purposes. Full license notice at the bottom.
====================


Table of Content:
-----------------
1.   Short Introduction
2    Memory and Structs
2.1    Creation and deletion of memory blocks
2.2    Initializating and Clearing of structs
3.   Runtime System
4.   C90 variable definitions
5.   Inline Implementations
6.   Opaque types
7.   Symbol naming
8.   Default header and footer




1. Short Introduction
---------------------
- Simply include the whole NALib folder into your project.
- NALib just needs the standard libraries.
- Any documentation can be found within the source files or in this file.

If you are curious on how NALib works, start at the most basic files:
- NASystem.h      Functions, Macros and Definitions, System-Dependancy
- NAMemory.h      Functions to work with the memory
- NABinaryData.h  Memory manipulations

For more information, visit
http://manderc.com/nalib/index.php

Have fun, Tobias Stamm.





2. Memory and Structs
---------------------

NALib is a collection of C code. The C programming language has no notion of
object oriented programming (OOP) and therefore relies on simpler concepts
like memory blocks, pointers and structs (sometimes called "records" in other
languages).

A huge deal of programming in C and hence programming with NALib is to create
and delete pointers and structs as well as using them with callbacks and
iterators. The following chapters will give you an overview of how to do all
that in NALib.



2.1 Creation and deletion of memory blocks
------------------------------------------
You can use the default malloc and free functions to create memory blocks
as you like. NALib nontheless provides its own allocation methods (which are
basically mappings to malloc and free):

- naMalloc            Allocates the given number of bytes.
- naAlloc             Allocates space for the given NALib structure. Use it
                      for example like this: NAArray* a = naAlloc(NAArray);
- naFree              Frees the memory allocated with above functions.

You can also allocate memory on byte numbers or even page boundaries:

- naMallocAligned     Allocates memory aligne to a certain byte number.
- naMallocPageAligned Allocates the given number of bytes but the first byte
                      will be aligned to a page boundary.
- naFreePageAligned   Frees the aligned memory.

Additionally, starting with NALib version 10, you can use the following
two functions (See section 3 for more information):

- naNew:        Uses the NARuntime system to quickly provide a pointer.
- naDelete:     Deletes a pointer allocated with naNew.




2.2 Initializating and Clearing of structs
-------------------------

Other than these basic memory allocation methods, NALib offers creation and
deletion functions for many structs.

The naming scheme guides you:

--- MAKE, FILL, INIT and CLEAR ---
- naMakeXXX:    Returns the struct as a value. Only very basic structs (pod)
                will provide such functions.
- naFillXXX:    Expects the first argument to be a pointer to the struct.
                Again, only basic structs (pod) will offer such functions.
- naInitXXX:    Expects a pointer to an existing struct in memory. That pointer
                is expected to point at a non-initialized struct and will now
                be initialized. Returns the pointer again.
- naClearXXX:   Expects a pointer to a struct which shall be cleared. Is used
                both by pod and non-pod structs.

--- NEW and DELETE ---
- naNewXXX:     Starting with NALib version 10, certain structs like NAPointer
                have been restricted to be managed by the runtime system. These
                structs do not provide naInitXXX_functions but naNewXXX-
                functions instead.
- naNew:        The plain naNew function allocates enough space for the given
                type. Must use NA_RUNTIME_TYPE in order to work.
- naDelete:     Deletes the given pointer which previously has been created
                using an naNewXXX function.

--- CREATE, RETAIN and RELEASE ---
- naCreateXXX:  Starting with NALib version 18, certain structs have reference
                counting built in. After creation, you shall use naRetainXXX
                and naReleaseXXX.
- naRetainXXX:  Adds 1 to the reference count of the struct.
- naReleaseXXX: Subtracts 1 from the reference count of the struct. If zero is
                reached, the struct will automatically be destroyed.


Structs usually must be initialized before being able to use them and must
be destroyed when not needed anymore. As NALib is a C library, there are no
constructors or destructors like in typical OOP languages and consequently,
such initialization must be done manually by the programmer.

Simple structs like NAPos are very basic structs and are generally called
pod (plain old data). Such often provide an naMakeXXX or naFillXXX functions
and sometimes need a corresponding naClearXXX function. See the specific API
for more information.

For more complex structs, the construction-functions usually are called
naInitXXX, naNewXXX or naCreateXXX. The corresponding destruction function
again is called naClearXXX. The naInitXXX-functions usually assume the given
struct to be uninitialized and will overwrite all values. The naNewXXX
functions use the runtime of NALib to allocate enough space and the naCreateXXX
functions do the allocation in their own way.

The naClearXXX-functions expect a pointer to an initialized struct and will
perform all necessary destructions. Beware that naClearXXX will only erase
the contents but not the pointer itself.

When using naInitXXX functions, the struct itself must always be allocated
outside of the naInitXXX function.

Considering creation and deletion, you can think of the following scenarios:

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

C) Use an existing naNewXXX function with naDelete:
NAPNG* myPNG = naNewPNG();
naDelete(myPNG);

D) Allocate with naNew and delete with naDelete:
NAArray* myarray = naInitArray(naNew(NAArray));
naDelete(myarray);

E) Use an existing naCreateXXX function with naRelease:
NABuffer* mybuffer = naCreateBuffer();
naRelease(mybuffer);

The cases listed here are usual scenarios. But in certain cases, a programmer
might want to choose differently, for example to save time. If you choose so,
just make sure you know what you are doing.







3 Runtime System
-----------
Since NALib version 10, there exists a small Runtime System which needs to be
running for certain structs. It mainly provided inifinitely large pools for
specific NALib structures for quick allocation. Starting with NALib version 16,
that runtime system can easily be enhanced for custom types.

The author decided to add such a system because certain structs which becomes
way easier to manage when they are always provided as pointers.

In order to manage the pointers, an instance of NARuntime must be created
before any naNew function can be called. When NDEBUG is undefined, a friendly
warning tells you to start the runtime system. You can start it by a simple
call to

naStartRuntime();

When you no longer need the runtime system, you can stop it using

naStopRuntime();

The runtime system may be enhanced in future versions like with deallocation
pools or even some sort of garbage collection or (in the far future)
just-in-time compilation.





4. C90 variable definitions
----------------------------
Note that this code has been written to be compatible with C90. This means in
particular that all variables have to be defined prior to any implementation in
the same code block.

The standard C90 is partially still in use by some compilers. This standard
states that each variable MUST be declared before it is used. Later standards
and most modern compilers do not really need this anymore but when you try to
compile a static or dynamic library on certain compilers, you will most
probably run into the problem that the declarations must be present.

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

Starting with NALib version 16, the author decided to move any inline
implementation into a separate header file, for example NAArray.h has an
additional NAArrayII.h file. This II-File (Inline Implementation) is stored
in a folder so that the main .h-File remains in the base folder visible to
anyone.




6. Opaque Types
---------------
All around NALib you find declarations like these:

typedef struct NAArray NAArray;

This line tells the compiler that NAArray shall be used as a type. After this
line, the compiler expects a declaration somewhere and allows the programmer to
use the type by just using its name. The actual declaration of this type can
be written anywhere but in NALib is often given in the inline implementation
files by the following lines starting with something like

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

To not limit the functionality, all such types are declared somewhen after its
typedef in the same header file. This allows NALib to inline lots of code and
hence become very fast. The programmer though has to take the responsibility
to not access any of the fields of an opaque type directly. Always use the API!

Note that if you want to create a library where the types become opaque but
can nontheless be accessed by your own implementation, collect all opaque type
declarations into one header file which will be included by the implementation
files and must be present during compilation. During distribution, remove that
header file from the package. It's tricky, but it works.






7. Symbol naming
----------------
In NALib, a (more or less) consistent naming scheme is applied. Every symbol
consists at least of a suffix and an identifier.

Suffix:
- types always have the prefix      NA    and are MixedCase
- macros always have the prefix     NA_   and are UPPERCASE
- functions always have the prefix  na    and are MixedCase
- variables always have the prefix  na_   and are lowercase

Further more, symbols are declared with the following prefixes:

Declaration | Definition | Explanation
------------+------------+------------------------
NA_API      | NA_DEF     | Public normal functions. Default calling convention.
            |            | API will not be exported when linking.
NA_IAPI     | NA_IDEF    | Public inline functions. Code will likely be inlined.
            |            | Declared static to prevent duplicate definitions and
            |            | to not export API when linking.
NA_HAPI     | NA_HDEF    | Helper function. Default calling convention. Belongs
            |            | to internal code of NALib. Would normally not be
            |            | visible to the programmer but in NALib, you can
            |            | look it up. API is not exported when linking.
NA_HIAPI    | NA_HIDEF   | Inline helper function. Same thing but inlined.

Types are always typedef'd, meaning you won't have to write the struct keyword
all the time when declaring variables. Also enums are typedef'd with a clear
name. Example: NAAscDateTimeFormat

Some macros have just one identifier after the prefix. These macros denote
key values often used in all of the source code. Examples: NA_NULL, NA_RESTRICT

Some other macros have multiple identifiers after the prefix. The first
identifier denotes the topic which the macros are used for. The remaining
identifiers are a more and more refined description of what the macro describes.
Examples: NA_SYSTEM_WINDOWS, NA_ENDIANNESS_BIG

Functions have a description of what they do after the prefix. The first word
after the prefix is usually a verb, such as Convert, Locate or Get. After that
comes usually a type upon which the function operates. For example Array or
String but also something like i8 or 32, denoting an integer with 8 bits or a
memory block with 32 bits. After that, a more concrete description of what the
function does or expects as parameters is given. Like for example WithLength,
FromFilename.

Variables and printf arguments are rarely used in NALib. They have an
appropriate description after the prefix like for example

na_monthenglishabbreviationnames, na_runtime
NA_PRIi, NA_TAB, NA_NL_UNIX




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
