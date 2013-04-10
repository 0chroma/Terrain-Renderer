#ifndef QuadTree_h
#define QuadTree_h

#include "QuadTreeNode.h"
#include "Image.h"
#include <vector>


class QuadTree {
    public:
        QuadTree();
        QuadTreeNode *root;
        void initialize(Image *img);
    private:
        void createLevel(std::vector< std::vector<QuadTreeNode *> > &lastLevel, int size);
};

#endif
