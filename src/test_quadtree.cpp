#include "Image.h"
#include "QuadTree.h"

#include <stdio.h>
#include <vector>

int main(int argc, char **argv)
{
    Image *img = new Image("./data/black.png");

    std::vector<int> data = img->get_rgba(0, 0);

    printf("rgba: %d/%d/%d/%d\n", data.at(0), data.at(1), data.at(2), data.at(3));

    QuadTree *tree = new QuadTree();
    tree->initialize(img);

    printf("Root node heightmap: \n");
    tree->root->printHeightmap();
    printf("top leftmost leaf: \n");
    QuadTreeNode *cursor = tree->root;
    while(cursor->tlNode){
        cursor = cursor->tlNode;
    }
    cursor->printHeightmap();

	return 0;
}
