
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
// You alternate between adding sections with naAddLayoutSection and elements
// with naAllLayoutElement.
//
// When using naAddLayoutElement, you actually create a layout block which
// itelf contais the element you want to add.
//
// Both each block and each element have a primary and secondary size, whereas
// the meaning of primary and secondary is defined by the layout direction set
// by the containing space.
//
// The element size is set by the arguments provided to naAddLayoutElement.
//
// The block size is set as follows:
// Primary size:     By default, this value is equal to the elements primary
//                   size but can be changed with naSetLayoutElementBlockSize1.
// Secondary size:   Is set by naAddLayoutSection.
//
// Blocks and their contained elements may therefore have different sizes.
// Both can be individually set to be either an explicitely fixed value or
// implicitely being computed:
//
// For all sizes, you can always provide one of those three things:
//
// - Positive value      A positive value is treated as a fixed size. Blocks
//                       as well as elements will explicitely be set to that
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
//                       never shrink to a value smaller than the initial size
//                       upon creation of the uiElement.
//                       For blocks, the size of all blocks will be distributed
//                       evenly as follows:
//
// If you set the blockSize to NA_LAYOUT_GROW, the final size will be computed
// by distributing any remaining space of the containing rectangle evenly to
// all such elements. For example in the following diagram, fixed blocks are
// marked with = and blocks with NA_LAYOUT_GROW are marked with X:
//
// As defined:
// +===+-----+===+---+===+=====+---+ - - - - - - - - +
// |   |  X  |   | X |   |     | X | undefined space |
// +===+-----+===+---+===+=====+---+ - - - - - - - - +
// After space distribution:
// +===+-----------+===+---------+===+=====+---------+
// |   |     X     |   |    X    |   |     |    X    |
// +===+-----------+===+---------+===+=====+---------+
//
// The alignment of the elements within the blocks is defined by the alignment
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
  NA_ALIGN_BEGIN,
  NA_ALIGN_CENTER,
  NA_ALIGN_END,
} NAAlignment;

#define NA_LAYOUT_MIN  -1.
#define NA_LAYOUT_GROW  0.

// The default line height for UI elements in NALib
#define NA_LAYOUT_LINE_HEIGHT 24.


// Start a new layout or add a sub-layout.
// When starting a new layout, you must provide a space.
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
void naAddLayoutElement(
  void* uiElement,
  double preMargin1,
  double contentSize1,
  double contentSize2);

// Assigns a space to the last added section. That space is automatically
// resized according to the size of the section. By default, a section has
// so space assigned.
// This is useful for example, if you want to paint in the background of a
// seciton or track whether the mouse is inside.
void naSetLayoutSectionSpace(NASpace* space);

// Changes the attributes of the last element added.
// Defaults are:
// PrimaryAlign:   NA_ALIGN_CENTER
// SecondaryAlign: NA_ALIGN_CENTER
// AligneBaseline: false for block elements, true for baseline elements.
// BlockSize1:     same as size1 for elements. NA_LAYOUT_GROW for layouts.
void naSetLayoutElementPrimaryAlign(NAAlignment alignment1);
void naSetLayoutElementSecondaryAlign(NAAlignment alignment2);
void naSetLayoutElementAlignBaseline(NABool alignBaseline);
void naSetLayoutElementBlockSize1(double blockSize1);
