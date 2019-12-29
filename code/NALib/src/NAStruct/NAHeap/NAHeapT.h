
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// TEMPLATE
// This is an NALib template file. It uses macros which are defined before
// including this file to manipulate the implementation. Go look for the place
// this file is included to find more info.

// Movedown function.
// Perform a reordering of the heap such that a new key can be inserted. The
// return value is the index in which the key can safely be inserted. The
// curindex points at the starting position and the element at curindex is
// assumed to be empty (or contain no useful information).
NA_HDEF NAInt NA_T3(naHeapMoveDown, NA_T_DONT_MOVE_DOWN_COMPARATOR, NA_T_TYPE, NA_T_USE_BACKPOINTERS)(NAHeap* heap, const void* key, NAInt curindex){
  #if NA_T_USE_BACKPOINTERS
    NAHeapBackEntry* entries = heap->data;
  #else
    NAHeapEntry* entries = heap->data;
  #endif
  NAInt nextindex = curindex / 2;
  
  // Go from the leaf to the root and test, where the new element shall lie.
  while((nextindex > 0) && NA_KEY_OP(NA_T_DONT_MOVE_DOWN_COMPARATOR, NA_T_TYPE)(entries[nextindex].key, key)){
    entries[curindex] = entries[nextindex];
    #if NA_T_USE_BACKPOINTERS
      *(entries[curindex].backpointer) = curindex;
    #endif
    curindex = nextindex;
    nextindex /= 2;
  }
  
  return curindex;
}



// Moveup function.
// Assumes the given index to point at an element which is empty (or contains
// no useful information). Performs a reordering of the heap such that the
// given key can be inserted at a suitable index and returns this index.
NA_HDEF NAInt NA_T3(naHeapMoveUp, NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE, NA_T_USE_BACKPOINTERS)(NAHeap* heap, const void* key, NAInt curindex){
  #if NA_T_USE_BACKPOINTERS
    NAHeapBackEntry* entries = heap->data;
  #else
    NAHeapEntry* entries = heap->data;
  #endif

  NAInt indexl = curindex * 2 + 0;
  NAInt indexr = curindex * 2 + 1;
  // as long as there is at least one possible position... 
  while(NA_TRUE){
  
    if(indexr > heap->count){
      // There is at least one of the elements out of bounds.
      
      if(indexl <= heap->count){
        // only the left element must be checked. 
        if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexl].key, key)){
          entries[curindex] = entries[indexl];
          #if NA_T_USE_BACKPOINTERS
            *(entries[curindex].backpointer) = curindex;
          #endif
          curindex = indexl;
        }
      }
      // as the leaves have been reached, no need to perform another loop. 
      break;
    }

    if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexl].key, entries[indexr].key)){
      // the left element is more important than the right 
      if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexl].key, key)){
        // the left is more important than the key 
        entries[curindex] = entries[indexl];
        #if NA_T_USE_BACKPOINTERS
          *(entries[curindex].backpointer) = curindex;
        #endif
        curindex = indexl;
      }else{
        // noone is more important. 
        break;
      }
    }else{
      // the right element is more important than the left
      if(NA_KEY_OP(NA_T_DONT_MOVE_UP_COMPARATOR, NA_T_TYPE)(entries[indexr].key, key)){
        // the right is more important than the key 
        entries[curindex] = entries[indexr];
        #if NA_T_USE_BACKPOINTERS
          *(entries[curindex].backpointer) = curindex;
        #endif
        curindex = indexr;
      }else{
        // noone is more important. 
        break;
      }
    }

    indexl = curindex * 2 + 0;
    indexr = curindex * 2 + 1;
  }
  return curindex;
}


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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
