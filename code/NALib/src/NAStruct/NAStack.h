
#ifndef NA_STACK_INCLUDED
#define NA_STACK_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// This data structure provides the functionality of storing multiple elements
// of the same type ordered in a stack-wise order. The number of elements
// stored is only limited by the available memory.
//
// NAStack internally stores elements in a list of memory blocks. That list
// can grow and shrink when needed. The addresses of your elements inside
// the memory blocks though will not change as long as they are stored in
// the stack. Traversing the stack one-by-one can be done efficiently using
// an iterator.
//
// The stack can grow and shrink automatically whereas you can choose upon
// initialization how the growing and shrinking shall be performed using
// flags. You can also set an initial count or let NALib choose one for you.
//
// Implementation note: There always is at least one memory block present!
//
// Implementation note about shrinking: As naPopStack returns the pointer to
// the popped element, it must reside in memory after the function call, even
// if it was stored in a memory block which now became empty. Therefore, when
// automatically shrinking, the last memory block emptied is kept in memory.
// This is called "mildly" shrinking. If you want even that last memory block
// to be erased, you have to "aggressively" shrink the stack by manually
// call naShrinkStackIfNecessary with the aggressive flag set to true.
//
// IMPORTANT:
// NAStack is not a Pool structure!
// NAStack is a growing and shrinking structure which is especially good at
// providing quick space for single elements which simply need to be stored
// somewhere where the programmer does not want to care about.
//
// Aside from the clear functionality if a stack, you can use this structure
// when you are building up collections, for example:
// - parsing files without prior knowledge of the number of elements stored.
// - Creating a bunch of particles in a physically based simulation.
// - Creating an unknown number of edges within a point cloud.
// And many more examples. Turns out the author uses this structure quite a
// lot.



#include <stdlib.h>
#include "../NABase/NABase.h"



// The full type definition is in the file "NAStackII.h"
typedef struct NAStack NAStack;
typedef struct NAStackIterator NAStackIterator;



// A stack has an initial count but can grow automatically when the space is
// not sufficient. This is done by allocating additional memory blocks of
// a specific size. Choose one of the following methods to grow the stack:
// AUTO:        Uses the flag which the author of NALib thinks is best for
//              everyday general use. Currently FIBONACCI.
// LINEAR:      Each new memory block has the same size as the previous one.
// FIBONACCI:   Uses a fibonacci like growth.
// EXPONENTIAL: Uses factor 2 growth.
// FIXED_SIZE:  Stack always remains at the initial count which in this case
//              you have to specify with a count > 0.
#define NA_STACK_GROW_AUTO        0x00
#define NA_STACK_GROW_LINEAR      0x01
#define NA_STACK_GROW_FIBONACCI   0x02
#define NA_STACK_GROW_EXPONENTIAL 0x03
#define NA_STACK_FIXED_SIZE       0x04

// When emptying a stack using naPopStack, the allocated memory blocks can be
// erased from memory automatically, which is called shrinking. Choose one of
// the following flags to shrink the stack:
// AUTO:         When shrinking, keeps the last emptied memory block in memory
//               for not having to reallocate it again when growing. Any
//               memory block after that is erased.
// NO_SHRINKING: Does not shrink automatically. To manually shrink the stack,
//               use the naShrinkStackIfNecessary function.
// Note that AUTO can not be combined with NA_STACK_FIXED_SIZE.
#define NA_STACK_SHRINK_AUTO       0x00
#define NA_STACK_NO_SHRINKING      0x10

// Initializes a stack with the given typeSize in Bytes. The initialCount
// denotes how many elements will be preallocated from the beginning. Use 0
// to let NALib choose a reasonable number. To grow or shrink the number of
// elements the stack can hold, use the flags described above, one for growing
// and one for shrinking and combine them with the | operator. Use 0 if you
// want everything to be automatic. Note that in case of the FIXED_SIZE flag,
// the shrinking flag is ignored and also, you must specify an initialCount
// which is greater than 0.
NA_API NAStack* naInitStack(
  NAStack* stack,
  size_t typeSize,
  size_t initialCount,
  NAInt flags);

// Clears the given stack.
NA_API void naClearStack  (NAStack* stack);

// Top:   Accesses the topmost element by returning a pointer to the element.
// Push:  Grows the stack by 1 element and returns a pointer to the element.
// Pop:   Shrinks the stack by 1 element and returns a pointer to the element.
//        which just had beed popped.
// Peek:  Looks at the element with the given index. Warning: Not very fast.
//        Better use iterators if you want to visit all elements of the stack.
//
// Note that the Pop function returns the element which had been removed. It
// will still be available after a call to this function as long as it does
// not gets overwritten by another push operation or the stack shrinks again
// either by a later pop operation or by calling naShrinkStackIfNecessary.
NA_IAPI void* naTopStack(NAStack* stack);
NA_IAPI void* naPushStack(NAStack* stack);
NA_IAPI void* naPopStack(NAStack* stack);
NA_API  void* naPeekStack(NAStack* stack, size_t index);

// Returns the number of elements actually stored in the stack
NA_IAPI size_t naGetStackCount(const NAStack* stack);
// Returns the number of elements reserved in memory. Warning: Rather slow.
NA_API size_t naGetStackReservedCount(const NAStack* stack);

// If you want the stack to shrink manually, you can call this function. This
// is useful if you defined NO_SHRINKING upon init or if you want to release
// as much memory as possible. Normal shrinking left one memory block after
// the current block untouched in case new push operations will take place.
// But if you set aggressive to true, all unused memory blocks are released.
// Note that upon using this function and when observing an unusual high
// amount of repetitive memory allocations, you may want to set aggressive
// to NA_FALSE.
NA_API void naShrinkStackIfNecessary(NAStack* stack, NABool aggressive);

// Copies all of the contents of the stack to the given buffer. It is expected
// that buf contains enough space to hold all stored elements arranged as a
// plain C array.
NA_API void naDumpStack(NAStack* stack, void* buf);



// //////////////////////////
// Iteration functions
// //////////////////////////

// Creates and clears an interator working upon a given NAStack. You have to
// choose whether it is an accessor or mutator. An accessor can not get a
// mutable pointers to the elements.
NA_IAPI NAStackIterator naMakeStackAccessor(const NAStack* stack);
NA_IAPI NAStackIterator naMakeStackMutator (const NAStack* stack);
NA_IAPI void naClearStackIterator(NAStackIterator* iterator);

// Iterates through the stack from base to top. Returns NA_FALSE if there are
// no more elements.
NA_IAPI NABool naIterateStack(NAStackIterator* iterator);

// Upon creating, an iterator is at an initial position which, when accessed
// will result in a crash. As soon as iterating starts, the iterator is not
// at the initial position anymore. When reaching the end of iteration, the
// iterator again returns to the initial position. Use these functions to
// test if it is at the initial position or manually reset it to.
NA_IAPI NABool naIsStackAtInitial(NAStackIterator* iterator);
NA_IAPI void   naResetStackIterator(NAStackIterator* iterator);

// Returns a pointer to the current element. Note that if you store pointers
// you will get a pointer to a pointer. If you want a pointer directly, just
// use the p variant.
NA_IAPI const void* naGetStackCurConst    (NAStackIterator* iterator);
NA_IAPI void*       naGetStackCurMutable  (NAStackIterator* iterator);
NA_IAPI const void* naGetStackCurpConst   (NAStackIterator* iterator);
NA_IAPI void*       naGetStackCurpMutable (NAStackIterator* iterator);

// Will call the accessor or mutator on every element stored in the stack.
// The p variants expect this stack to store pointers hence the accessor or
// mutator will directly be called with the stored pointer instead of a
// pointer to a pointer.
NA_IAPI void naForeachStackConst   (const NAStack* stack, NAAccessor accessor);
NA_IAPI void naForeachStackMutable (const NAStack* stack, NAMutator  mutator);
NA_IAPI void naForeachStackpConst  (const NAStack* stack, NAAccessor accessor);
NA_IAPI void naForeachStackpMutable(const NAStack* stack, NAMutator  mutator);



// Inline implementations are in a separate file:
#include "Core/NAStack/NAStackII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_STACK_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
