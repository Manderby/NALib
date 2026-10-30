
#include "../NAMath/NACoord.h"

NA_PROTOTYPE(NASpace);


// The layouter of NALib allows you to place elements in an ordered fashion
// inside of an NASpace.
//
// You start a layout by selecting a space. That space has itself defined a
// desired layout direction. A layout direction always consists of a primary
// and secondary direction. For example, typical western language reads
// primarily from left-to-right (LR) and secondarily from top-to-bottom (TB).
//
// Then, you place uiElements by defining sections which themselves contain
// elements. Depending on your layout directions, sections and elements
// alternate between being ordered vertically and horizontally.
//
// The following could be an example of a layout:
//
// naSetSpaceLayoutDirections(space, LRTB)
// naBeginLayout(space)
//   naAddLayoutSection(section 1)
//     naAddLayoutElement(element 1)
//     naAddLayoutElement(element 2)
//     naAddLayoutElement(element 3)
//   naAddLayoutSection(section 2)
//     naAddLayoutElement(element 4)
//     naAddLayoutElement(element 5)
//   naAddLayoutSection(section 3)
//     naAddLayoutElement(element 6)
//     naAddLayoutElement(element 7)
//     naAddLayoutElement(element 8)
//     naAddLayoutElement(element 9)
// naEndLayout()
//
//   ------------------------------------------------->
// +-----------+----------------------+-----------+-----+
// | element 1 |      element 2       | element 3 |     |    section 1   |
// +-----------+--+-----------+-------+-----------+-----+                |
// |  element 4   | element 5 |                         |    section 2   |
// +-----------+--+-----------+-+-----------+-----------+                |
// | element 6 |   element 7    | element 8 | element 9 |    section 3   V
// +-----------+----------------+-----------+-----------+
//
// The space containing all these sections and elements will automatically
// resize appropriately. If that space happens to be the contentSpace of a
// window, the window also resizes automatically.
//
// This should get you the basic idea behind the layouter. Everything after
// this explanation is more sophisticated. Only read further if you need to
// use more elaborated layouts.
//
// ---------------------
//
// This is the full picture of what is actually going on inside the layouter:
//
// +------------------------------- layout --------------------------------+
// |                                                                       |
// |         +------------ block ----------+- block -+- block -+-------+   |
// |         |                             |         |         |       |   |
// |         |   +------- content -----+   |         |         |       |   |
// |         |   |                     |   |         |         |       |   |
// |         |   |   +- uiElement -+   |   |         |         | flex  |   |
// | section |   |   |             |   |   |         |         | space |   |
// |         |   |   +-------------+   |   |         |         |       |   |
// |         |   |                     |   |         |         |       |   |
// |         |   +---------------------+   |         |         |       |   |
// |         |                             |         |         |       |   |
// |         +-----------------------------+---------+---------+-------+   |
// | section |                                                         |   |
// |         +---------------------------------------------------------+   |
// | section |                                                         |   |
// |         +---------------------------------------------------------+   |
// |                                                                       |
// +-----------------------------------------------------------------------+
//
// layout:
// The topmost element is the layout which must be defined by an NASpace.
// That space defines the layout directions. Here in this example, one assumes
// that the primary direction is from left to right and the secondary direction
// is from top to bottom.
//
// section:
// Each layout has multiple sections which are added sequentially with a call
// to naAddLayoutSection, here from top to bottom. Each section in one layout
// has the SAME width which is defined by how much space the elements contained
// within need at least. The height of each section is depicted by blocksize2,
// in this case, the height. You define blocksize2 when adding a section.
//
// block:
// Each section has multiple blocks which are added sequentially with a call
// to naAddLayoutElement, here from left to right. Each block in one section
// has the SAME height as defined by the height of the section. You define a
// margin to the previous element as well as blocksize1, in this case the
// width of the block. You can also create a sublayout with naBeginSubLayout
// which behaves just the same as an arbitrary block but contains a complete
// layout within.
//
// flex space:
// As each section in one layout has the SAME width, there can be sections
// which are not filled completely in the primary direction with blocks.
// That unused space is distributed equally among the blocks of that section
// which have NA_LAYOUT_GROW set as a blocksize1. See below for further info.
//
// content:
// The content is a virtual rectangle which you can define its dimensions
// arbitrarily if desired by calling naSetLayoutElementContentSize. By default,
// the content size is the same as the blocksize but if it is smaller than the
// blocksize, the user can define, how the content shall be aligned within the
// block. You can control the alignment with naSetLayoutElementPrimaryAlign and
// naSetLayoutElementSecondaryAlign. See below for further info.
//
// uiElement:
// The uiElement finally is the actual system-specific element which will be
// placed on the screen. That uiElement usually occupies the same rectangle
// as the content discussed above but there are exceptions. For example, there
// are uiElements which can not or should not stretch vertically, like for
// example a one-line label. These uiElements will be left unstretched even if
// the content has a different rectangle. Also, the uiElement might be shifted
// vertically to the content rectangle due to text aligment to the baseline.
// You control the latter with naSetLayoutElementAlignBaseline.
//
// For all blocksizes and contentsizes, you can always provide one of those
// three things:
//
// - Positive value      A positive value is treated as a fixed size. Blocks
//                       as well as the content will explicitely be set to that
//                       size.
//
// - any negative value  The absolute amount of the value does not matter but
//   or NA_LAYOUT_MIN    the negative sign means: Use the minimal amount of
//                       space to encapsulate content. For elements, this sets
//                       the size to the size of the actual uiElement provided.
//                       This is especially useful, if you do not (need to)
//                       know the actual size of an element (like for example,
//                       the height of a label). For sections, this computes
//                       the minimal size required to properly encapsulate all
//                       the content. For example imagine a section containing
//                       a small label and a large button. Using NA_LAYOUT_MIN,
//                       the height of the section will be set to the height
//                       of the large button.
//
// - zero (0)            Uses all available space provided by the parent
//   or NA_LAYOUT_GROW   element. For elements, this means, the size will grow
//                       to the size of the containing block. The size will
//                       never shrink to a value smaller than what size would
//                       be achieved with NA_LAYOUT_MIN.
//                       For blocks, the size of all blocks will be distributed
//                       evenly as follows:
//
// If you set the blockSize1 to NA_LAYOUT_GROW, the final size of a block will
// be computed by distributing any remaining space of the containing section
// evenly to all such elements. For example in the following diagram, fixed
// blocks are marked with = and blocks with NA_LAYOUT_GROW are marked with X:
//
// As defined:
// +===+-----+===+---+===+=====+---+ - - - - - - - - +
// |   |  X  |   | X |   |     | X |    flex space   |
// +===+-----+===+---+===+=====+---+ - - - - - - - - +
// After space distribution:
// +===+-----------+===+---------+===+=====+---------+
// |   |     X     |   |    X    |   |     |    X    |
// +===+-----------+===+---------+===+=====+---------+
//
// The alignment of the content within the blocks is defined by the alignment
// setting of the elements. By default, the align is centered. But one can
// choose to align them both in primary as well as in secondary direction.
// For example, if the direction is left-to-right, elements are aligned like
// this:
//
//     BEGIN           CENTER           END
// +-----------+   +-----------+   +-----------+
// +---+       |   |   +---+   |   |       +---+
// |   |       |   |   |   |   |   |       |   |
// +---+       |   |   +---+   |   |       +---+
// +-----------+   +-----------+   +-----------+
//




typedef enum {
  NA_ALIGNMENT_BEGIN,
  NA_ALIGNMENT_CENTER,
  NA_ALIGNMENT_END,
} NAAlignment;

#define NA_LAYOUT_MIN  -1.
#define NA_LAYOUT_GROW  0.

// The default line height for UI elements in NALib
#define NA_LAYOUT_LINE_HEIGHT 24.


// Start a new layout or add a sub-layout.
// When starting a new layout, you must provide a space. All previously
// contained elements in that space will be removed from the space.
void naBeginLayout(
  NASpace* space,
  NABorder2D padding);
  
// Any layout must be ended with naEndLayout.
void naEndLayout(void);

// Adds one section which is a block in the secondary reading direction.
void naAddLayoutSection(
  double blockSize2,
  NABorder2D padding);

// Adds one element in the current section.
// Use preMargin to define a margin to the previous element.
// If uiElement is a space, its children will not be removed automatically.
void naAddLayoutElement(
  void* uiElement,
  double preMargin1,
  double blockSize1);

// Sets the size of the actual content
void naSetLayoutElementContentSize(
  double contentSize1,
  double contentSize2);

// Starts a new sublayout. The new sublayout behaves the same as an arbitrary
// element added with naAddLayoutElement but contains subelements.
// When the sublayout is finished, you must call naEndLayout.
void naBeginSubLayout(
  NASpace* space,
  NABorder2D padding,
  double preMargin1,
  double blockSize1);

// Assigns a space to the last added section. That space is automatically
// resized according to the size of the section. By default, a section has
// no space assigned. If a space is provided, it is taken as is.
// This is useful for example, if you want to paint in the background of a
// section or track whether the mouse is inside.
void naSetLayoutSectionSpace(NASpace* space);

// Changes the attributes of the last element added.
// Defaults are:
// PrimaryAlign:   NA_ALIGNMENT_CENTER
// SecondaryAlign: NA_ALIGNMENT_CENTER
// AligneBaseline: false for block elements, true for baseline elements.
void naSetLayoutElementPrimaryAlign(NAAlignment alignment1);
void naSetLayoutElementSecondaryAlign(NAAlignment alignment2);
void naSetLayoutElementAlignBaseline(NABool alignBaseline);
