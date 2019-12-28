
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!


#include "../../../../NAUICore.h"



//#include <windows.h>
//#ifdef __gl_h_
//  #include <GL/GL.h>
//#endif

#define CUB_WINDOW_IGNORE_MOUSE_WARP  0x01



NA_HAPI NARect naGetApplicationAbsoluteRect(void);
NA_HAPI NARect naGetScreenAbsoluteRect(NACoreUIElement* screen);
NA_HAPI NARect naGetWindowAbsoluteOuterRect(NACoreUIElement* window);
NA_HAPI NARect naGetSpaceAbsoluteInnerRect(NACoreUIElement* space);
NA_HDEF NARect naGetImageSpaceAbsoluteInnerRect(NACoreUIElement* imagespace);
NA_HDEF NARect naGetOpenGLSpaceAbsoluteInnerRect(NACoreUIElement* space);
NA_HAPI NARect naGetButtonAbsoluteInnerRect(NACoreUIElement* button);
NA_HAPI NARect naGetRadioAbsoluteInnerRect(NACoreUIElement* radio);
NA_HAPI NARect naGetCheckBoxAbsoluteInnerRect(NACoreUIElement* checkbox);
NA_HAPI NARect naGetLabelAbsoluteInnerRect(NACoreUIElement* label);
NA_HDEF NARect naGetTextFieldAbsoluteInnerRect(NACoreUIElement* textfield);
NA_HDEF NARect naGetTextBoxAbsoluteInnerRect(NACoreUIElement* textbox);

NA_HDEF NARect naGetWindowAbsoluteInnerRect(NACoreUIElement* window);

NA_HAPI void naRenewWindowMouseTracking(NACoreWindow* corewindow);
NA_HAPI void naClearWindowMouseTracking(NACoreWindow* corewindow);

HWND naGetApplicationOffscreenWindow(void);
NACoreUIElement* naGetApplicationMouseHoverElement(void);
void naSetApplicationMouseHoverElement(NACoreUIElement* element);
const NONCLIENTMETRICS* naGetApplicationMetrics(void);



typedef struct NAWINAPIOpenGLSpace NAWINAPIOpenGLSpace;
struct NAWINAPIOpenGLSpace {
  NACoreOpenGLSpace coreopenglspace;
  HGLRC hRC;    // The rendering context for OpenGL
};



NA_HDEF void naClearUINativeId(NANativeID nativeId){
    DestroyWindow(nativeId);
}


NA_HDEF void naSetUIElementParent(NAUIElement* uielement, NAUIElement* parent){
  if(parent == naGetApplication()){return;}
  NACoreUIElement* coreelement = (NACoreUIElement*)uielement;
  NACoreUIElement* coreparent = (NACoreUIElement*)parent;
  // todo: remove from old parent
  coreelement->parent = parent;
  HWND result = SetParent(coreelement->nativeID, coreparent->nativeID);
}


NA_HDEF NACoreUIElement* naGetUIElementCommonParent(NACoreUIElement* elem1, NACoreUIElement* elem2){
  NACoreUIElement* commonParent = NA_NULL;
  NACoreUIElement* tmpelem2;
  while(elem1){
    tmpelem2 = elem2;
    while(tmpelem2){
      if(elem1 == tmpelem2){commonParent = elem1; break;}
      tmpelem2 = naGetUIElementParent(tmpelem2);
    }
    if(commonParent){break;}
    elem1 = naGetUIElementParent(elem1);
  }
  return commonParent;
}



NA_HAPI void naBlockUIElementNotifications(NACoreUIElement* elem){
  #ifndef NDEBUG
    if(elem->allownotifications == NA_FALSE)
      naError("Element already blocks notifications");
  #endif
  elem->allownotifications = NA_FALSE;
}


NA_HAPI void naAllowUIElementNotifications(NACoreUIElement* elem){
  #ifndef NDEBUG
    if(elem->allownotifications == NA_TRUE)
      naError("Element already allows notifications");
  #endif
  elem->allownotifications = NA_TRUE;
}


NA_HAPI NABool naAreUIElementNotificationsAllowed(NACoreUIElement* elem){
  return elem->allownotifications;
}




NA_DEF void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText){
    //NSAlert* alert = NA_COCOA_AUTORELEASE([[NSAlert alloc] init]);

    //switch(alertBoxType){
    //case NA_ALERT_BOX_INFO:    alert.alertStyle = NSInformationalAlertStyle; break;
    //case NA_ALERT_BOX_WARNING: alert.alertStyle = NSAlertStyleWarning; break;
    //case NA_ALERT_BOX_ERROR:   alert.alertStyle = NSAlertStyleCritical; break;
    //}
    //
    //alert.messageText = [NSString stringWithUTF8String:titleText];
    //alert.informativeText = [NSString stringWithUTF8String:infoText];
    //[alert runModal];
}


NA_DEF void naSetUIElementNextTabElement(NAUIElement* elem, NAUIElement* nextelem){
  //naDefineNativeCocoaObject(NANativeTextField, nativeelem, elem);
  //naDefineNativeCocoaObject(NSView, nativeview, nextelem);
  //[nativeelem setNextKeyView:nativeview];
}





// ///////////////////////////////////
// WINDOW CALLBACK
// ///////////////////////////////////

typedef struct NAWINAPICallbackInfo NAWINAPICallbackInfo;
struct NAWINAPICallbackInfo{
  NABool hasbeenhandeled;
  LRESULT result;
};

WNDPROC naGetApplicationOldButtonWindowProc();
WNDPROC naGetApplicationOldRadioWindowProc();
WNDPROC naGetApplicationOldCheckBoxWindowProc();
WNDPROC naGetApplicationOldLabelWindowProc();
WNDPROC naGetApplicationOldTextFieldWindowProc();

// Prototypes of the WindowProc handlers
NAWINAPICallbackInfo naUIElementWINAPIProc  (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWINAPINotificationProc(WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWINAPIDrawItemProc(WPARAM wParam, LPARAM lParam);

NAWINAPICallbackInfo naApplicationWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWindowWINAPIProc     (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naSpaceWINAPIProc      (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naImageSpaceWINAPIProc (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
#if (NA_CONFIG_COMPILE_OPENGL == 1)
  NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
#endif
NAWINAPICallbackInfo naButtonWINAPIProc     (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naRadioWINAPIProc      (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naCheckBoxWINAPIProc   (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naLabelWINAPIProc      (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naTextFieldWINAPIProc  (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naTextBoxWINAPIProc    (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);

NAWINAPICallbackInfo naButtonWINAPINotify   (NAUIElement* uielement, WORD notificationCode);
NAWINAPICallbackInfo naCheckBoxWINAPINotify (NAUIElement* uielement, WORD notificationCode);
NAWINAPICallbackInfo naLabelWINAPINotify    (NAUIElement* uielement, WORD notificationCode);
NAWINAPICallbackInfo naTextFieldWINAPINotify(NAUIElement* uielement, WORD notificationCode);

NAWINAPICallbackInfo naButtonWINAPIDrawItem (NAUIElement* uielement, DRAWITEMSTRUCT* drawitemstruct);

// This is the one and only, master of destruction, defender of chaos and
// pimp of the century function handling all and everything in WINAPI.

LRESULT CALLBACK naWINAPIWindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  //PAINTSTRUCT ps; // Paint information. Needed for WM_PAINT messages
  //LRESULT retvalue;
  //NAUIKeyCode keycode;
  //NAUIKeyCode scancode;   // used as UINT, converted to NAUIKeyCode

  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(hWnd);
  //todo: this is a fallback but maybe it is not clever to use application for that?
  NAUIElementType firsttype = uielement ? naGetUIElementType(uielement) : NA_UI_APPLICATION;

  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  if(message == WM_COMMAND){
    info = naWINAPINotificationProc(wParam, lParam);
  }
  if(message == WM_DRAWITEM){
    info = naWINAPIDrawItemProc(wParam, lParam);
  }

  while(uielement && !info.hasbeenhandeled){
    info = naUIElementWINAPIProc(uielement, message, wParam, lParam);
    if(info.hasbeenhandeled){break;}

    switch(naGetUIElementType(uielement)){
    case NA_UI_APPLICATION: info = naApplicationWINAPIProc(uielement, message, wParam, lParam); break;
    case NA_UI_WINDOW:      info = naWindowWINAPIProc     (uielement, message, wParam, lParam); break;
    case NA_UI_SPACE:       info = naSpaceWINAPIProc      (uielement, message, wParam, lParam); break;
    case NA_UI_IMAGESPACE:  info = naImageSpaceWINAPIProc (uielement, message, wParam, lParam); break;
    #if (NA_CONFIG_COMPILE_OPENGL == 1)
      case NA_UI_OPENGLSPACE: info = naOpenGLSpaceWINAPIProc(uielement, message, wParam, lParam); break;
    #endif
    case NA_UI_BUTTON:      info = naButtonWINAPIProc     (uielement, message, wParam, lParam); break;
    case NA_UI_RADIO:       info = naRadioWINAPIProc      (uielement, message, wParam, lParam); break;
    case NA_UI_CHECKBOX:    info = naCheckBoxWINAPIProc   (uielement, message, wParam, lParam); break;
    case NA_UI_LABEL:       info = naLabelWINAPIProc      (uielement, message, wParam, lParam); break;
    case NA_UI_TEXTFIELD:   info = naTextFieldWINAPIProc  (uielement, message, wParam, lParam); break;
    case NA_UI_TEXTBOX:     info = naTextBoxWINAPIProc    (uielement, message, wParam, lParam); break;
    default: break;
    }
    uielement = naGetUIElementParent(uielement);
  }

  // If the event has not been handeled, hand it over to the default procedure.
  if(!info.hasbeenhandeled){
    switch(firsttype){
    case NA_UI_BUTTON:    info.result = CallWindowProc(naGetApplicationOldButtonWindowProc(), hWnd, message, wParam, lParam); break;
    case NA_UI_RADIO:     info.result = CallWindowProc(naGetApplicationOldRadioWindowProc(), hWnd, message, wParam, lParam); break;
    case NA_UI_CHECKBOX:  info.result = CallWindowProc(naGetApplicationOldCheckBoxWindowProc(), hWnd, message, wParam, lParam); break;
    case NA_UI_LABEL:     info.result = CallWindowProc(naGetApplicationOldLabelWindowProc(), hWnd, message, wParam, lParam); break;
    case NA_UI_TEXTFIELD: info.result = CallWindowProc(naGetApplicationOldTextFieldWindowProc(), hWnd, message, wParam, lParam); break;
    default:
      info.result = DefWindowProc(hWnd, message, wParam, lParam);
      break;
    }
  }
  return info.result;

  ////if((message >= WM_USER) && (message <= 0x7fff)){
  ////  // User defined message
  ////  naDispatchUIElementCommand(uielement, ???, wParam);
  ////  return 0;
  ////}

  //switch (message) {

  //case WM_PAINT:
  //  // wParam: Unused
  //  // lParam: Unused
  //  if (uielement->elementtype == NA_UI_OPENGLSPACE) {
  //    BeginPaint(hWnd, &ps);

  //    //if(uielement is opengl)
  //    //        wglMakeCurrent(GetDC(naGetUIElementNativeID(&(openglspace->uielement))), openglspace->hRC);

  //    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW);
  //    //uielement->refreshrequested = NA_FALSE;
  //    EndPaint(hWnd, &ps);
  //  }
  //  break;

  //case WM_TIMER:
  //  // Note: Does not work with hWnd == NULL. Will not be called here.
  //  // wParam: timer identifier as an UINT
  //  // lParam: callback function. Unused in NALib
  //  //hasbeenhandeled = naExecuteApplicationTimer((UINT)wParam);
  //  break;

  //case WM_KEYDOWN:
  //  // wParam: virtual key code
  //  // lParam: several values, attributes, flags, ...
  //  keycode = wParam;
  //  scancode = (NAUIKeyCode)MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
  //  hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYDOWN);
  //  break;

  //case WM_KEYUP:
  //  // wParam: virtual key code
  //  // lParam: several values, attributes, flags, ...
  //  keycode = wParam;
  //  scancode = MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
  //  hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYUP);
  //  break;

  //case WM_MOUSELEAVE:
  //  //  window->mouseLeave();
  //  break;

  //case WM_NOTIFY:
  //  //  uielement = window->getUIElement(((LPNMHDR)lParam)->hwndFrom);
  //  //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(((LPNMHDR)lParam)->code, lParam);}
  //  break;

  //case WM_CLOSE:
  //  //  window->close();
  //  //  if(!::gui->numwindows){PostQuitMessage(0);}
  //  break;

  //default:
  //  break;

  //}

  //if(hasbeenhandeled) {
  //  retvalue = 0;
  //}else{
  //  retvalue = DefWindowProc(hWnd, message, wParam, lParam);
  //}
  //return retvalue;
}


void naWINAPICaptureMouseHover(){
  DWORD msgpos;
  POINT pt;
  HWND hWndUnderMouse;
  NACoreUIElement* elementUnderMouse;
  NACoreUIElement* curElement;
  NACoreUIElement* commonParent;

  msgpos = GetMessagePos();
  pt.x = GET_X_LPARAM(msgpos);
  pt.y = GET_Y_LPARAM(msgpos);
  hWndUnderMouse = WindowFromPoint(pt);
  elementUnderMouse = (NACoreUIElement*)naGetUINALibEquivalent(hWndUnderMouse);
  curElement = naGetApplicationMouseHoverElement();

  if(curElement != elementUnderMouse){
    commonParent = naGetUIElementCommonParent(curElement, elementUnderMouse);

    // Send a leave reaction to all elements which are not hovered anymore.
    while(curElement && curElement != commonParent){
      curElement->mouseinside = NA_FALSE;
      naDispatchUIElementCommand(curElement, NA_UI_COMMAND_MOUSE_EXITED);
      curElement = naGetUIElementParent(curElement);
    }

    // Reset the hover element to the current one and track the mouse leaving it.
    naSetApplicationMouseHoverElement(elementUnderMouse);
    if(elementUnderMouse){
    TRACKMOUSEEVENT winapitracking;
      winapitracking.cbSize = sizeof(TRACKMOUSEEVENT);
      winapitracking.dwFlags = TME_LEAVE;
      winapitracking.hwndTrack = naGetUIElementNativeID(elementUnderMouse);
      winapitracking.dwHoverTime = HOVER_DEFAULT;
      TrackMouseEvent(&winapitracking);
    }

    // Send the entered message to all elements which are newly hovered.
    while(elementUnderMouse && elementUnderMouse != commonParent){
      elementUnderMouse->mouseinside = NA_TRUE;
      naDispatchUIElementCommand(elementUnderMouse, NA_UI_COMMAND_MOUSE_ENTERED);
      elementUnderMouse = naGetUIElementParent(elementUnderMouse);
    }
  }
}


NAWINAPICallbackInfo naUIElementWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NACoreUIElement* coreelement = (NACoreUIElement*)uielement;
  //DWORD msgpos;
  //POINT pt;
  //HWND hWndUnderMouse;
  //NACoreUIElement* elementUnderMouse;
  //NACoreUIElement* commonParent;

  switch(message){
  case WM_MOUSEHOVER: // being inside the hWND for a specified amout of time.
    break;

  case WM_MOUSEMOVE:
    //if(!coreelement->mouseinside){
    //  //msgpos = GetMessagePos();
    //  //pt.x = GET_X_LPARAM(msgpos);
    //  //pt.y = GET_Y_LPARAM(msgpos);
    //  // beware! This returns the HWND being under RIGHT NOW! If you are debugging this, you will get a HWND of your current IDE!
    //  //hWndUnderMouse = WindowFromPoint(pt);
    //  //elementUnderMouse = (NACoreUIElement*)naGetUINALibEquivalent(hWndUnderMouse);

    //  if(elementUnderMouse){
    //    // Go through the whole tree and send an enter to all elements up until the common parent.
    //    while(coreelement && !coreelement->mouseinside){
    //      coreelement->mouseinside = NA_TRUE;
    //      naDispatchUIElementCommand(coreelement, NA_UI_COMMAND_MOUSE_ENTERED);
    //      coreelement = naGetUIElementParent(coreelement);
    //    }
    //  }

    //  // Reset the core element to the current one and track the mouse leaving it.
    //  coreelement = (NACoreUIElement*)uielement;
    //  TRACKMOUSEEVENT winapitracking;
    //  winapitracking.cbSize = sizeof(TRACKMOUSEEVENT);
    //  winapitracking.dwFlags = TME_LEAVE;
    //  winapitracking.hwndTrack = naGetUIElementNativeID(uielement);
    //  winapitracking.dwHoverTime = HOVER_DEFAULT;
    //  TrackMouseEvent(&winapitracking);
    //}

    //// Important: Not not mark this as handeled. Selecting text will not
    //// work if WM_MOUSEMOVE is not propagated.

    ////info.hasbeenhandeled = NA_TRUE;
    ////info.result = 0;

    naWINAPICaptureMouseHover();
    break;

  case WM_MOUSELEAVE:
    //msgpos = GetMessagePos();
    //pt.x = GET_X_LPARAM(msgpos);
    //pt.y = GET_Y_LPARAM(msgpos);
    //// beware! This returns the HWND being under RIGHT NOW! If you are debugging this, you will get a HWND of your current IDE!
    //hWndUnderMouse = WindowFromPoint(pt);
    //elementUnderMouse = (NACoreUIElement*)naGetUINALibEquivalent(hWndUnderMouse);
    //commonParent = naGetUIElementCommonParent(uielement, elementUnderMouse);
    //
    //if(elementUnderMouse){
    //  // Go through the whole tree and send an exit to all elements up until the common parent.
    //  while(coreelement != commonParent){
    //    //coreelement->mouseinside = NA_FALSE;
    //    //naDispatchUIElementCommand(coreelement, NA_UI_COMMAND_MOUSE_EXITED);
    //    //coreelement = naGetUIElementParent(coreelement);
    //  }
    //}

    // Currently, do not set this message as handeled. We don't know yet what
    // windows does with its controls.
    //info.hasbeenhandeled = NA_TRUE;
    //info.result = 0;
    
    naWINAPICaptureMouseHover();
    break;

  }

  return info;
}



NAWINAPICallbackInfo naWINAPINotificationProc(WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  WORD notificationCode = HIWORD(wParam);
  WORD controlIdentifier = LOWORD(wParam);
  HWND controlWnd = (HWND)lParam;
  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(controlWnd);

  if(uielement && naAreUIElementNotificationsAllowed(uielement)){
    switch(naGetUIElementType(uielement)){
    case NA_UI_BUTTON:    info = naButtonWINAPINotify   (uielement, notificationCode); break;
    case NA_UI_CHECKBOX:  info = naCheckBoxWINAPINotify (uielement, notificationCode); break;
    case NA_UI_LABEL:     info = naLabelWINAPINotify    (uielement, notificationCode); break;
    case NA_UI_TEXTFIELD: info = naTextFieldWINAPINotify(uielement, notificationCode); break;
    default:
      //printf("Uncaught notification\n");
      break;
    }
  }
  return info;
}

NAWINAPICallbackInfo naWINAPIDrawItemProc(WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  DRAWITEMSTRUCT* drawitemstruct = (DRAWITEMSTRUCT*)lParam;
  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(drawitemstruct->hwndItem);

  if(uielement){
    switch(naGetUIElementType(uielement)){
    case NA_UI_BUTTON:     info = naButtonWINAPIDrawItem    (uielement, drawitemstruct); break;
    default:
      //printf("Uncaught draw item message\n");
      break;
    }
  }
  return info;
}



// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////

NA_HDEF void naRefreshUIElementNow(NAUIElement* uielement){
  RedrawWindow(naGetUIElementNativeID(uielement), NA_NULL, NA_NULL, RDW_INVALIDATE | RDW_ERASE);
  //PostMessage(naGetUIElementNativeID(uielement), WM_PAINT, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
}






NA_HDEF NARect naGetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetScreenAbsoluteRect(NACoreUIElement* screen){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_DEF NARect naGetUIElementRect(NACoreUIElement* uielement, NAUIElement* relativeelement, NABool includebounds){
  NARect rect;
  NARect relrect;
  NACoreUIElement* coreelement;
  NACoreUIElement* corerelelement;
  NAApplication* app;

  coreelement = (NACoreUIElement*)uielement;
  corerelelement = (NACoreUIElement*)relativeelement;
  app = naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(coreelement == (NACoreUIElement*)app){
    #ifndef NDEBUG
      if(relativeelement && (relativeelement != app))
        naError("The relative element is invalid for the given uielement, which seems to be the application.");
    #endif
    return naGetApplicationAbsoluteRect();
  }

  // Now, we find the appropriate relative element.
  if(!corerelelement){corerelelement = naGetUIElementParent(coreelement);}

  switch(coreelement->elementtype){
  case NA_UI_APPLICATION: rect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = naGetScreenAbsoluteRect(coreelement); break;
  case NA_UI_WINDOW:
    if(includebounds){
      rect = naGetWindowAbsoluteOuterRect(coreelement);
    }else{
      rect = naGetWindowAbsoluteInnerRect(coreelement);
    }
    break;
  case NA_UI_OPENGLSPACE:  rect = naGetSpaceAbsoluteInnerRect(coreelement); break;
  default:
    //#ifndef NDEBUG
    //  naError("Invalid UI type");
    //#endif
    rect = naMakeRectSE(0., 0., 0., 0.);
    break;
  }

  if(corerelelement){
    switch(corerelelement->elementtype){
    case NA_UI_APPLICATION: relrect = naGetApplicationAbsoluteRect(); break;
    case NA_UI_SCREEN:      relrect = naGetScreenAbsoluteRect(corerelelement); break;
    case NA_UI_WINDOW:      relrect = naGetWindowAbsoluteInnerRect(corerelelement); break;
    case NA_UI_OPENGLSPACE:  relrect = naGetSpaceAbsoluteInnerRect(corerelelement); break;
    default:
      #ifndef NDEBUG
        naError("Invalid UI type");
      #endif
      relrect = naMakeRectSE(0., 0., 0., 0.);
      break;
    }

    rect.pos.x = rect.pos.x - relrect.pos.x;
    rect.pos.y = rect.pos.y - relrect.pos.y;
    rect.size.width = rect.size.width;
    rect.size.height = rect.size.height;
  }
  // Convert the rect into absolute coordinates.

  return rect;
}



NA_HDEF void* naAllocMouseTracking(NANativeID nativeId){
  TRACKMOUSEEVENT* winapitracking = naAlloc(TRACKMOUSEEVENT);
  winapitracking->cbSize = sizeof(TRACKMOUSEEVENT);
  winapitracking->dwFlags = TME_LEAVE;
  winapitracking->hwndTrack = nativeId;
  winapitracking->dwHoverTime = HOVER_DEFAULT;
  NABool success = TrackMouseEvent(winapitracking);
  //DWORD lasterror = GetLastError();
  return winapitracking;
}



NA_HDEF void naDeallocMouseTracking(void* tracking){
  TRACKMOUSEEVENT* winapitracking = (TRACKMOUSEEVENT*)tracking;
  winapitracking->dwFlags |= TME_CANCEL;
  TrackMouseEvent(winapitracking);
  naFree(winapitracking);
}



NA_API NARect naGetMainScreenRect(){
  HMONITOR screen;
  MONITORINFO screeninfo;
  NARect rect;
  POINT origin = {0, 0};
  screen = MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
  screeninfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(screen, &screeninfo);
  rect.pos.x = screeninfo.rcMonitor.left;
  rect.pos.y = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  rect.size.width = (double)screeninfo.rcMonitor.right - (double)screeninfo.rcMonitor.left;
  rect.size.height = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  return rect;
}


//int CALLBACK enumFonts(
//  _In_ ENUMLOGFONT   *lpelf,
//  _In_ NEWTEXTMETRIC *lpntm,
//  _In_ DWORD         FontType,
//  _In_ LPARAM        lParam
//){
//  int x = 1234;
//  printf("%s\n", lpelf->elfFullName);
//}

NAFont getFontWithKind(NAFontKind kind){
  HFONT font = NA_NULL;
  #if NA_CONFIG_USE_WINDOWS_COMMON_CONTROLS_6 == 1

  const NONCLIENTMETRICS* metrics = naGetApplicationMetrics();

  //EnumFontFamilies(GetDC(NA_NULL), NA_NULL, enumFonts, NA_NULL);

//  CGFloat systemSize = [NSFont systemFontSize];
  switch(kind){
    case NA_FONT_KIND_SYSTEM:
      font = CreateFont(
        metrics->lfMessageFont.lfHeight,
        0,
        0,
        0,
        FW_NORMAL,
        NA_FALSE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        metrics->lfMessageFont.lfFaceName);
      break;
    case NA_FONT_KIND_TITLE:
      font = CreateFont(
        metrics->lfMessageFont.lfHeight,
        0,
        0,
        0,
        FW_BOLD,
        NA_FALSE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        metrics->lfMessageFont.lfFaceName);
      break;
    case NA_FONT_KIND_MONOSPACE:
      font = CreateFont(
        metrics->lfMessageFont.lfHeight,
        0,
        0,
        0,
        FW_BOLD,
        NA_FALSE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Courier New"));
      break;
    case NA_FONT_KIND_PARAGRAPH:
      font = CreateFont(
        metrics->lfMessageFont.lfHeight,
        0,
        0,
        0,
        FW_BOLD,
        NA_FALSE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Palatino Linotype"));
      break;
    case NA_FONT_KIND_MATH:
      font = CreateFont(
        metrics->lfMessageFont.lfHeight,
        0,
        0,
        0,
        FW_BOLD,
        NA_TRUE,
        NA_FALSE,
        NA_FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        TEXT("Times New Roman"));
      break;
    default:
      #ifndef NDEBUG
        naError("Unknown font kind");
      #endif
      break;
  }
  #endif
    //// destroy font with DeleteObject(font);
  return (NAFont)font;
}


long getWINAPITextAlignmentWithAlignment(NATextAlignment alignment){
  long winapialignment;
  switch(alignment){
  case NA_TEXT_ALIGNMENT_LEFT: winapialignment = ES_LEFT; break;
  case NA_TEXT_ALIGNMENT_RIGHT: winapialignment = ES_RIGHT; break;
  case NA_TEXT_ALIGNMENT_CENTER: winapialignment = ES_CENTER; break;
  default:
    #ifndef NDEBUG
      naError("Invalid alignment enumeration");
    #endif
    winapialignment = ES_LEFT; break;
    break;
  }
  return winapialignment;
}


//void MBUIElement::setEnabled(MBBool enabled){
//  SendMessage(hWnd, WM_ENABLE, (WPARAM)enabled, 0);
//}
//
//
//
//void MBUIElement::leftMouseUp(int32 x, int32 y, int32 modifierkeys){
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseMove(int32 x, int32 y, int32 modifierkeys){
//  if(mousex == -1){
//    TRACKMOUSEEVENT tme;
//    tme.cbSize = sizeof(TRACKMOUSEEVENT);
//    tme.dwFlags = TME_LEAVE;
//    tme.hwndTrack = hWnd;
//    tme.dwHoverTime = HOVER_DEFAULT;
//    TrackMouseEvent(&tme);
//  }
//
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseLeave(){
//  mousex = -1;
//  mousey = -1;
//
//  TRACKMOUSEEVENT tme;
//  tme.cbSize = sizeof(TRACKMOUSEEVENT);
//  tme.dwFlags = TME_CANCEL;
//  tme.hwndTrack = hWnd;
//  tme.dwHoverTime = HOVER_DEFAULT;
//  TrackMouseEvent(&tme);
//}




//NA_DEF NARect naGetWindowRect(NAWindow* window){
//  NARect rect;
//  WINDOWINFO windowinfo;
//  windowinfo.cbSize = sizeof(WINDOWINFO);
//  GetWindowInfo(naGetUIElementNativeID(window), &windowinfo);
//  rect.size.width = windowinfo.rcWindow.right - windowinfo.rcWindow.left;
//  rect.size.height = windowinfo.rcWindow.bottom - windowinfo.rcWindow.top;
//  return rect;
//}









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
