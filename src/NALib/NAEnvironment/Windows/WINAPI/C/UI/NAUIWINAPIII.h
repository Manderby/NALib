
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




typedef struct NAWINAPIOpenGLSpace NAWINAPIOpenGLSpace;
struct NAWINAPIOpenGLSpace {
  NACoreOpenGLSpace coreopenglspace;
  HGLRC hRC;    // The rendering context for OpenGL
};



NA_HDEF void naClearUINativeId(NANativeID nativeId){
  //NA_COCOA_DISPOSE(nativeId);
}


NA_HDEF void naSetUIElementParent(NAUIElement* uielement, NAUIElement* parent){
  if(parent == naGetApplication()){return;}
  NACoreUIElement* coreelement = (NACoreUIElement*)uielement;
  NACoreUIElement* coreparent = (NACoreUIElement*)parent;
  // todo: remove from old parent
  coreelement->parent = parent;
  HWND result = SetParent(coreelement->nativeID, coreparent->nativeID);
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

// Prototypes of the WindowProc handlers
NABool naApplicationWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naWindowWINAPIProc     (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naSpaceWINAPIProc      (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naImageSpaceWINAPIProc (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
#if (NA_CONFIG_COMPILE_OPENGL == 1)
  NABool naOpenGLSpaceWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
#endif
NABool naButtonWINAPIProc     (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naRadioWINAPIProc      (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naCheckBoxWINAPIProc   (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naLabelWINAPIProc      (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naTextFieldWINAPIProc  (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);
NABool naTextBoxWINAPIProc    (NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam);

// This is the one and only, master of destruction, defender of chaos and
// pimp of the century function handling all and everything in WINAPI.

LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps; // Paint information. Needed for WM_PAINT messages
  NABool hasbeenhandeled = NA_FALSE;
  LRESULT retvalue;
  NAUIKeyCode keycode;
  NAUIKeyCode scancode;   // used as UINT, converted to NAUIKeyCode

  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(hWnd);

  if(uielement){
    switch(naGetUIElementType(uielement)){
    case NA_UI_APPLICATION: hasbeenhandeled = naApplicationWINAPIProc(uielement, message, wParam, lParam); break;
    case NA_UI_WINDOW:      hasbeenhandeled = naWindowWINAPIProc     (uielement, message, wParam, lParam); break;
    case NA_UI_SPACE:       hasbeenhandeled = naSpaceWINAPIProc      (uielement, message, wParam, lParam); break;
    case NA_UI_IMAGESPACE:  hasbeenhandeled = naImageSpaceWINAPIProc (uielement, message, wParam, lParam); break;
    #if (NA_CONFIG_COMPILE_OPENGL == 1)
      case NA_UI_OPENGLSPACE: hasbeenhandeled = naOpenGLSpaceWINAPIProc(uielement, message, wParam, lParam); break;
    #endif
    case NA_UI_BUTTON:      hasbeenhandeled = naButtonWINAPIProc     (uielement, message, wParam, lParam); break;
    case NA_UI_RADIO:       hasbeenhandeled = naRadioWINAPIProc      (uielement, message, wParam, lParam); break;
    case NA_UI_CHECKBOX:    hasbeenhandeled = naCheckBoxWINAPIProc   (uielement, message, wParam, lParam); break;
    case NA_UI_LABEL:       hasbeenhandeled = naLabelWINAPIProc      (uielement, message, wParam, lParam); break;
    case NA_UI_TEXTFIELD:   hasbeenhandeled = naTextFieldWINAPIProc  (uielement, message, wParam, lParam); break;
    case NA_UI_TEXTBOX:     hasbeenhandeled = naTextBoxWINAPIProc    (uielement, message, wParam, lParam); break;
    default: break;
    }
  }

  // If the event has not been handeled, hand it over to the default procedure.
  return hasbeenhandeled ? 0 : DefWindowProc(hWnd, message, wParam, lParam);




  //if((message >= WM_USER) && (message <= 0x7fff)){
  //  // User defined message
  //  naDispatchUIElementCommand(uielement, ???, wParam);
  //  return 0;
  //}

  switch (message) {

  case WM_PAINT:
    // wParam: Unused
    // lParam: Unused
    if (uielement->elementtype == NA_UI_OPENGLSPACE) {
      BeginPaint(hWnd, &ps);

      //if(uielement is opengl)
      //        wglMakeCurrent(GetDC(naGetUIElementNativeID(&(openglspace->uielement))), openglspace->hRC);

      hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW);
      //uielement->refreshrequested = NA_FALSE;
      EndPaint(hWnd, &ps);
    }
    break;

  case WM_CREATE:
    hasbeenhandeled = NA_FALSE;
    //  if(window->eraseBackground()){
    //    DefWindowProc(hWnd, message, wParam, lParam);
    //  }else{
    //  }
    break;

  case WM_ERASEBKGND:
    //hasbeenhandeled = NA_TRUE;
    //  if(window->eraseBackground()){
    //    DefWindowProc(hWnd, message, wParam, lParam);
    //  }else{
    //  }
    break;

  case WM_TIMER:
    // Note: Does not work with hWnd == NULL. Will not be called here.
    // wParam: timer identifier as an UINT
    // lParam: callback function. Unused in NALib
    //hasbeenhandeled = naExecuteApplicationTimer((UINT)wParam);
    break;

  case WM_HSCROLL:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(LOWORD(wParam), 0);} // note that this is the loword
    break;

  case WM_KEYDOWN:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = (NAUIKeyCode)MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYDOWN);
    break;

  case WM_KEYUP:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYUP);
    break;

  case WM_MOUSELEAVE:
    //  window->mouseLeave();
    break;

  case WM_LBUTTONUP:
    //  window->leftMouseUp(LOWORD(lParam), HIWORD(lParam), wParam);
    break;

  case WM_COMMAND:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(HIWORD(wParam), 0);}
    break;

  case WM_NOTIFY:
    //  uielement = window->getUIElement(((LPNMHDR)lParam)->hwndFrom);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(((LPNMHDR)lParam)->code, lParam);}
    break;

  case WM_USER:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(HIWORD(wParam), 0);}
    break;

  case WM_CLOSE:
    //  window->close();
    //  if(!::gui->numwindows){PostQuitMessage(0);}
    break;

  default:
    break;

  }

  if(hasbeenhandeled) {
    retvalue = 0;
  }else{
    retvalue = DefWindowProc(hWnd, message, wParam, lParam);
  }
  return retvalue;
}






HWND naGetApplicationOffscreenWindow(void);
const NONCLIENTMETRICS* naGetApplicationMetrics(void);



// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////

NA_HDEF void naRefreshUIElementNow(NAUIElement* uielement){
  PostMessage(naGetUIElementNativeID(uielement), WM_PAINT, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
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



NA_API NARect naGetMainScreenRect(){
  HMONITOR screen;
  MONITORINFO screeninfo;
  NARect rect;
  screen = MonitorFromWindow(NA_NULL, MONITOR_DEFAULTTOPRIMARY);
  screeninfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(screen, &screeninfo);
  rect.pos.x = screeninfo.rcMonitor.left;
  rect.pos.y = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  rect.size.width = (double)screeninfo.rcMonitor.right - (double)screeninfo.rcMonitor.left;
  rect.size.height = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  return rect;
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
