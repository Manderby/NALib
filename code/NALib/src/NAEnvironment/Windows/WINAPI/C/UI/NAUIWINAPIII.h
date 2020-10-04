
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#include "../../../../NAUICore.h"
#include "../../../../../NAValueHelper.h"



//#include <windows.h>
//#ifdef __gl_h_
//  #include <GL/GL.h>
//#endif



NA_HAPI NARect na_GetApplicationAbsoluteRect(void);
NA_HAPI NARect na_GetScreenAbsoluteRect(NA_UIElement* screen);
NA_HAPI NARect na_GetWindowAbsoluteOuterRect(NA_UIElement* window);
NA_HAPI NARect na_GetSpaceAbsoluteInnerRect(NA_UIElement* space);
NA_HAPI NARect na_GetImageSpaceAbsoluteInnerRect(NA_UIElement* imageSpace);
NA_HAPI NARect na_GetOpenGLSpaceAbsoluteInnerRect(NA_UIElement* space);
NA_HAPI NARect na_GetButtonAbsoluteInnerRect(NA_UIElement* button);
NA_HAPI NARect na_GetRadioAbsoluteInnerRect(NA_UIElement* radio);
NA_HAPI NARect na_GetCheckBoxAbsoluteInnerRect(NA_UIElement* checkBox);
NA_HAPI NARect na_GetLabelAbsoluteInnerRect(NA_UIElement* label);
NA_HAPI NARect na_GetTextFieldAbsoluteInnerRect(NA_UIElement* textField);
NA_HAPI NARect na_GetTextBoxAbsoluteInnerRect(NA_UIElement* textBox);

NA_HAPI void** na_GetUIElementNextTabReference(void* textField);
NA_HAPI void** na_GetUIElementPrevTabReference(void* textField);
NA_HAPI void** na_GetTextFieldNextTabReference(NATextField* textField);
NA_HAPI void** na_GetTextFieldPrevTabReference(NATextField* textField);
NA_HAPI void** na_GetTextBoxNextTabReference(NATextBox* textBox);
NA_HAPI void** na_GetTextBoxPrevTabReference(NATextBox* textBox);

HWND naGetApplicationOffscreenWindow(void);
NA_UIElement* naGetApplicationMouseHoverElement(void);
void naSetApplicationMouseHoverElement(NA_UIElement* element);
const NONCLIENTMETRICS* naGetApplicationMetrics(void);



NA_HDEF void na_ClearUINativeId(NANativeId nativeId){
    DestroyWindow(nativeId);
}


NA_HDEF void na_SetUIElementParent(void* uiElement, void* parent){
  NA_UIElement* elem;
  NA_UIElement* parentElem;
  NAWindow* window;

  #ifndef NDEBUG
    if(!uiElement)
      naError("uiElement is NULL");
  #endif
    elem = (NA_UIElement*)uiElement;
    parentElem = (NA_UIElement*)parent;

  window = naGetUIElementWindow(uiElement);
  if(window && naGetWindowFirstTabElement(window) == elem){
    naSetWindowFirstTabElement(window, NA_NULL);
  }

  if(!parentElem){
    HWND result;
    elem->parent = NA_NULL;
    result = SetParent(elem->nativeId, HWND_MESSAGE);
  }else{
    HWND result;
    #ifndef NDEBUG
    if(!elem)
      naCrash("elem is NULL");
    #endif

    elem->parent = parent;
    result = SetParent(elem->nativeId, parentElem->nativeId);
  }
}


NA_HDEF NA_UIElement* na_GetUIElementCommonParent(NA_UIElement* elem1, NA_UIElement* elem2){
  NA_UIElement* commonParent = NA_NULL;
  NA_UIElement* tmpelem2;
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



NA_HDEF void na_BlockUIElementNotifications(NA_UIElement* elem){
  #ifndef NDEBUG
    if(elem->allowNotifications == NA_FALSE)
      naError("Element already blocks notifications");
  #endif
  elem->allowNotifications = NA_FALSE;
}


NA_HDEF void na_AllowUIElementNotifications(NA_UIElement* elem){
  #ifndef NDEBUG
    if(elem->allowNotifications == NA_TRUE)
      naError("Element already allows notifications");
  #endif
  elem->allowNotifications = NA_TRUE;
}


NA_HDEF NABool na_AreUIElementNotificationsAllowed(NA_UIElement* elem){
  return elem->allowNotifications;
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


NA_HDEF void na_CaptureKeyboardStatus(MSG* message){  
  NABool lShift;
  NABool rShift;
  NABool lControl;
  NABool rControl;
  NABool lOption;
  NABool rOption;
  NABool hasShift   = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_SHIFT);
  NABool hasControl = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_CONTROL);
  NABool hasOption  = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_OPTION);
  NABool hasCommand = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_COMMAND);
  NABool isExtendedKey = (message->lParam >> 24) & 0x01;  // Extended keys usually are the ones on the right.

  NAUIKeyCode scanCode = (NAUIKeyCode)MapVirtualKey((UINT)message->wParam, MAPVK_VK_TO_VSC);
  na_App->keyboardStatus.keyCode = scanCode;
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
  hasCommand = scanCode == NA_KEYCODE_LEFT_COMMAND || scanCode == NA_KEYCODE_RIGHT_COMMAND;

  // Note, this implementation is far from finished. It does strange things, but that
  // just seems to be a thing with windows key mappings. :(

  na_App->keyboardStatus.modifiers = 0;
  na_App->keyboardStatus.modifiers |= hasShift * NA_MODIFIER_FLAG_SHIFT;
  na_App->keyboardStatus.modifiers |= hasControl * NA_MODIFIER_FLAG_CONTROL;
  na_App->keyboardStatus.modifiers |= hasOption * NA_MODIFIER_FLAG_OPTION;
  na_App->keyboardStatus.modifiers |= hasCommand * NA_MODIFIER_FLAG_COMMAND;
}



NA_HDEF NABool na_InterceptKeyboardShortcut(MSG* message){
  NABool retvalue = NA_FALSE;
  if(message->message == WM_KEYUP || message->message == WM_SYSKEYDOWN || message->message == WM_SYSKEYUP){
    na_CaptureKeyboardStatus(message);
  }else if(message->message == WM_KEYDOWN){
    NA_UIElement* elem;
    HWND keyWindow;
    na_CaptureKeyboardStatus(message);
    
    // Search for the native first responder which is represented in NALib.
    elem = NA_NULL;
    keyWindow = GetFocus();
    while(!elem && keyWindow){
      elem = na_GetUINALibEquivalent(keyWindow);
      keyWindow = GetParent(keyWindow);
    }
    if(!elem){
      elem = &(naGetApplication()->uiElement);
    }

    // Search for a matching keyboard shortcut by bubbling.
    while(!retvalue && elem){
      NAListIterator iter = naMakeListAccessor(&(elem->shortcuts));
      while(!retvalue && naIterateList(&iter)){
        const NAKeyboardShortcutReaction* keyReaction = naGetListCurConst(&iter);
        if(keyReaction->shortcut.keyCode == na_App->keyboardStatus.keyCode){
          NABool needsShift   = naGetFlagi(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool needsControl = naGetFlagi(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool needsOption  = naGetFlagi(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool needsCommand = naGetFlagi(keyReaction->shortcut.modifiers, NA_MODIFIER_FLAG_COMMAND);
          NABool hasShift   = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_SHIFT);
          NABool hasControl = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_CONTROL);
          NABool hasOption  = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_OPTION);
          NABool hasCommand = naGetFlagi(na_App->keyboardStatus.modifiers, NA_MODIFIER_FLAG_COMMAND);
          if(needsShift   == hasShift
          && needsControl == hasControl
          && needsOption  == hasOption
          && needsCommand == hasCommand){
            NAReaction reaction;
            reaction.uiElement = elem;
            reaction.command = NA_UI_COMMAND_KEYBOARD_SHORTCUT;
            reaction.controller = keyReaction->controller;
            retvalue = keyReaction->handler(reaction);
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
NAWINAPICallbackInfo naUIElementWINAPIProc  (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWINAPINotificationProc(WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWINAPIDrawItemProc(WPARAM wParam, LPARAM lParam);

NAWINAPICallbackInfo naApplicationWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWindowWINAPIProc     (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naSpaceWINAPIProc      (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naImageSpaceWINAPIProc (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
#if (NA_COMPILE_OPENGL == 1)
  NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
#endif
NAWINAPICallbackInfo naButtonWINAPIProc     (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naRadioWINAPIProc      (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naCheckBoxWINAPIProc   (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naLabelWINAPIProc      (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naTextFieldWINAPIProc  (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naTextBoxWINAPIProc    (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);

NAWINAPICallbackInfo naButtonWINAPINotify   (void* uiElement, WORD notificationCode);
NAWINAPICallbackInfo naCheckBoxWINAPINotify (void* uiElement, WORD notificationCode);
NAWINAPICallbackInfo naLabelWINAPINotify    (void* uiElement, WORD notificationCode);
NAWINAPICallbackInfo naTextFieldWINAPINotify(void* uiElement, WORD notificationCode);

NAWINAPICallbackInfo naButtonWINAPIDrawItem (void* uiElement, DRAWITEMSTRUCT* drawitemstruct);



// This is the one and only, master of destruction, defender of chaos and
// pimp of the century function handling all and everything in WINAPI.

LRESULT CALLBACK naWINAPIWindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(hWnd);
  NAUIElementType firsttype = uiElement ? naGetUIElementType(uiElement) : NA_UI_APPLICATION;

  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  if(message == WM_COMMAND){
    info = naWINAPINotificationProc(wParam, lParam);
  }
  if(message == WM_DRAWITEM){
    info = naWINAPIDrawItemProc(wParam, lParam);
  }

  while(uiElement && !info.hasbeenhandeled){
    info = naUIElementWINAPIProc(uiElement, message, wParam, lParam);
    if(info.hasbeenhandeled){break;}

    switch(naGetUIElementType(uiElement)){
    case NA_UI_APPLICATION: info = naApplicationWINAPIProc(uiElement, message, wParam, lParam); break;
    case NA_UI_WINDOW:      info = naWindowWINAPIProc     (uiElement, message, wParam, lParam); break;
    case NA_UI_SPACE:       info = naSpaceWINAPIProc      (uiElement, message, wParam, lParam); break;
    case NA_UI_IMAGESPACE:  info = naImageSpaceWINAPIProc (uiElement, message, wParam, lParam); break;
    #if (NA_COMPILE_OPENGL == 1)
      case NA_UI_OPENGLSPACE: info = naOpenGLSpaceWINAPIProc(uiElement, message, wParam, lParam); break;
    #endif
    case NA_UI_BUTTON:      info = naButtonWINAPIProc     (uiElement, message, wParam, lParam); break;
    case NA_UI_RADIO:       info = naRadioWINAPIProc      (uiElement, message, wParam, lParam); break;
    case NA_UI_CHECKBOX:    info = naCheckBoxWINAPIProc   (uiElement, message, wParam, lParam); break;
    case NA_UI_LABEL:       info = naLabelWINAPIProc      (uiElement, message, wParam, lParam); break;
    case NA_UI_TEXTFIELD:   info = naTextFieldWINAPIProc  (uiElement, message, wParam, lParam); break;
    case NA_UI_TEXTBOX:     info = naTextBoxWINAPIProc    (uiElement, message, wParam, lParam); break;
    default: break;
    }
    uiElement = naGetUIElementParent(uiElement);
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
  NA_UIElement* elementUnderMouse;
  NA_UIElement* curElement;
  NA_UIElement* commonParent;

  msgpos = GetMessagePos();
  pt.x = GET_X_LPARAM(msgpos);
  pt.y = GET_Y_LPARAM(msgpos);
  hWndUnderMouse = WindowFromPoint(pt);
  elementUnderMouse = (NA_UIElement*)na_GetUINALibEquivalent(hWndUnderMouse);
  curElement = naGetApplicationMouseHoverElement();

  if(curElement != elementUnderMouse){
    commonParent = na_GetUIElementCommonParent(curElement, elementUnderMouse);

    // Send a leave reaction to all elements which are not hovered anymore.
    while(curElement && curElement != commonParent){
      curElement->mouseInside = NA_FALSE;
      na_DispatchUIElementCommand(curElement, NA_UI_COMMAND_MOUSE_EXITED);
      curElement = naGetUIElementParent(curElement);
    }

    // Reset the hover element to the current one and track the mouse leaving it.
    naSetApplicationMouseHoverElement(elementUnderMouse);
    if(elementUnderMouse){
    TRACKMOUSEEVENT winapiTracking;
      winapiTracking.cbSize = sizeof(TRACKMOUSEEVENT);
      winapiTracking.dwFlags = TME_LEAVE;
      winapiTracking.hwndTrack = naGetUIElementNativeId(elementUnderMouse);
      winapiTracking.dwHoverTime = HOVER_DEFAULT;
      TrackMouseEvent(&winapiTracking);
    }

    // Send the entered message to all elements which are newly hovered.
    while(elementUnderMouse && elementUnderMouse != commonParent){
      elementUnderMouse->mouseInside = NA_TRUE;
      na_DispatchUIElementCommand(elementUnderMouse, NA_UI_COMMAND_MOUSE_ENTERED);
      elementUnderMouse = naGetUIElementParent(elementUnderMouse);
    }
  }
}


NAWINAPICallbackInfo naUIElementWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NA_UIElement* elem = (NA_UIElement*)uiElement;
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
    rect = naGetUIElementRect(uiElement, naGetApplication(), NA_FALSE);
    size.width += rect.pos.x;
    size.height = rect.pos.y + rect.size.height - size.height;
    mouseStatus = naGetMouseStatus();
    pos = naGetMousePos(mouseStatus);
    na_SetMouseMovedByDiff(size.width - pos.x, size.height - pos.y);

    handeled = na_DispatchUIElementCommand(elem, NA_UI_COMMAND_MOUSE_MOVED);
    if(handeled){
      info.hasbeenhandeled = NA_TRUE;
      info.result = 0;
    }
    break;

  case WM_MOUSELEAVE:
    naWINAPICaptureMouseHover();
    break;

  case WM_KEYDOWN:
    handeled = na_DispatchUIElementCommand(elem, NA_UI_COMMAND_KEYDOWN);
    if(handeled){
      info.hasbeenhandeled = NA_TRUE;
      info.result = 0;
    }
    break;

  case WM_KEYUP:
    handeled = na_DispatchUIElementCommand(elem, NA_UI_COMMAND_KEYUP);
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
  NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(controlWnd);

  if(uiElement && na_AreUIElementNotificationsAllowed(uiElement)){
    switch(naGetUIElementType(uiElement)){
    case NA_UI_BUTTON:    info = naButtonWINAPINotify   (uiElement, notificationCode); break;
    case NA_UI_CHECKBOX:  info = naCheckBoxWINAPINotify (uiElement, notificationCode); break;
    case NA_UI_LABEL:     info = naLabelWINAPINotify    (uiElement, notificationCode); break;
    case NA_UI_TEXTFIELD: info = naTextFieldWINAPINotify(uiElement, notificationCode); break;
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
  NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(drawitemstruct->hwndItem);

  if(uiElement){
    switch(naGetUIElementType(uiElement)){
    case NA_UI_BUTTON:     info = naButtonWINAPIDrawItem    (uiElement, drawitemstruct); break;
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

NA_HDEF void na_RefreshUIElementNow(void* uiElement){
  RedrawWindow(naGetUIElementNativeId(uiElement), NA_NULL, NA_NULL, RDW_INVALIDATE | RDW_ERASE);
}



NA_HDEF void** na_GetUIElementNextTabReference(void* uiElement){
  switch(naGetUIElementType(uiElement)){
  case NA_UI_TEXTFIELD: return na_GetTextFieldNextTabReference(uiElement); break;
  case NA_UI_TEXTBOX:   return na_GetTextBoxNextTabReference(uiElement); break;
  default:
    #ifndef NDEBUG
      naError("Invalid type");
    #endif
    return NA_NULL;
  }
}



NA_HDEF void** na_GetUIElementPrevTabReference(void* uiElement){
  switch(naGetUIElementType(uiElement)){
  case NA_UI_TEXTFIELD: return na_GetTextFieldPrevTabReference(uiElement); break;
  case NA_UI_TEXTBOX:   return na_GetTextBoxPrevTabReference(uiElement); break;
  default:
    #ifndef NDEBUG
      naError("Invalid type");
    #endif
    return NA_NULL;
  }
}



NA_DEF void naSetUIElementNextTabElement(void* elem, void* nextTabElem){
  void** elemNextRef;
  void** nextPrevRef;
  void** elemNextPrevRef;
  void** nextPrevNextRef;

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

  elemNextRef = na_GetUIElementNextTabReference(elem);
  nextPrevRef = na_GetUIElementPrevTabReference(nextTabElem);
  elemNextPrevRef = na_GetUIElementPrevTabReference(*elemNextRef);
  nextPrevNextRef = na_GetUIElementNextTabReference(*nextPrevRef);

  *nextPrevNextRef = *elemNextRef;
  *elemNextPrevRef = *nextPrevRef;
  *nextPrevRef = elem;
  *elemNextRef = nextTabElem;

  // This is how it is supposed to work on windows, but it doesn't.
  //SetWindowPos(((NA_UIElement*)nextTabElem)->nativeId, ((NA_UIElement*)elem)->nativeId, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
}




NA_HDEF NARect na_GetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect na_GetScreenAbsoluteRect(NA_UIElement* screen){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_DEF NARect naGetUIElementRect(void* uiElement, void* relativeelement, NABool includebounds){
  NARect rect;
  NARect relrect;
  NA_UIElement* elem;
  NA_UIElement* relElem;
  NAApplication* app;

  elem = (NA_UIElement*)uiElement;
  relElem = (NA_UIElement*)relativeelement;
  app = naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(elem == (NA_UIElement*)app){
    #ifndef NDEBUG
      if(relativeelement && (relativeelement != app))
        naError("The relative element is invalid for the given uiElement, which seems to be the application.");
    #endif
    return na_GetApplicationAbsoluteRect();
  }

  // Now, we find the appropriate relative element.
  if(!relElem){relElem = naGetUIElementParent(elem);}

  switch(elem->elementType){
  case NA_UI_APPLICATION: rect = na_GetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = na_GetScreenAbsoluteRect(elem); break;
  case NA_UI_WINDOW:
    if(includebounds){
      rect = na_GetWindowAbsoluteOuterRect(elem);
    }else{
      rect = na_GetWindowAbsoluteInnerRect(elem);
    }
    break;
  case NA_UI_OPENGLSPACE:  rect = na_GetSpaceAbsoluteInnerRect(elem); break;
  default:
    //#ifndef NDEBUG
    //  naError("Invalid UI type");
    //#endif
    rect = naMakeRectSE(0., 0., 0., 0.);
    break;
  }

  if(relElem){
    switch(relElem->elementType){
    case NA_UI_APPLICATION: relrect = na_GetApplicationAbsoluteRect(); break;
    case NA_UI_SCREEN:      relrect = na_GetScreenAbsoluteRect(relElem); break;
    case NA_UI_WINDOW:      relrect = na_GetWindowAbsoluteInnerRect(relElem); break;
    case NA_UI_OPENGLSPACE:  relrect = na_GetSpaceAbsoluteInnerRect(relElem); break;
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



NA_HDEF void* na_AllocMouseTracking(NANativeId nativeId){
  NABool success;
  TRACKMOUSEEVENT* winapiTracking = naAlloc(TRACKMOUSEEVENT);
  winapiTracking->cbSize = sizeof(TRACKMOUSEEVENT);
  winapiTracking->dwFlags = TME_LEAVE;
  winapiTracking->hwndTrack = nativeId;
  winapiTracking->dwHoverTime = HOVER_DEFAULT;
  success = TrackMouseEvent(winapiTracking);
  return winapiTracking;
}



NA_HDEF void na_DeallocMouseTracking(void* tracking){
  TRACKMOUSEEVENT* winapiTracking = (TRACKMOUSEEVENT*)tracking;
  winapiTracking->dwFlags |= TME_CANCEL;
  TrackMouseEvent(winapiTracking);
  naFree(winapiTracking);
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
  long winapiAlignment;
  switch(alignment){
  case NA_TEXT_ALIGNMENT_LEFT: winapiAlignment = ES_LEFT; break;
  case NA_TEXT_ALIGNMENT_RIGHT: winapiAlignment = ES_RIGHT; break;
  case NA_TEXT_ALIGNMENT_CENTER: winapiAlignment = ES_CENTER; break;
  default:
    #ifndef NDEBUG
      naError("Invalid alignment enumeration");
    #endif
    winapiAlignment = ES_LEFT; break;
    break;
  }
  return winapiAlignment;
}



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
