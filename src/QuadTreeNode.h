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
        QuadTreeNode *parent;

        int location;
        static const int TOP_LEFT = 0;
        static const int TOP_RIGHT = 1;
        static const int BOTTOM_LEFT = 2;
        static const int BOTTOM_RIGHT = 3;
        
        bool isLeaf();
        bool isRoot();
        void generateMatrixFromChildren();
        void printHeightmap();
};

#endif   
