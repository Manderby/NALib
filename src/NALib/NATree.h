
typedef struct NATree NATree;
typedef struct NATreeIterator NATreeIterator;


NATree* naInitTree(NATree* tree);
void naClearTree();

void naAddTreeLeaf(NATree* tree, double key, void* leaf);



NATreeIterator naMakeTreeAccessor(const NATree* tree);
NATreeIterator naMakeTreeMutator(NATree* tree);
NATreeIterator naMakeTreeModifier(NATree* tree);
void naClearTreeIterator(NATreeIterator* iter);
