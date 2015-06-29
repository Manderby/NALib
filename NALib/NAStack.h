
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_STACK_INCLUDED
#define NA_STACK_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif





typedef struct NAStack NAStack;
struct NAStack{
  void** pointers;
  NAUInt maxcount;
  NAUInt cur;
};



NAStack* naCreateStack(NAStack* stack, NAUInt maxcount){
  stack = naAllocNALibStruct(stack, NAStack);
  stack->pointers = naAllocate(sizeof(void*) * maxcount);
  stack->maxcount = maxcount;
  stack->cur = 0;
  return stack;
}



naClearStack(NAStack* stack){
  free(stack->pointers);
}



naDestroyStack(NAStack* stack){
  naClearStack(stack);
  free(stack);
}



void naPushStack(NAStack* stack, void* pointer){
  #ifndef NDEBUG
    if(stack->cur == stack->maxcount)
      naError("naPushStack", "Stack is full");
  #endif
  stack->pointers[stack->cur] = pointer;
  stack->cur++;
}



void* naPopStack(NAStack* stack){
  #ifndef NDEBUG
    if(stack->cur == 0)
      naError("naPopStack", "Stack is empty");
  #endif
  stack->cur--;
  return stack->pointers[stack->cur];
}



void* naTopStack(NAStack* stack){
  #ifndef NDEBUG
    if(stack->cur == 0)
      naError("naTopStack", "Stack is empty");
  #endif
  return stack->pointers[stack->cur - 1];
}



NABool naIsStackEmpty(NAStack* stack){
  return (stack->cur == 0);
}



NABool naIsStackFull(NAStack* stack){
  return (stack->cur == stack->maxcount);
}


#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_STACK_INCLUDED

// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
