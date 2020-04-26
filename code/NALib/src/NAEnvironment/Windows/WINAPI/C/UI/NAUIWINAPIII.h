
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#include "../../../../NAUICore.h"
#include "../../../../../NACore/NAValueHelper.h"



//#include <windows.h>
//#ifdef __gl_h_
//  #include <GL/GL.h>
//#endif



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

NA_HAPI NAUIElement** naGetUIElementNextTabReference(NATextField* textfield);
NA_HAPI NAUIElement** naGetUIElementPrevTabReference(NATextField* textfield);
NA_HAPI NAUIElement** naGetTextFieldNextTabReference(NATextField* textfield);
NA_HAPI NAUIElement** naGetTextFieldPrevTabReference(NATextField* textfield);
NA_HAPI NAUIElement** naGetTextBoxNextTabReference(NATextBox* textbox);
NA_HAPI NAUIElement** naGetTextBoxPrevTabReference(NATextBox* textbox);

HWND naGetApplicationOffscreenWindow(void);
NACoreUIElement* naGetApplicationMouseHoverElement(void);
void naSetApplicationMouseHoverElement(NACoreUIElement* element);
const NONCLIENTMETRICS* naGetApplicationMetrics(void);



NA_HDEF void naClearUINativeId(NANativeID nativeId){
    DestroyWindow(nativeId);
}


NA_HDEF void naSetUIElementParent(NAUIElement* uielement, NAUIElement* parent){
  NACoreUIElement* coreelement;
  NACoreUIElement* coreparent;
  NACoreWindow* window;

  #ifndef NDEBUG
    if(!uielement)
      naError("uielement is NULL");
  #endif
  coreelement = (NACoreUIElement*)uielement;
  coreparent = (NACoreUIElement*)parent;

  window = naGetUIElementWindow(uielement);
  if(window && naGetWindowFirstTabElement(window) == coreelement){
    naSetWindowFirstTabElement(window, NA_NULL);
  }

  if(!coreparent){
    HWND result;
    coreelement->parent = NA_NULL;
    result = SetParent(coreelement->nativeID, HWND_MESSAGE);
  }else{
    HWND result;
    coreelement->parent = parent;
    result = SetParent(coreelement->nativeID, coreparent->nativeID);
  }
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
  NAString* messageString;
  TCHAR* systemTitle;
  TCHAR* systemMessage;
  UINT iconType = MB_ICONINFORMATION;

  switch(alertBoxType){
  case NA_ALERT_BOX_INFO:    iconType = MB_ICONINFORMATION; break;
  case NA_ALERT_BOX_WARNING: iconType = MB_ICONWARNING; break;
  case NA_ALERT_BOX_ERROR:   iconType = MB_ICONERROR; break;
  }

  messageString = naNewStringWithFormat("%s" NA_NL "%s", titleText, infoText);
  systemTitle = naAllocSystemStringWithUTF8String(titleText);
  systemMessage = naAllocSystemStringWithUTF8String(naGetStringUTF8Pointer(messageString));

  MessageBox(NA_NULL, systemMessage, systemTitle, MB_OK | iconType);

  naDelete(messageString);
  naFree(systemMessage);
  naFree(systemTitle);
}


NA_HDEF void naCaptureKeyboardStatus(MSG* message){  
  NABool lShift;
  NABool rShift;
  NABool lControl;
  NABool rControl;
  NABool lOption;
  NABool rOption;
  NABool hasShift   = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_SHIFT);
  NABool hasControl = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_CONTROL);
  NABool hasOption  = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_OPTION);
  NABool hasCommand = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_COMMAND);
  NABool isExtendedKey = (message->lParam >> 24) & 0x01;  // Extended keys usually are the ones on the right.

  NAUIKeyCode scancode = (NAUIKeyCode)MapVirtualKey((UINT)message->wParam, MAPVK_VK_TO_VSC);
  na_app->keyboardStatus.keyCode = scancode;
  lShift = (GetKeyState(VK_LSHIFT) & 0x8000) >> 15;
  rShift = (GetKeyState(VK_RSHIFT) & 0x8000) >> 15;
  // Note: Due to the right shift key not properly being detected by the extended key flag
  // of lParam, we have to rely on GetKeyState. Pity.
  hasShift = lShift | rShift;
  lControl = (GetKeyState(VK_LCONTROL) & 0x8000) >> 15;
  rControl = (GetKeyState(VK_RCONTROL) & 0x8000) >> 15;
  hasControl = lControl | rControl;
  lOption = (GetKeyState(VK_LMENU) & 0x8000) >> 15;
  rOption = (GetKeyState(VK_RMENU) & 0x8000) >> 15;
  // Note: AltGr actually sends first an rControl and then an rOption. Don't know why.
  hasOption = lOption | rOption;
  hasCommand = scancode == NA_KEYCODE_LEFT_COMMAND || scancode == NA_KEYCODE_RIGHT_COMMAND;

  // Note, this implementation is far from finished. It does strange things, but that
  // just seems to be a thing with windows key mappings. :(

  na_app->keyboardStatus.modifiers = 0;
  na_app->keyboardStatus.modifiers |= hasShift * NA_MODIFIER_FLAG_SHIFT;
  na_app->keyboardStatus.modifiers |= hasControl * NA_MODIFIER_FLAG_CONTROL;
  na_app->keyboardStatus.modifiers |= hasOption * NA_MODIFIER_FLAG_OPTION;
  na_app->keyboardStatus.modifiers |= hasCommand * NA_MODIFIER_FLAG_COMMAND;
}



NA_HDEF NABool naInterceptKeyboardShortcut(MSG* message){
  NABool retvalue = NA_FALSE;
  if(message->message == WM_KEYUP || message->message == WM_SYSKEYDOWN || message->message == WM_SYSKEYUP){
    naCaptureKeyboardStatus(message);
  }else if(message->message == WM_KEYDOWN){
    NACoreUIElement* elem;
    HWND keyWindow;
    naCaptureKeyboardStatus(message);
    
    // Search for the native first responder which is represented in NALib.
    elem = NA_NULL;
    keyWindow = GetFocus();
    while(!elem && keyWindow){
      elem = naGetUINALibEquivalent(keyWindow);
      keyWindow = GetParent(keyWindow);
    }
    if(!elem){
      elem = naGetApplication();
    }

    // Search for a matching keyboard shortcut by bubbling.
    while(!retvalue && elem){
      NAListIterator iter = naMakeListAccessor(&(elem->shortcuts));
      while(!retvalue && naIterateList(&iter)){
        const NACoreKeyboardShortcutReaction* corereaction = naGetListCurConst(&iter);
        if(corereaction->shortcut.keyCode == na_app->keyboardStatus.keyCode){
          NABool needsShift   = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool needsControl = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool needsOption  = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool needsCommand = naGetFlagi(corereaction->shortcut.modifiers, NA_MODIFIER_FLAG_COMMAND);
          NABool hasShift   = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool hasControl = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool hasOption  = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool hasCommand = naGetFlagi(na_app->keyboardStatus.modifiers, NA_MODIFIER_FLAG_COMMAND);
          if(needsShift   == hasShift
          && needsControl == hasControl
          && needsOption  == hasOption
          && needsCommand == hasCommand){
            NAReaction reaction;
            reaction.uielement = elem;
            reaction.command = NA_UI_COMMAND_KEYBOARD_SHORTCUT;
            reaction.controller = corereaction->controller;
            retvalue = corereaction->handler(reaction);
          }
        }
      }
      naClearListIterator(&iter);
      elem = naGetUIElementParent(elem);
    }
  }
  return retvalue;
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
  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(hWnd);
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
  NABool handeled;
  NAPos pos;
  NASize size;
  NARect rect;
  const NAMouseStatus* mouseStatus;

  switch(message){
  case WM_MOUSEHOVER: // being inside the hWND for a specified amout of time.
    break;

  case WM_MOUSEMOVE:
    // wParam: several special keys
    // GET_X_LPARAM(lParam): x coord relative to top left
    // GET_Y_LPARAM(lParam): y coord relative to top left
    naWINAPICaptureMouseHover();

    size.width = GET_X_LPARAM(lParam);
    size.height = GET_Y_LPARAM(lParam);
    rect = naGetUIElementRect(uielement, naGetApplication(), NA_FALSE);
    size.width += rect.pos.x;
    size.height = rect.pos.y + rect.size.height - size.height;
    mouseStatus = naGetMouseStatus();
    pos = naGetMousePos(mouseStatus);
    naSetMouseMovedByDiff(size.width - pos.x, size.height - pos.y);

    handeled = naDispatchUIElementCommand(coreelement, NA_UI_COMMAND_MOUSE_MOVED);
    if(handeled){
      info.hasbeenhandeled = NA_TRUE;
      info.result = 0;
    }
    break;

  case WM_MOUSELEAVE:
    naWINAPICaptureMouseHover();
    break;

  case WM_KEYDOWN:
    handeled = naDispatchUIElementCommand(coreelement, NA_UI_COMMAND_KEYDOWN);
    if(handeled){
      info.hasbeenhandeled = NA_TRUE;
      info.result = 0;
    }
    break;

  case WM_KEYUP:
    handeled = naDispatchUIElementCommand(coreelement, NA_UI_COMMAND_KEYUP);
    if(handeled){
      info.hasbeenhandeled = NA_TRUE;
      info.result = 0;
    }
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
      //printf("Uncaught notification" NA_NL);
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
      //printf("Uncaught draw item message" NA_NL);
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
}



NA_HDEF NAUIElement** naGetUIElementNextTabReference(NAUIElement* uielement){
  switch(naGetUIElementType(uielement)){
  case NA_UI_TEXTFIELD: return naGetTextFieldNextTabReference(uielement); break;
  case NA_UI_TEXTBOX:   return naGetTextBoxNextTabReference(uielement); break;
  default:
    #ifndef NDEBUG
      naError("Invalid type");
    #endif
    return NA_NULL;
  }
}



NA_HDEF NAUIElement** naGetUIElementPrevTabReference(NAUIElement* uielement){
  switch(naGetUIElementType(uielement)){
  case NA_UI_TEXTFIELD: return naGetTextFieldPrevTabReference(uielement); break;
  case NA_UI_TEXTBOX:   return naGetTextBoxPrevTabReference(uielement); break;
  default:
    #ifndef NDEBUG
      naError("Invalid type");
    #endif
    return NA_NULL;
  }
}



NA_DEF void naSetUIElementNextTabElement(NAUIElement* elem, NAUIElement* nextTabElem){
  NAUIElement** elemNextRef;
  NAUIElement** nextPrevRef;
  NAUIElement** elemNextPrevRef;
  NAUIElement** nextPrevNextRef;

  #ifndef NDEBUG
    if(naGetUIElementWindow(elem) != naGetUIElementWindow(nextTabElem))
      naError("element do not share the same window.");
  #endif

  if(  naGetUIElementType(elem) != NA_UI_TEXTFIELD
    && naGetUIElementType(elem) != NA_UI_TEXTBOX){
    #ifndef NDEBUG
      naError("elem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  if(  naGetUIElementType(nextTabElem) != NA_UI_TEXTFIELD
    && naGetUIElementType(nextTabElem) != NA_UI_TEXTBOX){
    #ifndef NDEBUG
      naError("nextTabElem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  elemNextRef = naGetUIElementNextTabReference(elem);
  nextPrevRef = naGetUIElementPrevTabReference(nextTabElem);
  elemNextPrevRef = naGetUIElementPrevTabReference(*elemNextRef);
  nextPrevNextRef = naGetUIElementNextTabReference(*nextPrevRef);

  *nextPrevNextRef = *elemNextRef;
  *elemNextPrevRef = *nextPrevRef;
  *nextPrevRef = elem;
  *elemNextRef = nextTabElem;

  // This is how it is supposed to work on windows, but it doesn't.
  //SetWindowPos(((NACoreUIElement*)nextTabElem)->nativeID, ((NACoreUIElement*)elem)->nativeID, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
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

  return rect;
}



NA_HDEF void* naAllocMouseTracking(NANativeID nativeId){
  NABool success;
  TRACKMOUSEEVENT* winapitracking = naAlloc(TRACKMOUSEEVENT);
  winapitracking->cbSize = sizeof(TRACKMOUSEEVENT);
  winapitracking->dwFlags = TME_LEAVE;
  winapitracking->hwndTrack = nativeId;
  winapitracking->dwHoverTime = HOVER_DEFAULT;
  success = TrackMouseEvent(winapitracking);
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
  POINT origin = {0, 0};  // top left point which surely is on the main screen.
  screen = MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
  screeninfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(screen, &screeninfo);
  rect.pos.x = 0; // The main monitor is by definition at (0,0)
  rect.pos.y = 0; // which in NALib is the bottom left corner.
  rect.size.width = (double)screeninfo.rcMonitor.right - (double)screeninfo.rcMonitor.left;
  rect.size.height = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  return rect;
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
