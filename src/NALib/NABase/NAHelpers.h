
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// ////////////////////////
// Note that in NALib, there is no base typedef for a void* or a const void*
// pointer. Some libraries or frameworks define them with a name like NAVoid.
// The author believes that void* pointers should only be typedef'd if they
// describe a distinct purpose, like for example if a returned void* points
// to a group of structs sharing some common properties.
//
// If void* is used plainly as a pointer to an untyped address, the author
// thinks that it is best to just write void*, not some abscure type name.
// Therefore, you won't find any type definition of NAVoid here.



// ////////////////////////
// The following macro is used to suppress compiler warnings in functions
// which do not make any use of a given parameter. This happens when function
// signatures are chosen to match predefined function pointer types or shall
// in general have a specific look. By using the given parameter as an
// expression and casting the result to void makes the compiler think that
// the parameter is used but has the explicit order to not use any
// evaluated value.
//
// Be careful with this when using the volatile keyword.

#define NA_UNUSED(x) (void)(x)




// ////////////////////////
// Some container structs like NAArray or NAList provide naForeachXXX functions
// with an additional parameter: An accessor or mutator callback with the
// following signatures:

typedef void (*NAAccessor) (const void*);
typedef void (*NAMutator)  (      void*);

// This parameter allows you to provide a function pointer to an accessing or
// mutation function which will be called for every element in the container.
//
// You can use accessors or mutators to simply address all elements in a
// container struct. But you can also use them to initialize all elements with
// a call to naInitXXX and to desctruct all elements with a call to naClearXXX.
//
// IMPORTANT:
// Beware that your accessor or mutator will always be called with a POINTER to
// the content. If for example, you have an array of integers, your callback
// will get values of type "int *". But if your array stores pointers to int,
// your callback will get values of type "int **".
//
// Note that you can also use any of the functions of NALib as callback
// functions as long as they accept only one data pointer parameter and return
// void.
//
// You may have to cast callback functions to "NAAccessor" or NAMutator".
//
// Apart from callback functions, many container structs have separate
// iterators.
// Have a look at the API.
//
// Callback functions are used for:
// - Accessor or Mutator functions for naForeachXXX functions.
// - Application start functions
// - Threading start functions
// - Timed functions
// - General callback functions







// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
