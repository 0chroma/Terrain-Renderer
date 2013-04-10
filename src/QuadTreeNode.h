#ifndef QuadTreeNode_h
#define QuadTreeNode_h

#define HEIGHTMAP_SIZE 32

class QuadTreeNode {
    public:
        QuadTreeNode();
        int heightmap[HEIGHTMAP_SIZE][HEIGHTMAP_SIZE];
        QuadTreeNode *tlNode;
        QuadTreeNode *trNode;
        QuadTreeNode *blNode;
        QuadTreeNode *brNode;
        bool isLeaf();
        void generateMatrixFromChildren();
        void printHeightmap();
};

#endif   
