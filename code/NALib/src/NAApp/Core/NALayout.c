
#include "NAAppCore.h"
#include "../NAApp.h"
#include "../../NAStruct/NAList.h"



//   /-margin-\/-padding-\/------- block --------\
//
//   +--------------------------------------------------+
//   |         +-------------------------------------+  |
//   |         |          +-----------------------+  |  |
//   |         |          |           A           |  |  |
//   |         |          |           |           |  |  |
//   |         |          |           V           |  |  |
//   |         |          |      +=========+      |  |  |
//   |         |          | <--> | content | <--> |  |  |
//   |         |          |      +=========+      |  |  |
//   |         |          |           A           |  |  |
//   |         |          |           |           |  |  |
//   |         |          |           V           |  |  |
//   |         |          +-----------------------+  |  |
//   |         +-------------------------------------+  |
//   +--------------------------------------------------+
//



typedef struct NA_LayoutElement NA_LayoutElement;
struct NA_LayoutElement {
  NA_LayoutElement* parent;
  NAList childs;
  void* uiElement;
  NABool isSecondary;

  double contentSize1;
  double contentSize2;
  double blockSize1;
  double blockSize2;
  double minPaddingSize1;
  double minPaddingSize2;
  NAAlignment alignment1;
  NAAlignment alignment2;
  NABool alignBaseline;
  NABorder2D margin;
  NABorder2D padding;
};






void na_EndLayoutElement(NA_LayoutElement* elem);
void na_AlignLayoutElement(
  NASpace* layoutingSpace,
  NA_LayoutElement* elem,
  NARect rect);
NASpace* na_GetLayoutingSpace(NA_LayoutElement* elem);

NA_LayoutElement* na_curLayoutElement = NA_NULL;




NA_HDEF void na_EndLastSectionElement() {
  #if NA_DEBUG
    if(!na_curLayoutElement->isSecondary)
      naError("This function must be used with sections");
  #endif // NA_DEBUG

  if(!naIsListEmpty(&na_curLayoutElement->childs)) {
    NA_LayoutElement* lastElement = naGetListLastMutable(&na_curLayoutElement->childs);
    na_EndLayoutElement(lastElement);
  }
}


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

  elem->contentSize1 = NA_LAYOUT_GROW;
  elem->contentSize2 = NA_LAYOUT_GROW;
  elem->blockSize1 = NA_LAYOUT_GROW;
  elem->blockSize2 = NA_LAYOUT_GROW;
  elem->minPaddingSize1 = 0.; // not yet computed
  elem->minPaddingSize2 = 0.; // not yet computed
  elem->alignment1 = NA_ALIGN_CENTER;
  elem->alignment2 = NA_ALIGN_CENTER;
  elem->alignBaseline = NA_TRUE;
  elem->margin = naMakeBorder2D(0., 0., 0., 0.);
  elem->padding = padding;

  return elem;
}



void na_DeallocLayoutElement(NA_LayoutElement* elem) {
  naClearList(&elem->childs, (NAMutator)na_DeallocLayoutElement);
  naFree(elem);
}



void naBeginLayout(NASpace* space, NABorder2D padding) {
  #if NA_DEBUG
    if(na_curLayoutElement == NA_NULL && !space)
      naError("For a new layout, space must not be a nullptr.");
    if(na_curLayoutElement && !na_curLayoutElement->isSecondary)
      naError("Use naBeginLayout either for a new layout or when working within a section.");
  #endif // NA_DEBUG

  // First, we set the parent-child relationship right.
  if(na_curLayoutElement && space) {
    na_AddSpaceChildUnpositioned(
      na_GetLayoutingSpace(na_curLayoutElement),
      space);
  }

  NA_LayoutElement* newElement = na_AllocLayoutElement(
    na_curLayoutElement,
    space,
    padding,
    NA_FALSE);

  newElement->alignBaseline = NA_FALSE;

  if(na_curLayoutElement) {
    // We create a sub-layout
    // End the last element of the current section if available
    na_EndLastSectionElement();
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
    // Here, one might be tempted to end the current element but that will
    // acutally happen automatically as soon as the containing section will
    // be closed. Therefore, simply move up to the containing section.
    na_curLayoutElement = na_curLayoutElement->parent;
  }else{
    // na_curLayoutElement is the root element. Finalize the element.
    na_EndLayoutElement(na_curLayoutElement);

    NABool orderingVH = naGetSpaceLayoutDirectionsPrimaryIsVertical(na_curLayoutElement->uiElement);

    // Reaching here, all min sizes of the whole design are computed. Propagate
    // the information down the whole tree and align all elements properly.
    na_AlignLayoutElement(
      na_GetLayoutingSpace(na_curLayoutElement),
      na_curLayoutElement,
      naMakeRectS(
        0.,
        0.,
        orderingVH ? na_curLayoutElement->minPaddingSize2 : na_curLayoutElement->minPaddingSize1,
        orderingVH ? na_curLayoutElement->minPaddingSize1 : na_curLayoutElement->minPaddingSize2));
    
    // Finally, deallocate everything.
    na_DeallocLayoutElement(na_curLayoutElement);
    na_curLayoutElement = NA_NULL;
  }
}



NASpace* na_GetLayoutingSpace(NA_LayoutElement* elem) {
  if(elem->uiElement && naGetUIElementType(elem->uiElement) == NA_UI_SPACE) {
    return elem->uiElement;
  }
  while(1) {
    elem = elem->parent;
    if(elem->uiElement) {
      #if NA_DEBUG
        if(naGetUIElementType(elem->uiElement) != NA_UI_SPACE)
          naError("Wrong element type");
      #endif
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
    // Currently working on another section. Close that one first.
    na_EndLayoutElement(na_curLayoutElement);
    // Move up to the parent.
    na_curLayoutElement = na_curLayoutElement->parent;
  }

  NA_LayoutElement* subElem = na_AllocLayoutElement(
    na_curLayoutElement,
    NA_NULL,
    padding,
    NA_TRUE);
  subElem->blockSize2 = blockSize2;

  naAddListLastMutable(&na_curLayoutElement->childs, subElem);
  na_curLayoutElement = subElem;
}



void na_EndLayoutElement(NA_LayoutElement* elem) {
  // If this is a section, end the last element if available
  if(elem->isSecondary) {
    na_EndLastSectionElement();
  }
  
  // We compute the minimal size of content of this element. That contains
  // a potential uiElement as well as all childs.
  
  double minContentSize1 = 0;
  double minContentSize2 = 0;

  // We try to find the minimal space necessary for the padding sizes
  // of all childs.
  
  // Go through all childs and sum up the minimal required padding sizes and
  // the margins of those.
  NAListIterator iter = naMakeListAccessor(&elem->childs);
  while(naIterateList(&iter)) {
    const NA_LayoutElement* child = naGetListCurConst(&iter);
    double childMinSize1 = child->margin.begin1 + child->minPaddingSize1 + child->margin.end1;
    double childMinSize2 = child->margin.begin2 + child->minPaddingSize2 + child->margin.end2;
    if(elem->isSecondary) {
      minContentSize1 += childMinSize1;
      if(childMinSize2 > minContentSize2) {
        minContentSize2 = childMinSize2;
      }
    }else{
      if(childMinSize1 > minContentSize1) {
        minContentSize1 = childMinSize1;
      }
      minContentSize2 += childMinSize2;
    }
  }
  naClearListIterator(&iter);

  // If this element uses an NASpace as uiElement and the primary and secondary
  // layout directions of that are different to the directions of the space
  // this element is contained in, we need to swap the sizes.
  if(elem->parent && naGetSpaceLayoutDirectionsPrimaryIsVertical(na_GetLayoutingSpace(elem)) != naGetSpaceLayoutDirectionsPrimaryIsVertical(na_GetLayoutingSpace(elem->parent))) {
    double tmpSize = minContentSize1;
    minContentSize1 = minContentSize2;
    minContentSize2 = tmpSize;
  }
  
  // If this element declares a fixed content or even a fixed block size, that
  // overrides the values.
  // Note that we do not check whether a fixed size is smaller than the minimal
  // requirement computed above as there are situations like uiElements having
  // a size bigger than a lineHeight by default even when coming from the
  // native system implementation.
  if(elem->contentSize1 > 0.) {
    minContentSize1 = elem->contentSize1;
  }
  if(elem->contentSize2 > 0.) {
    minContentSize2 = elem->contentSize2;
  }
  if(elem->blockSize1 > 0.) {
    minContentSize1 = elem->blockSize1;
  }
  if(elem->blockSize2 > 0.) {
    minContentSize2 = elem->blockSize2;
  }
  
  #if NA_DEBUG
    if(minContentSize1 == 0.)
      naError("Can not compute minimal size 1 for this element");
    if(minContentSize2 == 0.)
      naError("Can not compute minimal size 2 for this element");
  #endif
  
  // Add the padding of this section.
  elem->minPaddingSize1 = minContentSize1 + elem->padding.begin1 + elem->padding.end1;
  elem->minPaddingSize2 = minContentSize2 + elem->padding.begin2 + elem->padding.end2;
}



void na_PreserveDebugInfo(const NASpace* layoutingSpace, NA_LayoutElement* elem, NARect paddingRect) {
  NA_LayoutRects* layoutRects = naAlloc(NA_LayoutRects);
  
  NABool horizontalIsRightToLeft = naGetSpaceLayoutDirectionsHorizontalIsRightToLeft(layoutingSpace);
  NABool verticalIsBottomToTop = naGetSpaceLayoutDirectionsVerticalIsBottomToTop(layoutingSpace);
  NABool orderingVH = naGetSpaceLayoutDirectionsPrimaryIsVertical(layoutingSpace);

  layoutRects->marginRect = paddingRect;
  if(orderingVH) {
    layoutRects->marginRect.pos.x -= horizontalIsRightToLeft ? elem->margin.end2 : elem->margin.begin2;
    layoutRects->marginRect.pos.y -= verticalIsBottomToTop   ? elem->margin.begin1 : elem->margin.end1;
    layoutRects->marginRect.size.width  += (elem->margin.begin2 + elem->margin.end2);
    layoutRects->marginRect.size.height += (elem->margin.begin1 + elem->margin.end1);
  }else{
    layoutRects->marginRect.pos.x -= horizontalIsRightToLeft ? elem->margin.end1 : elem->margin.begin1;
    layoutRects->marginRect.pos.y -= verticalIsBottomToTop   ? elem->margin.begin2 : elem->margin.end2;
    layoutRects->marginRect.size.width  += (elem->margin.begin1 + elem->margin.end1);
    layoutRects->marginRect.size.height += (elem->margin.begin2 + elem->margin.end2);
  }

  layoutRects->paddingRect = paddingRect;
  
  layoutRects->contentRect = paddingRect;
  if(orderingVH) {
    layoutRects->contentRect.pos.x += horizontalIsRightToLeft ? elem->padding.end2 : elem->padding.begin2;
    layoutRects->contentRect.pos.y += verticalIsBottomToTop   ? elem->padding.begin1 : elem->padding.end1;
    layoutRects->contentRect.size.width  -= (elem->padding.begin2 + elem->padding.end2);
    layoutRects->contentRect.size.height -= (elem->padding.begin1 + elem->padding.end1);
  }else{
    layoutRects->contentRect.pos.x += horizontalIsRightToLeft ? elem->padding.end1 : elem->padding.begin1;
    layoutRects->contentRect.pos.y += verticalIsBottomToTop   ? elem->padding.begin2 : elem->padding.end2;
    layoutRects->contentRect.size.width  -= (elem->padding.begin1 + elem->padding.end1);
    layoutRects->contentRect.size.height -= (elem->padding.begin2 + elem->padding.end2);
  }

  if(((NA_UIElement*)elem->uiElement)->layoutRects) {
    naFree(((NA_UIElement*)elem->uiElement)->layoutRects);
  }
  ((NA_UIElement*)elem->uiElement)->layoutRects = layoutRects;
}



void naAddLayoutElement(
  void* uiElement,
  double preMargin1,
  double contentSize1,
  double contentSize2)
{
  #if NA_DEBUG
    if(!na_curLayoutElement)
      naError("No layout in progress. Use naBeginLayout.");
    if(!na_curLayoutElement->isSecondary)
      naError("Use naAddLayoutSection before adding Elements.");
  #endif // NA_DEBUG

  // End the last element of the current section if available
  na_EndLastSectionElement();
  
  NASpace* layoutingSpace = na_GetLayoutingSpace(na_curLayoutElement);
  
  if(uiElement) {
    na_AddSpaceChildUnpositioned(
      layoutingSpace,
      uiElement);
  }

  NABool orderingVH = naGetSpaceLayoutDirectionsPrimaryIsVertical(layoutingSpace);
  NARect contentRect = uiElement
    ? naGetUIElementRect(uiElement)
    : naMakeRectZero();

  NA_LayoutElement* subElem = na_AllocLayoutElement(
    na_curLayoutElement,
    uiElement,
    naMakeBorder2D(0., 0., 0., 0.),
    NA_FALSE);
  
  // compute the element primary size.
  if(contentSize1 < 0.) { // NA_LAYOUT_MIN
    #if NA_DEBUG
      if(!uiElement)
        naError("Using NA_LAYOUT_MIN without a uiElement will shrink to zero.");
    #endif // NA_DEBUG
    subElem->contentSize1 = (orderingVH)
      ? contentRect.size.height
      : contentRect.size.width;
  }else{
    subElem->contentSize1 = contentSize1;
  }
  
  // By default, the blockSize1 is set to the same as contentSize1.
  subElem->blockSize1 = subElem->contentSize1;

  // compute the element secondary size.
  if(contentSize2 < 0.) { // NA_LAYOUT_MIN
    #if NA_DEBUG
      if(!uiElement)
        naError("Using NA_LAYOUT_MIN without a uiElement will shrink to zero.");
    #endif // NA_DEBUG
    subElem->contentSize2 = (orderingVH)
      ? contentRect.size.width
      : contentRect.size.height;
  }else{
    subElem->contentSize2 = contentSize2;
  }

  subElem->margin.begin1 = preMargin1;
  
  if(uiElement && !naIsUIElementBlock(uiElement)) {
    subElem->alignBaseline = NA_TRUE;
  }else{
    subElem->alignBaseline = NA_FALSE;
  }
  
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
      na_GetLayoutingSpace(na_curLayoutElement),
      space);
  }

  na_curLayoutElement->uiElement = space;
}



void naSetLayoutElementPrimaryAlign(NAAlignment alignment1) {
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

  lastChild->alignment1 = alignment1;
}



void naSetLayoutElementSecondaryAlign(NAAlignment alignment2) {
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

  lastChild->alignment2 = alignment2;
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

  lastChild->alignBaseline = alignBaseline;
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
  NASpace* layoutingSpace,
  NA_LayoutElement* elem,
  NARect paddingRect)
{
  NABool horizontalIsRightToLeft = naGetSpaceLayoutDirectionsHorizontalIsRightToLeft(layoutingSpace);
  NABool verticalIsBottomToTop = naGetSpaceLayoutDirectionsVerticalIsBottomToTop(layoutingSpace);
  NABool orderingVH = naGetSpaceLayoutDirectionsPrimaryIsVertical(layoutingSpace);
  
  // Compute the actual size of the padded content of the element.
  
  double paddedContentSize1;
  double paddedContentSize2;
  
  if(elem->contentSize1 < 0.) { // NA_LAYOUT_MIN
    paddedContentSize1 = elem->minPaddingSize1;
  }else if(elem->contentSize1 == NA_LAYOUT_GROW) {
    paddedContentSize1 = (orderingVH) ? paddingRect.size.height : paddingRect.size.width;
  }else{
    paddedContentSize1 = elem->contentSize1 + elem->padding.begin1 + elem->padding.end1;
  }
  if(elem->contentSize2 < 0.) { // NA_LAYOUT_MIN
    paddedContentSize2 = elem->minPaddingSize2;
  } if(elem->contentSize2 == NA_LAYOUT_GROW) {
    paddedContentSize2 = (orderingVH) ? paddingRect.size.width : paddingRect.size.height;
  }else{
    paddedContentSize2 = elem->contentSize2 + elem->padding.begin2 + elem->padding.end2;
  }

  // Properly align in both directions if the content size is smaller than the
  // block size.
  
  double alignMargin1;
  double alignMargin2;

  switch(elem->alignment1) {
  case NA_ALIGN_BEGIN:
    if(orderingVH) {
      alignMargin1 = verticalIsBottomToTop
        ? paddingRect.size.height - paddedContentSize1
        : 0.;
    }else{
      alignMargin1 = horizontalIsRightToLeft
        ? paddingRect.size.width - paddedContentSize1
        : 0.;
    }
    break;
  case NA_ALIGN_CENTER:
    if(orderingVH) {
      alignMargin1 = ((paddingRect.size.height - paddedContentSize1) * .5);
    }else{
      if(orderingVH) {
        alignMargin1 = naFloor((paddingRect.size.height - paddedContentSize1) * .5);
      }else{
        alignMargin1 = naFloor((paddingRect.size.width  - paddedContentSize1) * .5);
      }
    }
    break;
  case NA_ALIGN_END:
    if(orderingVH) {
      alignMargin1 = verticalIsBottomToTop 
        ? 0.
        : paddingRect.size.height - paddedContentSize1;
    }else{
      alignMargin1 = horizontalIsRightToLeft
        ? 0.
        : paddingRect.size.width - paddedContentSize1;
    }
    break;
  }

  switch(elem->alignment2) {
  case NA_ALIGN_BEGIN:
    if(orderingVH) {
      alignMargin2 = horizontalIsRightToLeft
        ? paddingRect.size.width - paddedContentSize2
        : 0.;
    }else{
      alignMargin2 = verticalIsBottomToTop
        ? paddingRect.size.height - paddedContentSize2
        : 0.;
    }
    break;
  case NA_ALIGN_CENTER:
    if(orderingVH) {
      alignMargin2 = naFloor((paddingRect.size.width  - paddedContentSize2) * .5);
    }else{
      alignMargin2 = naFloor((paddingRect.size.height - paddedContentSize2) * .5);
    }
    break;
  case NA_ALIGN_END:
    if(orderingVH) {
      alignMargin2 = horizontalIsRightToLeft
        ? 0.
        : paddingRect.size.width - paddedContentSize2;
    }else{
      alignMargin2 = verticalIsBottomToTop
        ? 0.
        : paddingRect.size.height - paddedContentSize2;
    }
    break;
  }

  // Based on these informations, compute the content Rect.

  NARect paddedContentRect = naMakeRectS(
    paddingRect.pos.x + (orderingVH ? alignMargin2 : alignMargin1),
    paddingRect.pos.y + (orderingVH ? alignMargin1 : alignMargin2),
    orderingVH ? paddedContentSize2 : paddedContentSize1,
    orderingVH ? paddedContentSize1 : paddedContentSize2
  );

  // Place the uiElement and adjust the window if needed.
  if(elem->uiElement) {
  
//    if(elem->alignBaseline) {
//      naSetUIElementRect(elem->uiElement, paddedContentRect);
//    }else{
      naSetUIElementRectRaw(elem->uiElement, paddedContentRect);
//    }
  
    if(!elem->parent) {
      NAWindow* window = naGetUIElementWindowMutable(elem->uiElement);
      if(window){
        NASpace* contentSpace = naGetWindowContentSpace(window);
        if(contentSpace == elem->uiElement){
          NARect windowRect = naGetUIElementRect(window);
          windowRect.size = paddedContentRect.size;
          naSetUIElementRect(window, windowRect);
        }
      }
    }
  }

  // When the given element denotes a section or a new sub-layout, the given
  // uiElement must be an NASpace and childs will be added to that space.
  // Therefore, we reset the local coordinate system origin to zero.
  if(elem->uiElement && (naGetUIElementType(elem->uiElement) == NA_UI_SPACE)) {
    paddedContentRect.pos = naMakePosZero();
  }

  #if NA_DEBUG
    if(elem->uiElement) {
      na_PreserveDebugInfo(layoutingSpace, elem, paddedContentRect);
    }
  #endif // NA_DEBUG

  // Remove the padding to get the final contentRect in which we want to
  // distribute our elements in.

  NARect contentRect;
  if(orderingVH) {
    contentRect = naMakeRectS(
      paddedContentRect.pos.x + (horizontalIsRightToLeft ? elem->padding.end2 : elem->padding.begin2),
      paddedContentRect.pos.y + (verticalIsBottomToTop   ? elem->padding.begin1 : elem->padding.end1),
      paddedContentRect.size.width  - elem->padding.begin2 - elem->padding.end2,
      paddedContentRect.size.height - elem->padding.begin1 - elem->padding.end1);
  }else{
    contentRect = naMakeRectS(
      paddedContentRect.pos.x + (horizontalIsRightToLeft ? elem->padding.end1 : elem->padding.begin1),
      paddedContentRect.pos.y + (verticalIsBottomToTop   ? elem->padding.begin2 : elem->padding.end2),
      paddedContentRect.size.width  - elem->padding.begin1 - elem->padding.end1,
      paddedContentRect.size.height - elem->padding.begin2 - elem->padding.end2);
  }

  // From here on, contentRect becomes a working rect and denotes the remaining
  // space available for each of the child elements.
  
  // Compute, how much flex space there is to distribute
  double totalFlexibleSpace1 = orderingVH
    ? paddingRect.size.height - elem->minPaddingSize1
    : paddingRect.size.width  - elem->minPaddingSize1;
  double totalFlexibleSpace2 = orderingVH
    ? paddingRect.size.width  - elem->minPaddingSize2
    : paddingRect.size.height - elem->minPaddingSize2;

  // Go through all childs and count how many have to grow in size.
  size_t elementsToFit1 = 0;
  size_t elementsToFit2 = 0;
  NAListIterator countingIter = naMakeListMutator(&elem->childs);
  while(naIterateList(&countingIter)) {
    NA_LayoutElement* child = naGetListCurMutable(&countingIter);
    if(child->blockSize1 == 0.) { // NA_LAYOUT_GROW
      elementsToFit1++;
    }
    if(child->blockSize2 == 0.) { // NA_LAYOUT_GROW
      elementsToFit2++;
    }
  }
  naClearListIterator(&countingIter);
  
  NABool distributeVertically = (elem->isSecondary && orderingVH) || (!elem->isSecondary && !orderingVH);
  
  // We have all the information we need. Go through all childs and align them.
  
  NAListIterator iter = naMakeListMutator(&elem->childs);
  while(naIterateList(&iter)) {
    NA_LayoutElement* child = naGetListCurMutable(&iter);
    
    // Determine the size which needs to be added to the child rect due to
    // distribution of flexible space. Note that the computation is done like
    // so that no rounding errors or left over sizes occur.
    double flexSpace1 = 0.;
    double flexSpace2 = 0.;
    
    if(child->blockSize1 == 0.) { // NA_LAYOUT_GROW
      flexSpace1 = naFloor(totalFlexibleSpace1 / (double)elementsToFit1);
      totalFlexibleSpace1 -= flexSpace1;
      elementsToFit1--;
    }
    if(child->blockSize2 == 0.) { // NA_LAYOUT_GROW
      flexSpace2 = naFloor(totalFlexibleSpace2 / (double)elementsToFit2);
      totalFlexibleSpace2 -= flexSpace2;
      elementsToFit2--;
    }
    
    NARect childMarginRect;
    childMarginRect.size.width = orderingVH
      ? child->minPaddingSize2 + flexSpace2 + (child->margin.begin2 + child->margin.end2)
      : child->minPaddingSize1 + flexSpace1 + (child->margin.begin1 + child->margin.end1);
    childMarginRect.size.height = orderingVH
      ? child->minPaddingSize1 + flexSpace1 + (child->margin.begin1 + child->margin.end1)
      : child->minPaddingSize2 + flexSpace2 + (child->margin.begin2 + child->margin.end2);
      
    // Set the fixed block size1 if this is a primary element
    if(!elem->isSecondary){
      if(orderingVH) {
        childMarginRect.size.height = contentRect.size.height;
      }else{
        childMarginRect.size.width = contentRect.size.width;
      }
    }
    // Set the fixed block size2 if this is a secondary element
    if(elem->isSecondary){
      if(orderingVH) {
        childMarginRect.size.width = contentRect.size.width;
      }else{
        childMarginRect.size.height = contentRect.size.height;
      }
    }
    
    // set the child to the proper position based on the layout direction and
    // adjust the working rect too.
    if(horizontalIsRightToLeft) {
      childMarginRect.pos.x = naGetRectEndX(contentRect) - childMarginRect.size.width;
      if(!distributeVertically) {
        contentRect.size.width -= childMarginRect.size.width;
      }
    }else{
      childMarginRect.pos.x = contentRect.pos.x;
      if(!distributeVertically) {
        contentRect.pos.x += childMarginRect.size.width;
        contentRect.size.width -= childMarginRect.size.width;
      }
    }
    if(verticalIsBottomToTop) {
      childMarginRect.pos.y = contentRect.pos.y;
      if(distributeVertically) {
        contentRect.pos.y += childMarginRect.size.height;
        contentRect.size.height -= childMarginRect.size.height;
      }
    }else{
      childMarginRect.pos.y = naGetRectEndY(contentRect) - childMarginRect.size.height;
      if(distributeVertically) {
        contentRect.size.height -= childMarginRect.size.height;
      }
    }
    
    // Subtract the margin again.
    NARect childPaddingRect = childMarginRect;
    if(orderingVH) {
      childPaddingRect.pos.x += horizontalIsRightToLeft ? child->margin.end2 : child->margin.begin2;
      childPaddingRect.pos.y += verticalIsBottomToTop   ? child->margin.begin1 : child->margin.end1;
      childPaddingRect.size.width  -= (child->margin.begin2 + child->margin.end2);
      childPaddingRect.size.height -= (child->margin.begin1 + child->margin.end1);
    }else{
      childPaddingRect.pos.x += horizontalIsRightToLeft ? child->margin.end1 : child->margin.begin1;
      childPaddingRect.pos.y += verticalIsBottomToTop   ? child->margin.begin2 : child->margin.end2;
      childPaddingRect.size.width  -= (child->margin.begin1 + child->margin.end1);
      childPaddingRect.size.height -= (child->margin.begin2 + child->margin.end2);
    }

    // Finally, align the child.
    na_AlignLayoutElement(
      na_GetLayoutingSpace(child),
      child,
      childPaddingRect);
  }
  naClearListIterator(&iter);
}
