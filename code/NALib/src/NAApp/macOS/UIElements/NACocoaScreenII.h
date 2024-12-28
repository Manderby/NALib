
NA_DEF NARect naGetMainScreenRect() {
  return naMakeRectWithNSRect([[NSScreen mainScreen] frame]);
}
