
// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#include "../Core/NAAppCore.h"
#include "../../NAUtility/NAValueHelper.h"


// UI_Element flags:
// Note that flags will always be initialized with 0
#define NA_UI_ELEMENT_FLAG_MOUSE_INSIDE                    0x01
#define NA_UI_ELEMENT_FLAG_BLOCK_WINAPI_NOTIFICATIONS      0x02



NA_HAPI void** na_GetUIElementNextTabReference(void* textField);
NA_HAPI void** na_GetUIElementPrevTabReference(void* textField);
NA_HAPI void** na_GetTextBoxNextTabReference(NATextBox* textBox);
NA_HAPI void** na_GetTextBoxPrevTabReference(NATextBox* textBox);
NA_HAPI void** na_GetTextFieldNextTabReference(NATextField* textField);
NA_HAPI void** na_GetTextFieldPrevTabReference(NATextField* textField);

HWND naGetApplicationOffscreenWindow(void);
NA_UIElement* naGetApplicationMouseHoverElement(void);
void naSetApplicationMouseHoverElement(NA_UIElement* element);
const NONCLIENTMETRICS* naGetApplicationMetrics(void);

NA_HAPI UINT na_GetApplicationNextMenuItemId(NAApplication* application);
NA_HAPI void na_SetApplicationLastOpenedMenu(NAApplication* application, const NAMenu* menu);
NA_HAPI const NAMenu* na_GetApplicationLastOpenedMenu(NAApplication* application);
NA_HAPI void na_SetMenuItemId(NAMenuItem* menuItem, uint32 id, NAMenu* menu);
NA_HAPI uint32 na_GetMenuItemId(const NAMenuItem* menuItem);
NA_HAPI void na_updateMenuItem(NAMenu* menu, const NAMenuItem* menuItem);

NA_HAPI NAUIColor* na_SwapWINAPISpaceBackgroundColor(NASpace* space, NAUIColor* bgColor);
NA_HAPI void na_ForceWINAPISpaceToRedrawBackground(NA_UIElement* space);


NA_HDEF void na_ClearUINativePtr(void* nativePtr) {
    DestroyWindow(nativePtr);
}

NA_HDEF void na_SetUIElementParent(NA_UIElement* uiElement, void* parent, NABool isElementAttachable) {
  NA_UIElement* elem;
  NA_UIElement* parentElem;
  NAWindow* window;

  #if NA_DEBUG
    if(!uiElement)
      naCrash("uiElement is Null");
  #endif
  elem = (NA_UIElement*)uiElement;
  parentElem = (NA_UIElement*)parent;

  window = naGetUIElementWindow(uiElement);
  if(window && naGetWindowFirstTabElement(window) == elem) {
    naSetWindowFirstTabElement(window, NA_NULL);
  }

  if(!parentElem) {
    HWND result;
    elem->parent = NA_NULL;
    result = SetParent(elem->nativePtr, HWND_MESSAGE);
  }else{
    HWND result;

    elem->parent = parent;
    if(isElementAttachable)
    {
      result = SetParent(elem->nativePtr, parentElem->nativePtr);
    }
  }
}



NA_HDEF double na_GetUIElementYOffset(const NA_UIElement* elem) {
  // Line height is considered to be 25 for an optimal display. In this
  // function, the UI elements are shifted in Y direction such that text
  // always is displayed on a common baseline with 1x resolution. Higher
  // resolutions might show different alignments out of our control.
  // The reference element is a stateful text button.
  // All spaces and stateful/image buttons have offset 0.

  switch(naGetUIElementType(elem)) {
  case NA_UI_APPLICATION:  return  0.0;
  case NA_UI_BUTTON:       return  0.0;
  case NA_UI_CHECKBOX:     return  2.5;
  case NA_UI_IMAGE_SPACE:  return  0.0;
  case NA_UI_LABEL:        return  4.0;
  case NA_UI_MENU:         return  0.0;
  case NA_UI_MENUITEM:     return  0.0;
  case NA_UI_METAL_SPACE:  return  0.0;
  case NA_UI_OPENGL_SPACE: return  0.0;
  case NA_UI_RADIO:        return  2.5;
  case NA_UI_SCREEN:       return  0.0;
  case NA_UI_SELECT:       return -0.5;
  case NA_UI_SLIDER:       return  0.0;
  case NA_UI_SPACE:        return  0.0;
  case NA_UI_TEXTBOX:      return  2.0;
  case NA_UI_TEXTFIELD:    return  2.0;
  case NA_UI_WINDOW:       return  0.0;
  default: return 0.;
  }
}



NA_HDEF NA_UIElement* na_GetUIElementCommonParent(NA_UIElement* elem1, NA_UIElement* elem2) {
  NA_UIElement* commonParent = NA_NULL;
  NA_UIElement* tmpelem2;
  while(elem1) {
    tmpelem2 = elem2;
    while(tmpelem2) {
      if(elem1 == tmpelem2) {
        commonParent = elem1;
        break;
      }
      tmpelem2 = naGetUIElementParent(tmpelem2);
    }
    
    if(commonParent)
      break;
      
    elem1 = naGetUIElementParent(elem1);
  }
  return commonParent;
}



NA_HDEF NABool na_GetUIElementMouseInside(const NA_UIElement* elem) {
  return naGetFlagu32(elem->flags, NA_UI_ELEMENT_FLAG_MOUSE_INSIDE);
}

NA_HDEF void na_SetUIElementMouseInside(NA_UIElement* elem, NABool inside) {
  #if NA_DEBUG
  if(naGetFlagu32(elem->flags, NA_UI_ELEMENT_FLAG_MOUSE_INSIDE) == inside)
    naError("mouse inside flag already set");
  #endif
  naGetFlagu32(elem->flags, NA_UI_ELEMENT_FLAG_MOUSE_INSIDE);
}



NA_HDEF NABool na_GetUIElementWINAPINotificationsBlocked(const NA_UIElement* elem) {
  return naGetFlagu32(elem->flags, NA_UI_ELEMENT_FLAG_BLOCK_WINAPI_NOTIFICATIONS);
}

NA_HDEF void na_SetUIElementWINAPINotificationsBlocked(NA_UIElement* elem, NABool block) {
  #if NA_DEBUG
  if(naGetFlagu32(elem->flags, NA_UI_ELEMENT_FLAG_BLOCK_WINAPI_NOTIFICATIONS) == block)
    naError("WINAPI notifications block already set");
  #endif
  naSetFlagu32(&elem->flags, NA_UI_ELEMENT_FLAG_BLOCK_WINAPI_NOTIFICATIONS, block);
}



NA_DEF void naPresentAlertBox(NAAlertBoxType alertBoxType, const NAUTF8Char* titleText, const NAUTF8Char* infoText) {
  NAString* messageString;
  TCHAR* systemTitle;
  TCHAR* systemMessage;
  UINT iconType = MB_ICONINFORMATION;

  switch(alertBoxType) {
  case NA_ALERT_BOX_INFO:    iconType = MB_ICONINFORMATION; break;
  case NA_ALERT_BOX_WARNING: iconType = MB_ICONWARNING; break;
  case NA_ALERT_BOX_ERROR:   iconType = MB_ICONERROR; break;
  }

  messageString = naNewStringWithFormat("%s" NA_NL "%s", titleText, infoText);
  systemTitle = naAllocSystemStringWithUTF8String(titleText);
  systemMessage = naAllocSystemStringWithUTF8String(naGetStringUTF8Pointer(messageString));

  MessageBox(NA_NULL, systemMessage, systemTitle, MB_OK | iconType);

  naFree(systemMessage);
  naFree(systemTitle);
  naDelete(messageString);
}


NA_HDEF NABool naPresentFilePanel(
  NABool load,
  const NAUTF8Char* fileName,
  const NAUTF8Char* allowedFileSuffix,
  NAFilePanelCallback callback,
  const void* data)
{
  // todo
  return NA_FALSE;
}



NA_HDEF void na_CaptureKeyboardStatus(MSG* message) {  
  NABool lShift;
  NABool rShift;
  NABool lControl;
  NABool rControl;
  NABool lOption;
  NABool rOption;

  NABool isExtendedKey = (message->lParam >> 24) & 0x01;  // Extended keys usually are the ones on the right.

  NAKeyCode keyCode = (NAKeyCode)MapVirtualKey((UINT)message->wParam, MAPVK_VK_TO_VSC);

  lShift = (GetKeyState(VK_LSHIFT) & 0x8000) >> 15;
  rShift = (GetKeyState(VK_RSHIFT) & 0x8000) >> 15;
  // Note: Due to the right shift key not properly being detected by the extended key flag
  // of lParam, we have to rely on GetKeyState. Pity.
  // of lParam, we have to rely on GetKeyState. Pity.
  
  NABool hasShift = lShift || rShift;
  lControl = (GetKeyState(VK_LCONTROL) & 0x8000) >> 15;
  rControl = (GetKeyState(VK_RCONTROL) & 0x8000) >> 15;
  
  NABool hasControl = lControl || rControl;
  lOption = (GetKeyState(VK_LMENU) & 0x8000) >> 15;
  rOption = (GetKeyState(VK_RMENU) & 0x8000) >> 15;
  
  // Note: AltGr actually sends first an rControl and then an rOption. Don't know why.
  NABool hasOption = lOption || rOption;
  
  NABool hasCommand = keyCode == NA_KEYCODE_LEFT_COMMAND || keyCode == NA_KEYCODE_RIGHT_COMMAND;

  uint32 modifierFlags =
    (uint32)hasShift * NA_KEY_MODIFIER_SHIFT |
    (uint32)hasControl * NA_KEY_MODIFIER_CONTROL |
    (uint32)hasOption * NA_KEY_MODIFIER_OPTION |
    (uint32)hasCommand * NA_KEY_MODIFIER_COMMAND;

  // Note, this implementation is far from finished. It does strange things, but that
  // just seems to be a thing with windows key mappings. :(

  NAKeyStroke* keyStroke = naNewKeyStroke(keyCode, modifierFlags);
  NAApplication* app = naGetApplication();
  na_SetApplicationKeyStroke(app, keyStroke);
}



// For certain elements like TextField, WS_TABSTOP and WS_GROUP seem not to
// work... strange. I solved it using this function. 
NA_HDEF NABool na_InterceptKeyboardShortcut(MSG* message) {
  NABool retValue = NA_FALSE;

  if(message->message == WM_KEYUP || message->message == WM_SYSKEYDOWN || message->message == WM_SYSKEYUP) {
    NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(message->hwnd);
    na_CaptureKeyboardStatus(message);
    if(!naGetDefaultWindowSystemKeyHandling(uiElement)) {
      // We mark the key stroke to be handeled such that Windows will not
      // do any funny business.
      retValue = NA_TRUE;
    }

  }else if(message->message == WM_KEYDOWN) {
    NA_UIElement* elem;
    HWND keyWindow;
    na_CaptureKeyboardStatus(message);
    
    // Search for the native first responder which is represented in NALib.
    elem = NA_NULL;
    keyWindow = GetFocus();
    while(!elem && keyWindow) {
      elem = na_GetUINALibEquivalent(keyWindow);
      keyWindow = GetParent(keyWindow);
    }
    if(!elem) {
      elem = &naGetApplication()->uiElement;
    }

    const NAKeyStroke* keyStroke = naGetCurrentKeyStroke();

    // Search for a matching keyboard shortcut by bubbling.
    while(!retValue && elem) {
      NAListIterator iter = naMakeListAccessor(&elem->shortcuts);

      while(!retValue && naIterateList(&iter)) {
        const NA_KeyboardShortcutReaction* keyReaction = naGetListCurConst(&iter);
        const NAKeyStroke* shortcut = keyReaction->shortcut;
        NABool needsShift   = naGetKeyStrokeModifierPressed(shortcut, NA_KEY_MODIFIER_SHIFT);
        NABool needsControl = naGetKeyStrokeModifierPressed(shortcut, NA_KEY_MODIFIER_CONTROL);
        NABool needsOption  = naGetKeyStrokeModifierPressed(shortcut, NA_KEY_MODIFIER_OPTION);
        NABool needsCommand = naGetKeyStrokeModifierPressed(shortcut, NA_KEY_MODIFIER_COMMAND);
        NABool hasShift   = naGetKeyStrokeModifierPressed(keyStroke, NA_KEY_MODIFIER_SHIFT);
        NABool hasControl = naGetKeyStrokeModifierPressed(keyStroke, NA_KEY_MODIFIER_CONTROL);
        NABool hasOption  = naGetKeyStrokeModifierPressed(keyStroke, NA_KEY_MODIFIER_OPTION);
        NABool hasCommand = naGetKeyStrokeModifierPressed(keyStroke, NA_KEY_MODIFIER_COMMAND);

        if(naGetKeyStrokeKeyCode(shortcut) == naGetKeyStrokeKeyCode(keyStroke) &&
          needsShift   == hasShift &&
          needsControl == hasControl &&
          needsOption  == hasOption &&
          needsCommand == hasCommand)
        {
          NAReaction reaction = {
            elem,
            NA_UI_COMMAND_KEYBOARD_SHORTCUT,
            keyReaction->controller};

          keyReaction->callback(reaction);
          retValue = NA_TRUE;
        }
      }
      naClearListIterator(&iter);
      elem = naGetUIElementParent(elem);
    }
  }

  return retValue;
}



typedef struct NAWINAPICallbackInfo NAWINAPICallbackInfo;
struct NAWINAPICallbackInfo{
  LRESULT result;
  NABool hasBeenHandeled;
};



// ///////////////////////////////////
// WINDOW CALLBACK
// ///////////////////////////////////

// Get overridden WindowProcs of specialized controls.
WNDPROC na_GetApplicationOldButtonWindowProc();
WNDPROC na_GetApplicationOldCheckBoxWindowProc();
WNDPROC na_GetApplicationOldLabelWindowProc();
WNDPROC na_GetApplicationOldRadioWindowProc();
WNDPROC na_GetApplicationOldSelectWindowProc();
WNDPROC na_GetApplicationOldSliderWindowProc();
WNDPROC na_GetApplicationOldTextFieldWindowProc();

// Prototypes of specialized WindowProc event procedures
NAWINAPICallbackInfo naWINAPIDrawItemProc    (WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWINAPINotificationProc(WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWINAPIScrollItemProc  (WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naUIElementWINAPIPreProc  (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naButtonWINAPIDrawItem (void* uiElement, DRAWITEMSTRUCT* drawitemstruct);
NAWINAPICallbackInfo naSliderWINAPIScroll   (void* uiElement, WPARAM wParam);

// Prototypes of the WindowProc procedures of the specific controls
NAWINAPICallbackInfo naApplicationWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naButtonWINAPIProc     (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naCheckBoxWINAPIProc   (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naImageSpaceWINAPIProc (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naLabelWINAPIProc      (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naRadioWINAPIProc      (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naSelectWINAPIProc     (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naSliderWINAPIProc     (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naSpaceWINAPIProc      (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naTextBoxWINAPIProc    (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naTextFieldWINAPIProc  (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);
NAWINAPICallbackInfo naWindowWINAPIProc     (void* uiElement, UINT message, WPARAM wParam, LPARAM lParam);

// Prototypes of WindowProc procedures which react to notifications
NABool naButtonWINAPINotify      (void* uiElement, WORD notificationCode);
NABool naCheckBoxWINAPINotify    (void* uiElement, WORD notificationCode);
NABool naLabelWINAPINotify       (void* uiElement, WORD notificationCode);
NABool naSelectWINAPINotify      (void* uiElement, WORD notificationCode);
NABool naTextFieldWINAPINotify   (void* uiElement, WORD notificationCode);

NA_HAPI HICON na_GetWINAPIApplicationIcon(void);


// Returns true if the element under the mouse must handle hovering events.
NABool naWINAPICaptureMouseHover() {
  DWORD msgpos = GetMessagePos();
  POINT pt = {GET_X_LPARAM(msgpos), GET_Y_LPARAM(msgpos)};
  HWND hWndUnderMouse = WindowFromPoint(pt);
  NA_UIElement* elementUnderMouse = (NA_UIElement*)na_GetUINALibEquivalent(hWndUnderMouse);

  if(elementUnderMouse && elementUnderMouse->hoverReactionCount == 0)
    return NA_FALSE;

  NA_UIElement* curElement = naGetApplicationMouseHoverElement();

  if(curElement != elementUnderMouse) {
    NA_UIElement* commonParent = na_GetUIElementCommonParent(curElement, elementUnderMouse);

    // Send a leave reaction to all elements which are not hovered anymore.
    while(curElement && curElement != commonParent) {
      na_SetUIElementMouseInside(curElement, NA_FALSE);

      if(naGetUIElementType(curElement) == NA_UI_BUTTON) {
        na_RefreshUIElementNow(curElement);
      }

      if(!na_DispatchUIElementCommand(curElement, NA_UI_COMMAND_MOUSE_EXITED)) {
        // don't know what to do.
      }
      curElement = naGetUIElementParent(curElement);
    }

    // Reset the hover element to the current one and track the mouse leaving it.
    naSetApplicationMouseHoverElement(elementUnderMouse);
    if(elementUnderMouse) {
      TRACKMOUSEEVENT winapiTracking = {
        sizeof(TRACKMOUSEEVENT),
        TME_LEAVE,
        naGetUIElementNativePtr(elementUnderMouse),
        HOVER_DEFAULT};
      TrackMouseEvent(&winapiTracking);
    }

    // Send the entered message to all elements which are newly hovered.
    while(elementUnderMouse && elementUnderMouse != commonParent) {
      na_SetUIElementMouseInside(elementUnderMouse, NA_TRUE);

      if(naGetUIElementType(elementUnderMouse) == NA_UI_BUTTON) {
        na_RefreshUIElementNow(curElement);
      }

      if(!na_DispatchUIElementCommand(elementUnderMouse, NA_UI_COMMAND_MOUSE_ENTERED)) {
        // don't know what to do.
      }
      elementUnderMouse = naGetUIElementParent(elementUnderMouse);
    }
  }
  return NA_TRUE;
}



NAWINAPICallbackInfo na_HandleMousePress(
  NA_UIElement* elem,
  NAMouseButton button,
  NABool press)
{
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NAUIElementType type = naGetUIElementType(elem);
  if(type == NA_UI_APPLICATION
    || type == NA_UI_IMAGE_SPACE
    || type == NA_UI_METAL_SPACE
    || type == NA_UI_OPENGL_SPACE
    || type == NA_UI_SCREEN
    || type == NA_UI_SPACE)
  {
    NAApplication* app = naGetApplication();
    na_SetMouseButtonPressed(na_GetApplicationMouseStatus(app), button, press);
    NAUICommand command = press
      ? NA_UI_COMMAND_MOUSE_DOWN
      : NA_UI_COMMAND_MOUSE_UP;
    if(!na_DispatchUIElementCommand(elem, command)) {
      // don't know what to do.
    }
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
  }

  return info;
}


// Capture messages which shall be handeled globally the same no matter what
// the object behind it is.
NAWINAPICallbackInfo naUIElementWINAPIPreProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  NA_UIElement* elem = (NA_UIElement*)uiElement;
  NAPos pos;
  NASize size = {0};
  NARect rect = {0};
  const NAMouseStatus* mouseStatus;

  switch(message) {

  case WM_LBUTTONDOWN:
    info = na_HandleMousePress(elem, NA_MOUSE_BUTTON_LEFT, NA_TRUE);
    break;

  case WM_RBUTTONDOWN:
    info = na_HandleMousePress(elem, NA_MOUSE_BUTTON_RIGHT, NA_TRUE);
    break;

  case WM_MBUTTONDOWN:
    info = na_HandleMousePress(elem, NA_MOUSE_BUTTON_MIDDLE, NA_TRUE);
    break;

  case WM_LBUTTONUP:
    info = na_HandleMousePress(elem, NA_MOUSE_BUTTON_LEFT, NA_FALSE);
    break;

  case WM_RBUTTONUP:
    info = na_HandleMousePress(elem, NA_MOUSE_BUTTON_RIGHT, NA_FALSE);
    break;

  case WM_MBUTTONUP:
    info = na_HandleMousePress(elem, NA_MOUSE_BUTTON_MIDDLE, NA_FALSE);
    break;

  case WM_MOUSEHOVER: // being inside the hWND for a specified amout of time.
    break;

  case WM_MOUSEMOVE:
    // wParam: several special keys
    // GET_X_LPARAM(lParam): x coord relative to top left
    // GET_Y_LPARAM(lParam): y coord relative to top left
    if(naWINAPICaptureMouseHover()) {
      double uiScale = naGetUIElementResolutionScale(elem);
      size.width = GET_X_LPARAM(lParam) / uiScale;
      size.height = GET_Y_LPARAM(lParam) / uiScale;
      rect = naGetUIElementRectAbsolute(uiElement);
      size.width += rect.pos.x;
      size.height = rect.pos.y + rect.size.height - size.height;
      mouseStatus = naGetCurrentMouseStatus();
      pos = naGetMousePos(mouseStatus);
      na_SetMouseMovedByDiff(na_GetApplicationMouseStatus(naGetApplication()), size.width - pos.x, size.height - pos.y);

      if(!na_DispatchUIElementCommand(elem, NA_UI_COMMAND_MOUSE_MOVED)) {
        // don't know what to do.
      }
      info.result = 0;
      info.hasBeenHandeled = NA_TRUE;
    }
    break;

  case WM_MOUSELEAVE:
    naWINAPICaptureMouseHover();
    break;

  case WM_KEYDOWN:
    if(na_UIHasElementCommandDispatches(elem, NA_UI_COMMAND_KEY_DOWN)) {
      if(!na_DispatchUIElementCommand(elem, NA_UI_COMMAND_KEY_DOWN)) {
        // don't know what to do.
      }
      info.result = 0;
      info.hasBeenHandeled = NA_TRUE;
    }
    break;

  case WM_KEYUP:
    if(na_UIHasElementCommandDispatches(elem, NA_UI_COMMAND_KEY_UP)) {
      if(!na_DispatchUIElementCommand(elem, NA_UI_COMMAND_KEY_UP)) {
        // don't know what to do.
      }
      info.result = 0;
      info.hasBeenHandeled = NA_TRUE;
    }
    break;
  }

  return info;
}



// Capture messages which shall be handeled globally, if and only if the
// object behind the message does NOT provide a WINAPIProc handling.
NAWINAPICallbackInfo naUIElementWINAPIPostProc(void* uiElement, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message) {
  case WM_SETFOCUS:
  case WM_KILLFOCUS:
    // We do not display any caret.
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
    break;

  case WM_WINDOWPOSCHANGED:
    // Always handle this message otherwise it will be given to the parents
    // until someone implements it. But then, the coords are wrong.
    info.result = 0;
    info.hasBeenHandeled = NA_TRUE;
    break;

  default:
    // do nothing.
    break;
  }

  return info;
}



// This method calls the default procedure given by WINAPI.
NAWINAPICallbackInfo naUIElementWINAPIDefaultProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(hWnd);
  NAUIElementType uiType = uiElement ? naGetUIElementType(uiElement) : NA_UI_APPLICATION;

  switch(uiType) {
  case NA_UI_BUTTON:
    info.result = CallWindowProc(na_GetApplicationOldButtonWindowProc(), hWnd, message, wParam, lParam);
    break;
  case NA_UI_CHECKBOX:
    info.result = CallWindowProc(na_GetApplicationOldCheckBoxWindowProc(), hWnd, message, wParam, lParam);
    break;
  case NA_UI_LABEL:
    info.result = CallWindowProc(na_GetApplicationOldLabelWindowProc(), hWnd, message, wParam, lParam);
    break;
  case NA_UI_RADIO:
    info.result = CallWindowProc(na_GetApplicationOldRadioWindowProc(), hWnd, message, wParam, lParam);
    break;
  case NA_UI_SELECT:
    info.result = CallWindowProc(na_GetApplicationOldSelectWindowProc(), hWnd, message, wParam, lParam);
    break;
  case NA_UI_SLIDER:
    info.result = CallWindowProc(na_GetApplicationOldSliderWindowProc(), hWnd, message, wParam, lParam);
    break;
  case NA_UI_TEXTFIELD:
    info.result = CallWindowProc(na_GetApplicationOldTextFieldWindowProc(), hWnd, message, wParam, lParam);
    break;
  default:
    info.result = DefWindowProc(hWnd, message, wParam, lParam);
    break;
  }

  return info;
}



// This is the one and only, master of destruction, defender of chaos and
// pimp of the century function handling all and everything in WINAPI.

LRESULT CALLBACK naWINAPIWindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(hWnd);

  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  // Capture specific messages
  if(message == WM_DRAWITEM) {
    info = naWINAPIDrawItemProc(wParam, lParam);
  }else if(message == WM_COMMAND) {
    info = naWINAPINotificationProc(wParam, lParam);
  }else if(message == WM_HSCROLL) {
    info = naWINAPIScrollItemProc(wParam, lParam);
  }else if(message == WM_NOTIFY) {
  }



  // Capture messages handeled by the actual UIElements or its parents.
  if(uiElement) {

    // First, capture mouse and keyboard events if necessary.
    info = naUIElementWINAPIPreProc(uiElement, message, wParam, lParam);
    
    if(!info.hasBeenHandeled) {
      // Go to the specialized window proc.
      switch(naGetUIElementType(uiElement)) {
      case NA_UI_APPLICATION:
        info = naApplicationWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_CHECKBOX:
        info = naCheckBoxWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_BUTTON:
        info = naButtonWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_IMAGE_SPACE:
        info = naImageSpaceWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_LABEL:
        info = naLabelWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_OPENGL_SPACE:
        info = naOpenGLSpaceWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_RADIO:
        info = naRadioWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_SLIDER:
        info = naSliderWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_SELECT:
        info = naSelectWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_SPACE:
        info = naSpaceWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_TEXTBOX:
        info = naTextBoxWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_TEXTFIELD:
        info = naTextFieldWINAPIProc(uiElement, message, wParam, lParam);
        break;
      case NA_UI_WINDOW:
        info = naWindowWINAPIProc(uiElement, message, wParam, lParam);
        break;
      default:
        break;
    }
    }

    if(!info.hasBeenHandeled) {
      // If the element does not handle the event, try handling it with the
      // global post handling function.
      info = naUIElementWINAPIPostProc(uiElement, message, wParam, lParam);
    }
  }

  // If the event has not been handeled, hand it over to the default procedure.
  if(!info.hasBeenHandeled) {
    info = naUIElementWINAPIDefaultProc(hWnd, message, wParam, lParam);
  }

  return info.result;
}



NAWINAPICallbackInfo naWINAPIDrawItemProc(WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  DRAWITEMSTRUCT* drawitemstruct = (DRAWITEMSTRUCT*)lParam;
  NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(drawitemstruct->hwndItem);

  if(uiElement) {
    switch(naGetUIElementType(uiElement)) {
    case NA_UI_BUTTON:     info = naButtonWINAPIDrawItem (uiElement, drawitemstruct); break;
    default:
      //printf("Uncaught draw item message" NA_NL);
      break;
    }
  }
  return info;
}

NAWINAPICallbackInfo naWINAPINotificationProc(WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  WORD notificationCode = HIWORD(wParam);
  WORD controlIdentifier = LOWORD(wParam);
  HWND controlWnd = (HWND)lParam;
  NABool hasBeenHandeled = NA_FALSE;

  if(controlWnd == 0 && notificationCode == 0)
  {
    // This is a menu message
    const NAMenu* menu = na_GetApplicationLastOpenedMenu(naGetApplication());
    const NAMenuItem* menuItem = NA_NULL;
    NAListIterator iter = naMakeListAccessor(&menu->childs);
    while(naIterateList(&iter)) {
      menuItem = naGetListCurConst(&iter);
      if(na_GetMenuItemId(menuItem) == controlIdentifier) {
        break;
      }
      menuItem = NA_NULL;
    }
    naClearListIterator(&iter);

    if(menuItem) {
      if(!na_DispatchUIElementCommand((const NA_UIElement*)menuItem, NA_UI_COMMAND_PRESSED)) {
        // don't know what to do.
      }
      hasBeenHandeled = NA_TRUE;
    }
    na_SetApplicationLastOpenedMenu(naGetApplication(), NA_NULL);

  }else{
    // This is a control message
    NA_UIElement* uiElement = (NA_UIElement*)na_GetUINALibEquivalent(controlWnd);
    if(uiElement && !na_GetUIElementWINAPINotificationsBlocked(uiElement)) {
      switch(naGetUIElementType(uiElement)) {
      case NA_UI_BUTTON:
        hasBeenHandeled = naButtonWINAPINotify(uiElement, notificationCode);
        break;
      case NA_UI_CHECKBOX:
        hasBeenHandeled = naCheckBoxWINAPINotify(uiElement, notificationCode);
        break;
      case NA_UI_LABEL:
        hasBeenHandeled = naLabelWINAPINotify(uiElement, notificationCode);
        break;
      case NA_UI_SELECT:
        hasBeenHandeled = naSelectWINAPINotify(uiElement, notificationCode);
        break;
      case NA_UI_TEXTFIELD:
        hasBeenHandeled = naTextFieldWINAPINotify(uiElement, notificationCode);
        break;
      default:
        //printf("Uncaught notification" NA_NL);
        break;
      }
    }
  }

  if(hasBeenHandeled) {
    info.result = 0; // Must be zero. See documentation.
    info.hasBeenHandeled = NA_TRUE;
  }
  return info;
}

NAWINAPICallbackInfo naWINAPIScrollItemProc(WPARAM wParam, LPARAM lParam) {
  NAWINAPICallbackInfo info = {NA_FALSE, 0};
  // lParam is the control which has the event. Sliders for example. Is a HWND.
  NA_UIElement* scrollElem = (NA_UIElement*)na_GetUINALibEquivalent((void*)lParam);
  if(naGetUIElementType(scrollElem) == NA_UI_SLIDER) {
    info = naSliderWINAPIScroll(scrollElem, wParam);
  }
  return info;
}



// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////

NA_HDEF void na_RefreshUIElementNow(void* uiElement) {
  RedrawWindow(naGetUIElementNativePtr(uiElement), NA_NULL, NA_NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}



NA_HDEF void** na_GetUIElementNextTabReference(void* uiElement) {
  switch(naGetUIElementType(uiElement)) {
  case NA_UI_TEXTFIELD: return na_GetTextFieldNextTabReference(uiElement); break;
  case NA_UI_TEXTBOX:   return na_GetTextBoxNextTabReference(uiElement); break;
  default:
    #if NA_DEBUG
      naError("Invalid type");
    #endif
    return NA_NULL;
  }
}



NA_HDEF void** na_GetUIElementPrevTabReference(void* uiElement) {
  switch(naGetUIElementType(uiElement)) {
  case NA_UI_TEXTFIELD: return na_GetTextFieldPrevTabReference(uiElement); break;
  case NA_UI_TEXTBOX:   return na_GetTextBoxPrevTabReference(uiElement); break;
  default:
    #if NA_DEBUG
      naError("Invalid type");
    #endif
    return NA_NULL;
  }
}



NA_DEF void naSetUIElementNextTabElement(void* uiElement, void* nextTabElem) {
  void** elemNextRef;
  void** nextPrevRef;
  void** elemNextPrevRef;
  void** nextPrevNextRef;

  #if NA_DEBUG
    if(naGetUIElementWindowConst(uiElement) != naGetUIElementWindowConst(nextTabElem))
      naError("element do not share the same window.");
  #endif

  if(  naGetUIElementType(uiElement) != NA_UI_TEXTFIELD
    && naGetUIElementType(uiElement) != NA_UI_TEXTBOX) {
    #if NA_DEBUG
      naError("elem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  if(  naGetUIElementType(nextTabElem) != NA_UI_TEXTFIELD
    && naGetUIElementType(nextTabElem) != NA_UI_TEXTBOX) {
    #if NA_DEBUG
      naError("nextTabElem has a type which can not be used as a next tab.");
    #endif
    return;
  }

  elemNextRef = na_GetUIElementNextTabReference(uiElement);
  nextPrevRef = na_GetUIElementPrevTabReference(nextTabElem);
  elemNextPrevRef = na_GetUIElementPrevTabReference(*elemNextRef);
  nextPrevNextRef = na_GetUIElementNextTabReference(*nextPrevRef);

  *nextPrevNextRef = *elemNextRef;
  *elemNextPrevRef = *nextPrevRef;
  *nextPrevRef = uiElement;
  *elemNextRef = nextTabElem;

  // This is how it is supposed to work on windows, but it doesn't. todo.
  //SetWindowPos(((NA_UIElement*)nextTabElem)->nativePtr, ((NA_UIElement*)elem)->nativePtr, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE); 
}



NA_DEF double naGetUIElementResolutionScale(const void* uiElement) {
  int dpi;
  HDC hDC;
  if(hDC = GetDC (NULL)) {
    dpi = GetDeviceCaps (hDC, LOGPIXELSX);
    ReleaseDC (NULL, hDC);
  }else{
    dpi = USER_DEFAULT_SCREEN_DPI;
  }
  return (double)dpi / (double)USER_DEFAULT_SCREEN_DPI;
}



NA_API NARect naGetMainScreenRect() {
  HMONITOR screen;
  MONITORINFO screeninfo = {0};
  POINT origin = {0, 0};  // top left point which surely is on the main screen.
  screen = MonitorFromPoint(origin, MONITOR_DEFAULTTOPRIMARY);
  screeninfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(screen, &screeninfo);
  double uiScale = naGetUIElementResolutionScale(NA_NULL);

  // The main monitor is by definition at (0,0) which in NALib is the bottom left corner.
  NARect rect = naMakeRectS(
    0. / uiScale,
    0. / uiScale,
    ((double)screeninfo.rcMonitor.right - (double)screeninfo.rcMonitor.left) / uiScale,
    ((double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top) / uiScale);
  return rect;
}



long getWINAPITextAlignmentWithAlignment(NATextAlignment alignment) {
  long winapiAlignment;
  switch(alignment) {
  case NA_TEXT_ALIGNMENT_LEFT: winapiAlignment = ES_LEFT; break;
  case NA_TEXT_ALIGNMENT_RIGHT: winapiAlignment = ES_RIGHT; break;
  case NA_TEXT_ALIGNMENT_CENTER: winapiAlignment = ES_CENTER; break;
  default:
    #if NA_DEBUG
      naError("Invalid alignment enumeration");
    #endif
    winapiAlignment = ES_LEFT; break;
    break;
  }
  return winapiAlignment;
}



NA_HDEF void* na_AddMouseTracking(NA_UIElement* uiElement) {
  NABool success;
  TRACKMOUSEEVENT* winapiTracking = naAlloc(TRACKMOUSEEVENT);
  winapiTracking->cbSize = sizeof(TRACKMOUSEEVENT);
  winapiTracking->dwFlags = TME_LEAVE;
  winapiTracking->hwndTrack = naGetUIElementNativePtr(uiElement);
  winapiTracking->dwHoverTime = HOVER_DEFAULT;
  success = TrackMouseEvent(winapiTracking);
  return winapiTracking;
}



NA_HDEF void na_ClearMouseTracking(NA_UIElement* uiElement, void* mouseTracking) {
  TRACKMOUSEEVENT* winapiTracking = (TRACKMOUSEEVENT*)mouseTracking;
  winapiTracking->dwFlags |= TME_CANCEL;
  TrackMouseEvent(winapiTracking);
  naFree(winapiTracking);
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
