
NALib Readme
This file is part of NALib, a collection of C and C++ source code
intended for didactical purposes. Full license notice at the bottom.
====================


Table of Content:
-----------------
1.   Short Introduction
2.   Creation and deletion of memory and structs
2.1    Constructors
2.2    Destructors
2.3    Callbacks for container structs
3.   -
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
as you like. NALib nontheless provides two allocation methods (which can
be deallocated using free):

- naAllocate        Allocates the given number of bytes.
- naAllocateIfNull  Same as naAllocate but the space will only be allocated
                    if the pointer provided is NA_NULL.

Other than these basic allocation methods, NALib offers creation and deletion
functions for many structs.

The naming scheme guides you:

- naMakeXXX:    Returns the struct as a value. Only very basic structs will
                provide such functions.
- naFillXXX:    Expects the first argument to be a pointer to the struct.
- naCreateXXX:  Expects either a pointer to an existing block in memory or
                NA_NULL if you want NALib to allocate the memory. In any case:
                Returns the pointer to the struct. See section 2.1 for more
                information.
- naClearXXX:   Expects a pointer to a struct in which the memory shall be
                whiped. The pointer itself is not deallocated though. See
                section 2.2 for more information.
- naDestroyXXX: Same thing as naClearXXX, but the pointer is deallocated. See
                section 2.2 for more information.
- naNewXXX:     Uses the NARuntime system to quickly provide a pointer.
- naDelete:     Deletes a pointer allocated with naNewXXX.



2.1 Constructors
-----------------
Quite often, structs must be initialized before being able to use them.
As NALib is a C library, there are no constructors like in typical OOP
languages and consequently, such initialization must be done manually by
the programmer.

In NALib, initialization-functions always start with naCreateXXX. They
assume the given struct to be uninitialized and will overwrite all values.
The struct itself can either be already allocated outside of the createXXX
function (case A) or being automatically allocated within (case B):

A. If the given first argument of a createXXX function is NOT NULL, the
   struct at the given pointer is used. Outside of the function, the
   given struct is usually already stored in a variable and a pointer to
   that variable is used in the function call. For example:

   NAByteArray myarray;
   naCreateByteArrayWithSize(&myarray, 1234);

   When no longer used, these structs usually are destructed with the
   appropriate clearXXX function. See destructor explanation below.

   To stay consistent with case B, the pointer to the given struct
   is returned at the end of the function.

B. If the given first argument of an naCreateXXX function is a NULL-pointer,
   a new struct is allocated on the heap using malloc and at the end of the
   function, the pointer to that memory location is returned.
   Outside of the function, the returned pointer is usually stored in a
   pointer variable. For example:

   NAByteArray* myarray = naCreateByteArrayWithSize(NA_NULL, 1234);

   When no longer used, these structs usually are deleted using the
   appropriate destroyXXX function. See destructor explanation below.

   Important notice: Using a NULL-pointer might seem to be the easiest way
   when createing new structs. But each time, a new (likely small) space
   must be allocated on the heap and later be freed again. In contrast to
   modern programming languages, C is not optimized for this case and might
   result in quite noticeable speed drops.






2.2 Destructors
---------------
Structs which have been initialized using an naCreateXXX function (see above)
must be deleted with the appropriate destructor.

As NALib is a C library, there are no destructors like in typical OOP
languages and consequently, such deletion must be done manually by
the programmer.

In NALib, destruction-functions either start with naDestroyXXX or naClearXXX.
Both functions expect a pointer to the struct and will perform all
appropriate destructions. Basically, naClearXXX will erase the contents.
naDestroyXXX will call naClearXXX first and free the given pointer after that.

A. clearXXX functions are meant for variables whose content shall be erased
   but the memory of the struct itself shall not be freed.

B. destroyXXX functions are meant for pointers which shall be erased and
   freed completely from memory.

Usually, the cases listed here go in pairs with the cases listed in the
Constructors explanation above. Meaning a case A constructor usually will
require a case A destructor and a case B constructor usually a case B
destructor. But in certain cases, a programmer might want to choose
differently, for example to save time. If you choose so, just make sure
you know what you are doing.

Do not use the naDestroyXXX functions on stack variables!




2.3 Callbacks for container structs
-----------------------------------
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




3 Section 3
-----------
Looking for section 3? Currently there is none due to reordering of the
contents of this file. Don't worry about it.



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
