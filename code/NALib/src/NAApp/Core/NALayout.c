
#include "NAAppCore.h"
#include "../NAApp.h"
#include "../../NAStruct/NAList.h"



//              /--------------- block ------------\
//   /-margin-\/-padding-\                          \
//
//   +-------------------------------------------------+
//   |         +------------------------------------+  |
//   |         |          +----------------------+  |  |
//   |         |          |      +=========+     |  |  |
//   |         |          |      | content |     |  |  |
//   |         |          |      +=========+     |  |  |
//   |         |          +----------------------+  |  |
//   |         +------------------------------------+  |
//   +-------------------------------------------------+
//



typedef struct NA_LayoutElement NA_LayoutElement;
struct NA_LayoutElement {
  NA_LayoutElement* parent;
  NAList childs;
  void* uiElement;
  NABool isSecondary;

  double blockSize1;
  double blockSize2;
  double minBlockSize1;
  double minBlockSize2;
  NABorder2D margin;
  NABorder2D padding;

  union {
    struct {
      double size1;
      double size2;
      NALayoutAlign align1;
      NALayoutAlign align2;
      NABool alignBaseline;
      NALayoutDirections layoutDirections;
    } element;
  };
};



#define NA_LAYOUT_DIRECTION_PRIMARY_IS_VERTICAL 0x04
#define NA_LAYOUT_DIRECTION_BOTTOM_TO_TOP 0x02
#define NA_LAYOUT_DIRECTION_RIGHT_TO_LEFT 0x01



NABool naGetLayoutDirectionsHorizontalIsRightToLeft(NALayoutDirections directions) {
  #if NA_DEBUG
    if(directions < 0 || directions > 7)
      naError("Only explicit directions can be used with this function");
  #endif
  return naGetFlagu32(directions, NA_LAYOUT_DIRECTION_RIGHT_TO_LEFT);
}

NABool naGetLayoutDirectionsVerticalIsBottomToTop(NALayoutDirections directions) {
  #if NA_DEBUG
    if(directions < 0 || directions > 7)
      naError("Only explicit directions can be used with this function");
  #endif
  return naGetFlagu32(directions, NA_LAYOUT_DIRECTION_BOTTOM_TO_TOP);
}

NABool naGetLayoutDirectionsPrimaryIsVertical(NALayoutDirections directions) {
  #if NA_DEBUG
    if(directions < 0 || directions > 7)
      naError("Only explicit directions can be used with this function");
  #endif
  return naGetFlagu32(directions, NA_LAYOUT_DIRECTION_PRIMARY_IS_VERTICAL);
}



void na_EndLayoutElement(NA_LayoutElement* elem);
void na_AlignLayoutElement(
  NA_LayoutElement* elem,
  NARect rect,
  NABool horizontalIsRightToLeft,
  NABool verticalIsBottomToTop,
  NABool orderingVH); // sections vertical, elements horizontal
NASpace* na_GetLayoutParentSpace(NA_LayoutElement* elem);

NA_LayoutElement* na_curLayoutElement = NA_NULL;



NA_LayoutElement* na_AllocLayoutElement(
  NA_LayoutElement* parent,
  void* uiElement,
  NABorder2D padding,
  NABool isSecondary)
{
  NA_LayoutElement* elem = naAlloc(NA_LayoutElement);
  elem->parent = parent;
  naInitList(&elem->childs);
  elem->uiElement = uiElement;
  elem->isSecondary = isSecondary;

  elem->minBlockSize1 = 0.;
  elem->minBlockSize2 = 0.;
  elem->margin = naMakeBorder2D(0., 0., 0., 0.);
  elem->padding = padding;

  return elem;
}



void na_DeallocLayoutElement(NA_LayoutElement* elem) {
  naClearList(&elem->childs, (NAMutator)na_DeallocLayoutElement);
  naFree(elem);
}



void naBeginLayout(NASpace* space, NABorder2D padding, NALayoutDirections layoutDirections) {
  #if NA_DEBUG
    if(na_curLayoutElement == NA_NULL && !space)
      naError("For a new layout, space must not be a nullptr.");
    if(na_curLayoutElement && !na_curLayoutElement->isSecondary)
      naError("Use naBeginLayout either for a new layout or when working within a subsection.");
  #endif // NA_DEBUG

  if(na_curLayoutElement && space) {
    na_AddSpaceChildUnpositioned(
      na_curLayoutElement->uiElement ? na_curLayoutElement->uiElement : na_GetLayoutParentSpace(na_curLayoutElement),
      space);
  }

  NA_LayoutElement* newElement = na_AllocLayoutElement(
    na_curLayoutElement,
    space,
    padding,
    NA_FALSE);

  newElement->element.size1 = NA_LAYOUT_GROW;
  newElement->element.size2 = NA_LAYOUT_GROW;
  newElement->blockSize1 = NA_LAYOUT_GROW;
  newElement->element.align1 = NA_ALIGN_CENTER;
  newElement->element.align2 = NA_ALIGN_CENTER;
  newElement->element.alignBaseline = NA_FALSE;
  newElement->element.layoutDirections = layoutDirections;

  if(na_curLayoutElement) {
    // We create a sub-layout
    // End the last element of the current section if available
    if(!naIsListEmpty(&na_curLayoutElement->childs)) {
      NA_LayoutElement* lastElement = naGetListLastMutable(&na_curLayoutElement->childs);
      na_EndLayoutElement(lastElement);
    }
    // Add the new layout to the end.
    naAddListLastMutable(&na_curLayoutElement->childs, newElement);
  }

  na_curLayoutElement = newElement;
}



void naEndLayout() {
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
  #endif // NA_DEBUG

  if(na_curLayoutElement->isSecondary) {
    // Currently working on a subSection. Close this first.
    na_EndLayoutElement(na_curLayoutElement);
    // Move up to the parent.
    na_curLayoutElement = na_curLayoutElement->parent;
  }
  
  if(na_curLayoutElement->parent) {
    // We are in a sub-layout.
    // Move up to the containing section.
    na_curLayoutElement = na_curLayoutElement->parent;
  }else{
    // Reaching here, na_curLayoutElement is the root element which now has
    // the min sizes computed. Propagate the information down the whole tree
    // and align all elements properly.
    
    // Compute the min sizes
    na_EndLayoutElement(na_curLayoutElement);

    na_AlignLayoutElement(
      na_curLayoutElement,
      naMakeRectS(
        0.,
        0.,
        na_curLayoutElement->minBlockSize1,
        na_curLayoutElement->minBlockSize2),
      naGetLayoutDirectionsHorizontalIsRightToLeft(na_curLayoutElement->element.layoutDirections),
      naGetLayoutDirectionsVerticalIsBottomToTop(na_curLayoutElement->element.layoutDirections),
      naGetLayoutDirectionsPrimaryIsVertical(na_curLayoutElement->element.layoutDirections));
    
    // Finally, deallocate everything.
    na_DeallocLayoutElement(na_curLayoutElement);
    na_curLayoutElement = NA_NULL;
  }
}



NASpace* na_GetLayoutParentSpace(NA_LayoutElement* elem) {
  while(1) {
    elem = elem->parent;
    if(elem->uiElement) {
      return elem->uiElement;
    }
    if(!elem->parent) {
      #if NA_DEBUG
        naError("No parent space found");
      #endif
      return NA_NULL;
    }
  }
}



void naAddLayoutSection(
  double blockSize2,
  NABorder2D padding)
{
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
  #endif // NA_DEBUG

  if(na_curLayoutElement->isSecondary) {
    // Currently working on another section. Close this first.
    na_EndLayoutElement(na_curLayoutElement);
    // Move up to the parent.
    na_curLayoutElement = na_curLayoutElement->parent;
  }

  NA_LayoutElement* subElem = na_AllocLayoutElement(
    na_curLayoutElement,
    NA_NULL,
    padding,
    NA_TRUE);
  subElem->blockSize1 = NA_LAYOUT_GROW;
  subElem->blockSize2 = blockSize2;

  naAddListLastMutable(&na_curLayoutElement->childs, subElem);
  na_curLayoutElement = subElem;
}



void na_EndLayoutElement(NA_LayoutElement* elem) {
  // If this is a section, end the last element if available
  if(elem->isSecondary && !naIsListEmpty(&elem->childs)) {
    NA_LayoutElement* lastElement = naGetListLastMutable(&elem->childs);
    na_EndLayoutElement(lastElement);
  }
  
  // Go through all childs and sum up the min blockSizes and the margins.
  NAListIterator iter = naMakeListAccessor(&elem->childs);
  while(naIterateList(&iter)) {
    const NA_LayoutElement* child = naGetListCurConst(&iter);
    double childMinBlockSize1 = child->margin.begin1 + child->minBlockSize1 + child->margin.end1;
    double childMinBlockSize2 = child->margin.begin2 + child->minBlockSize2 + child->margin.end2;
    if(elem->isSecondary) {
      elem->minBlockSize1 += childMinBlockSize1;
      if(childMinBlockSize2 > elem->minBlockSize2) {
        elem->minBlockSize2 = childMinBlockSize2;
      }
    }else{
      if(childMinBlockSize1 > elem->minBlockSize1) {
        elem->minBlockSize1 = childMinBlockSize1;
      }
      elem->minBlockSize2 += childMinBlockSize2;
    }
  }
  naClearListIterator(&iter);

  // If this elements sub-layouts primary and secondary directions are opposite
  // to the directions this element is contained in, we need to swap the sizes.
  if(!elem->isSecondary && elem->parent && naGetLayoutDirectionsPrimaryIsVertical(elem->element.layoutDirections) != naGetLayoutDirectionsPrimaryIsVertical(elem->parent->parent->element.layoutDirections)) {
    double tmpSize = elem->minBlockSize1;
    elem->minBlockSize1 = elem->minBlockSize2;
    elem->minBlockSize2 = tmpSize;
  }
  
  if(!elem->isSecondary) {
    if(elem->element.size1 > elem->minBlockSize1) {
      elem->minBlockSize1 = elem->element.size1;
    }
    if(elem->element.size2 > elem->minBlockSize2) {
      elem->minBlockSize2 = elem->element.size2;
    }
  }

  // The following error message was removed because there are situations
  // like uiElements having a size bigger than a lineHeight by default
  // even when coming from the native system implementation.
  // if(elem->minBlockSize2 > elem->section.size2)
  //   naError("The minimal required size is larger than the given size");

  // If the blockSize of the section is defined, override the minBlockSize.
  if(elem->blockSize1 > 0.) {
    elem->minBlockSize1 = elem->blockSize1;
  }
  if(elem->blockSize2 > 0.) {
    elem->minBlockSize2 = elem->blockSize2;
  }
  
  // If the blockSize was set to shrinking, we set it to the minimal size.
  if(elem->blockSize1 < 0.) { // NA_LAYOUT_MIN
    elem->blockSize1 = elem->minBlockSize1;
  }
  if(elem->blockSize2 < 0.) { // NA_LAYOUT_MIN
    elem->blockSize2 = elem->minBlockSize2;
  }
  
  // Add the padding of this section.
  elem->minBlockSize1 += elem->padding.begin1 + elem->padding.end1;
  elem->minBlockSize2 += elem->padding.begin2 + elem->padding.end2;

  // Reaching here, the following holds true:
  // minBlockSize1 and minBlockSize2 are defined.
  // element.size is either defined or NA_LAYOUT_GROW.
  // blockSize is either defined or NA_LAYOUT_GROW.
}



void na_PreserveDebugInfo(NA_LayoutElement* elem, NARect blockRect) {
  NA_LayoutRects* layoutRects = naAlloc(NA_LayoutRects);
  
  layoutRects->marginRect = naMakeRectWithBorder(blockRect, elem->margin);
  layoutRects->blockRect = blockRect;
  
  NABorder2D negativePadding;
  negativePadding.begin1 = -elem->padding.begin1;
  negativePadding.begin2 = -elem->padding.begin2;
  negativePadding.end1 = -elem->padding.end1;
  negativePadding.end2 = -elem->padding.end2;
  layoutRects->contentRect = naMakeRectWithBorder(blockRect, negativePadding);
  
  if(((NA_UIElement*)elem->uiElement)->layoutRects) {
    naFree(((NA_UIElement*)elem->uiElement)->layoutRects);
  }
  ((NA_UIElement*)elem->uiElement)->layoutRects = layoutRects;
}



void na_AlignLayoutSection(
  NA_LayoutElement* elem,
  NARect blockRect,
  NABool horizontalIsRightToLeft,
  NABool verticalIsBottomToTop,
  NABool orderingVH)
{
  // Finally, if a uiElement is available, add it to the parent space.
  if(elem->uiElement) {
    naSetUIElementRect(elem->uiElement, blockRect);

    #if NA_DEBUG
      na_PreserveDebugInfo(elem, blockRect);
    #endif // NA_DEBUG
  }

  NARect flexRect = blockRect;

  // First, we position the uiElement, if available.
  if(elem->uiElement) {
    // When the uiElement is given, it must be an NASpace and childs will be
    // added to that space. Therefore, we reset the local coordinate system
    // origin to zero.
    flexRect.pos = naMakePosZero();
  }
  
  // Compute, how much space there is in the primary direction to distribute
  double totalFlexibleSpace1 = orderingVH
    ? flexRect.size.height - elem->minBlockSize1
    : flexRect.size.width - elem->minBlockSize1;

  // We subtract the padding around the rectangle.
  if(orderingVH) {
    flexRect.pos.x += horizontalIsRightToLeft ? elem->padding.end2 : elem->padding.begin2;
    flexRect.pos.y += verticalIsBottomToTop ? elem->padding.end1 : elem->padding.begin1;
    flexRect.size.width -= elem->padding.begin2 + elem->padding.end2;
    flexRect.size.height -= elem->padding.begin1 + elem->padding.end1;
  }else{
    flexRect.pos.x += horizontalIsRightToLeft ? elem->padding.end1 : elem->padding.begin1;
    flexRect.pos.y += verticalIsBottomToTop ? elem->padding.end2 : elem->padding.begin2;
    flexRect.size.width -= elem->padding.begin1 + elem->padding.end1;
    flexRect.size.height -= elem->padding.begin2 + elem->padding.end2;
  }
  
  // Now, flexRect is the final rect we want to distribute our elements in.
  
  // Go through all childs and count how many have to grow in primary size.
  size_t elementsToFit = 0;
  NAListIterator countingIter = naMakeListMutator(&elem->childs);
  while(naIterateList(&countingIter)) {
    NA_LayoutElement* child = naGetListCurMutable(&countingIter);
    if(child->blockSize1 == NA_LAYOUT_GROW) {
      elementsToFit++;
    }
  }
  naClearListIterator(&countingIter);
  
  // We have all the information we need. Go through all childs and align them.
  
  NAListIterator iter = naMakeListMutator(&elem->childs);
  while(naIterateList(&iter)) {
    NA_LayoutElement* child = naGetListCurMutable(&iter);
    
    // Determine the primary size which needs to be added to the child rect
    // due to distribution of flexible space.
    double flexSpace1 = 0.;
    if(child->blockSize1 == NA_LAYOUT_GROW) {
      // Compute it like that so that no rounding errors or left over sizes
      // occur.
      flexSpace1 = naFloor(totalFlexibleSpace1 / (double)elementsToFit);
      totalFlexibleSpace1 -= flexSpace1;
      elementsToFit--;
    }
    
    double childBlockSize1 = child->minBlockSize1 + flexSpace1;
    
    NARect subElemRect = naMakeRectS(
        flexRect.pos.x,
        flexRect.pos.y,
        orderingVH ? flexRect.size.width : childBlockSize1,
        orderingVH ? childBlockSize1 : flexRect.size.height);
        
    double marginedChildBlockSize1 = childBlockSize1 + child->margin.begin1;
        
    // Flip the primary direction if necessary and adjust the rect of the
    // container as well as the child.
    if(orderingVH) {
      if(verticalIsBottomToTop) {
        flexRect.pos.y += marginedChildBlockSize1;
        subElemRect.pos.y += child->margin.begin1;
      }else{
        subElemRect.pos.y = naGetRectEndY(flexRect) - marginedChildBlockSize1;
      }
      flexRect.size.height -= marginedChildBlockSize1;
    }else{
      if(horizontalIsRightToLeft) {
        subElemRect.pos.x = naGetRectEndX(flexRect) - marginedChildBlockSize1;
      }else{
        flexRect.pos.x += marginedChildBlockSize1;
        subElemRect.pos.x += child->margin.begin1;
      }
      flexRect.size.width -= marginedChildBlockSize1;
    }

    // Finally, align the child.
    na_AlignLayoutElement(
      child,
      subElemRect,
      naGetLayoutDirectionsHorizontalIsRightToLeft(elem->parent->element.layoutDirections),
      naGetLayoutDirectionsVerticalIsBottomToTop(elem->parent->element.layoutDirections),
      orderingVH);
  }
  naClearListIterator(&iter);
}



void naAddLayoutElement(
  void* uiElement,
  double preMargin1,
  double size1,
  double size2)
{
  // End the last element of the current section if available
  if(!naIsListEmpty(&na_curLayoutElement->childs)) {
    NA_LayoutElement* lastElement = naGetListLastMutable(&na_curLayoutElement->childs);
    na_EndLayoutElement(lastElement);
  }
  
  if(uiElement) {
    na_AddSpaceChildUnpositioned(
      na_curLayoutElement->uiElement ? na_curLayoutElement->uiElement : na_GetLayoutParentSpace(na_curLayoutElement),
      uiElement);
  }

  NABool orderingVH = naGetLayoutDirectionsPrimaryIsVertical(na_curLayoutElement->parent->element.layoutDirections);

  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
    if(!na_curLayoutElement->isSecondary)
      naError("Use naAddLayoutSection before adding Elements.");
  #endif // NA_DEBUG
  
  NARect uiElementRect = uiElement
    ? naGetUIElementRect(uiElement)
    : naMakeRectZero();

  NA_LayoutElement* subElem = na_AllocLayoutElement(
    na_curLayoutElement,
    uiElement,
    naMakeBorder2D(0., 0., 0., 0.),
    NA_FALSE);
  
  // compute the element primary size.
  if(size1 < 0) { // NA_LAYOUT_MIN
    #if NA_DEBUG
      if(!uiElement)
        naError("Using NA_LAYOUT_MIN without a uiElement will shrink to zero.");
    #endif // NA_DEBUG
    if(orderingVH) {
      subElem->element.size1 = uiElementRect.size.height;
    }else{
      subElem->element.size1 = uiElementRect.size.width;
    }
  }else{
    subElem->element.size1 = size1;
  }
  
  subElem->blockSize1 = subElem->element.size1;
  subElem->blockSize2 = NA_LAYOUT_GROW;

  // compute the element secondary size.
  if(size2 < 0) { // NA_LAYOUT_MIN
    #if NA_DEBUG
      if(!uiElement)
        naError("Using NA_LAYOUT_MIN without a uiElement will shrink to zero.");
    #endif // NA_DEBUG
    if(orderingVH) {
      subElem->element.size2 = uiElementRect.size.width;
    }else{
      subElem->element.size2 = uiElementRect.size.height;
    }
  }else{
    subElem->element.size2 = size2;
  }

  subElem->margin.begin1 = preMargin1;
  subElem->element.align1 = NA_ALIGN_CENTER;
  subElem->element.align2 = NA_ALIGN_CENTER;
  
  if(uiElement && !naIsUIElementBlock(uiElement)) {
    subElem->element.alignBaseline = NA_TRUE;
  }else{
    subElem->element.alignBaseline = NA_FALSE;
  }
  
  subElem->element.layoutDirections = na_curLayoutElement->parent->element.layoutDirections;

  naAddListLastMutable(&na_curLayoutElement->childs, subElem);
}



void naSetLayoutSectionSpace(NASpace* space) {
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
    if(!na_curLayoutElement->isSecondary)
      naError("Use naAddLayoutSection first.");
  #endif // NA_DEBUG
  
  if(space) {
    na_AddSpaceChildUnpositioned(
      na_curLayoutElement->uiElement ? na_curLayoutElement->uiElement : na_GetLayoutParentSpace(na_curLayoutElement),
      space);
  }

  na_curLayoutElement->uiElement = space;
}



void naSetLayoutElementPrimaryAlign(NALayoutAlign align1) {
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
    if(!na_curLayoutElement->isSecondary)
      naError("Use naAddLayoutSection before working with Elements.");
  #endif // NA_DEBUG
  
  NA_LayoutElement* lastChild = naGetListLastMutable(&na_curLayoutElement->childs);
  
  #if NA_DEBUG
    if(!lastChild)
      naError("No element in the current section available.");
  #endif // NA_DEBUG

  lastChild->element.align1 = align1;
}



void naSetLayoutElementSecondaryAlign(NALayoutAlign align2) {
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
    if(!na_curLayoutElement->isSecondary)
      naError("Use naAddLayoutSection before working with Elements.");
  #endif // NA_DEBUG
  
  NA_LayoutElement* lastChild = naGetListLastMutable(&na_curLayoutElement->childs);
  
  #if NA_DEBUG
    if(!lastChild)
      naError("No element in the current section available.");
  #endif // NA_DEBUG

  lastChild->element.align2 = align2;
}



void naSetLayoutElementAlignBaseline(NABool alignBaseline) {
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
    if(!na_curLayoutElement->isSecondary)
      naError("Use naAddLayoutSection before working with Elements.");
  #endif // NA_DEBUG
  
  NA_LayoutElement* lastChild = naGetListLastMutable(&na_curLayoutElement->childs);
  
  #if NA_DEBUG
    if(!lastChild)
      naError("No element in the current section available.");
  #endif // NA_DEBUG

  lastChild->element.alignBaseline = alignBaseline;
}



void naSetLayoutElementBlockSize1(double blockSize1) {
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
  #endif // NA_DEBUG
  
  if(na_curLayoutElement->isSecondary) {
    NA_LayoutElement* lastChild = naGetListLastMutable(&na_curLayoutElement->childs);
    #if NA_DEBUG
      if(!lastChild)
        naError("No element in the current section available.");
    #endif // NA_DEBUG

    lastChild->blockSize1 = blockSize1;
  }else{
    na_curLayoutElement->blockSize1 = blockSize1;
  }
}



void na_AlignLayoutElement(
  NA_LayoutElement* elem,
  NARect blockRect,
  NABool horizontalIsRightToLeft,
  NABool verticalIsBottomToTop,
  NABool orderingVH)
{
  #if NA_DEBUG
    if(elem->uiElement) {
      na_PreserveDebugInfo(elem, blockRect);
    }
  #endif // NA_DEBUG

  // compute absolute values for blockSize and size.
  if(elem->blockSize1 == NA_LAYOUT_GROW) {
    elem->blockSize1 = (orderingVH) ? blockRect.size.height : blockRect.size.width;
  }
  if(elem->element.size1 == NA_LAYOUT_GROW) {
    elem->element.size1 = (orderingVH) ? blockRect.size.height : blockRect.size.width;
  }
  if(elem->blockSize2 == NA_LAYOUT_GROW) {
    elem->blockSize2 = (orderingVH) ? blockRect.size.width : blockRect.size.height;
  }
  if(elem->element.size2 == NA_LAYOUT_GROW) {
    elem->element.size2 = (orderingVH) ? blockRect.size.width : blockRect.size.height;
  }

  #if NA_DEBUG
    if(elem->blockSize1 != (orderingVH ? blockRect.size.height : blockRect.size.width))
      naError("blockSize should be the same");
    if(elem->blockSize2 != (orderingVH ? blockRect.size.width : blockRect.size.height))
      naError("blockSize should be the same");
  #endif // NA_DEBUG

  double alignMargin1;
  switch(elem->element.align1) {
  case NA_ALIGN_BEGIN:
    alignMargin1 = 0.;
    break;
  case NA_ALIGN_CENTER:
    if(orderingVH) {
      alignMargin1 = elem->element.alignBaseline
        ? blockRect.size.height - NA_LAYOUT_LINE_HEIGHT
        : naFloor((blockRect.size.height - elem->element.size1) * .5);
    }else{
      alignMargin1 = naFloor((blockRect.size.width - elem->element.size1) * .5);
    }
    break;
  case NA_ALIGN_END:
    if(orderingVH) {
      alignMargin1 = elem->element.alignBaseline
        ? blockRect.size.height - NA_LAYOUT_LINE_HEIGHT
        : blockRect.size.height - elem->element.size1;
    }else{
      alignMargin1 = blockRect.size.width - elem->element.size1;
    }
    break;
  }

  double alignMargin2;
  switch(elem->element.align2) {
  case NA_ALIGN_BEGIN:
    if(orderingVH) {
      alignMargin2 = blockRect.size.width - elem->element.size2;
    }else{
      alignMargin2 = elem->element.alignBaseline
        ? blockRect.size.height - NA_LAYOUT_LINE_HEIGHT
        : blockRect.size.height - elem->element.size2;
    }
    break;
  case NA_ALIGN_CENTER:
    if(orderingVH) {
      alignMargin2 = naFloor((blockRect.size.width - elem->element.size2) * .5);
    }else{
      alignMargin2 = elem->element.alignBaseline
        ? naFloor((blockRect.size.height - NA_LAYOUT_LINE_HEIGHT) * .5)
        : naFloor((blockRect.size.height - elem->element.size2) * .5);
    }
    break;
  case NA_ALIGN_END:
    alignMargin2 = 0.;
    break;
  }

  NARect innerBlockRect = naMakeRectS(
    blockRect.pos.x + (orderingVH ? alignMargin2 : alignMargin1),
    blockRect.pos.y + (orderingVH ? alignMargin1 : alignMargin2),
    elem->element.size1,
    elem->element.size2
  );

  // This is the block we want to use with our uiElement if available.
  NARect uiElementRect = innerBlockRect;

  // Finally, if a uiElement is available, add it to the parent space and
  // adjust the window if needed.
  if(elem->uiElement) {
    if(naGetUIElementType(elem->uiElement) == NA_UI_SPACE) {
      naSetSpaceLayoutDirections(elem->uiElement, elem->element.layoutDirections);
    }

    naSetUIElementRect(elem->uiElement, uiElementRect);
    if(!elem->parent) {
      NAWindow* window = naGetUIElementWindowMutable(elem->uiElement);
      if(window){
        NASpace* contentSpace = naGetWindowContentSpace(window);
        if(contentSpace == elem->uiElement){
          NARect windowRect = naGetUIElementRect(window);
          windowRect.size = uiElementRect.size;
          naSetUIElementRect(window, windowRect);
        }
      }
    }
  }

  // When the given element denotes a section or a new sub-layout, the given
  // uiElement must be an NASpace and childs will be added to that space.
  // Therefore, we reset the local coordinate system origin to zero.
  if(elem->uiElement) {
    innerBlockRect.pos = naMakePosZero();
  }

  // We subtract the padding around the rectangle.
  if(orderingVH) {
    innerBlockRect.pos.x += horizontalIsRightToLeft ? elem->padding.end2 : elem->padding.begin2;
    innerBlockRect.pos.y += verticalIsBottomToTop ? elem->padding.end1 : elem->padding.begin1;
    innerBlockRect.size.width -= elem->padding.begin2 + elem->padding.end2;
    innerBlockRect.size.height -= elem->padding.begin1 + elem->padding.end1;
  }else{
    innerBlockRect.pos.x += horizontalIsRightToLeft ? elem->padding.end1 : elem->padding.begin1;
    innerBlockRect.pos.y += verticalIsBottomToTop ? elem->padding.end2 : elem->padding.begin2;
    innerBlockRect.size.width -= elem->padding.begin1 + elem->padding.end1;
    innerBlockRect.size.height -= elem->padding.begin2 + elem->padding.end2;
  }

  // Now, innerBlockRect is the final rect we want to distribute our elements in.
  
  // Compute, how much flex space there is to distribute
  double totalFlexibleSpace2 = orderingVH
    ? blockRect.size.width - elem->minBlockSize2
    : blockRect.size.height - elem->minBlockSize2;

  // Go through all childs and count how many have to grow in secondary size.
  size_t elementsToFit = 0;
  NAListIterator countingIter = naMakeListMutator(&elem->childs);
  while(naIterateList(&countingIter)) {
    NA_LayoutElement* child = naGetListCurMutable(&countingIter);
    if(child->blockSize2 == 0.) { // NA_LAYOUT_GROW
      elementsToFit++;
    }
  }
  naClearListIterator(&countingIter);
  
  // We have all the information we need. Go through all childs and align them.
  
  NAListIterator iter = naMakeListMutator(&elem->childs);
  while(naIterateList(&iter)) {
    NA_LayoutElement* child = naGetListCurMutable(&iter);
    
    // Determine the secondary size which needs to be added to the child rect
    // due to distribution of flexible space.
    double flexSpace2 = 0.;
    if(child->blockSize2 == 0.) { // NA_LAYOUT_GROW
      // Compute it like that so that no rounding errors or left over sizes
      // occur.
      flexSpace2 = naFloor(totalFlexibleSpace2 / (double)elementsToFit);
      totalFlexibleSpace2 -= flexSpace2;
      elementsToFit--;
    }
    
    double childBlockSize2 = child->minBlockSize2 + flexSpace2;
    
    NARect subElemRect = naMakeRectS(
      innerBlockRect.pos.x,
      innerBlockRect.pos.y,
      orderingVH ? childBlockSize2 : innerBlockRect.size.width,
      orderingVH ? innerBlockRect.size.height : childBlockSize2);
 
    double marginedChildBlockSize2 = childBlockSize2 + child->margin.begin2;
        
    // Flip the secondary direction if necessary and adjust the rect of the
    // container as well as the child.
    if(orderingVH) {
      if(horizontalIsRightToLeft) {
        subElemRect.pos.x = naGetRectEndX(innerBlockRect) - marginedChildBlockSize2;
      }else{
        innerBlockRect.pos.x += marginedChildBlockSize2;
        subElemRect.pos.x += child->margin.begin2;
      }
      innerBlockRect.size.width -= marginedChildBlockSize2;
    }else{
      if(verticalIsBottomToTop) {
        innerBlockRect.pos.y += marginedChildBlockSize2;
        subElemRect.pos.y += child->margin.begin2;
      }else{
        subElemRect.pos.y = naGetRectEndY(innerBlockRect) - marginedChildBlockSize2;
      }
      innerBlockRect.size.height -= marginedChildBlockSize2;
    }

    // Finally, align the child.
    na_AlignLayoutSection(
      child,
      subElemRect,
      naGetLayoutDirectionsHorizontalIsRightToLeft(elem->element.layoutDirections),
      naGetLayoutDirectionsVerticalIsBottomToTop(elem->element.layoutDirections),
      naGetLayoutDirectionsPrimaryIsVertical(elem->element.layoutDirections));
  }
  naClearListIterator(&iter);
}
