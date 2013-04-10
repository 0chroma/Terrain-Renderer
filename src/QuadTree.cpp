#include "QuadTree.h"

#include <math.h>

using namespace std;

QuadTree::QuadTree(){
    root=0;
}

void QuadTree::initialize(Image *img){
    int dimension = img->height / HEIGHTMAP_SIZE;
    //use this to trick the compiler
    QuadTreeNode* nullNode = 0;
    vector< vector<QuadTreeNode *> > leaves(dimension, vector<QuadTreeNode *>(dimension, nullNode));

    int i, j, x, y, offsetx, offsety;
    for(i=0; i<dimension; i++){
        offsetx = HEIGHTMAP_SIZE * i;
        for(j=0; j<dimension; j++){
            offsety = HEIGHTMAP_SIZE * j;
            QuadTreeNode *node = new QuadTreeNode();
            
            //fill heightmap with data from image
            for(x=0; x<HEIGHTMAP_SIZE; x++){
                for(y=0; y<HEIGHTMAP_SIZE; y++){
                    vector<int> data = img->get_rgba(x+offsetx, y+offsety);
                    //here we're using the red chanel. But we could use anything really.
                    node->heightmap[x][y] = data.at(0);
                }
            }

            leaves.at(i).at(j) = node;
        }
    }

    //now generate the non-leaf nodes
    
    createLevel(leaves, dimension);

    root = new QuadTreeNode();
    root->tlNode = leaves.at(0).at(0);
    root->trNode = leaves.at(1).at(0);
    root->blNode = leaves.at(0).at(1);
    root->brNode = leaves.at(1).at(1);
    root->generateMatrixFromChildren();
}

void QuadTree::createLevel(std::vector< std::vector<QuadTreeNode*> >& lastLevel, int size){
    if(size == 2)
        return;

    int newSize = size/2;
    
    int i, j;
    for(i=0; i<newSize; i++){
        for(j=0; j<newSize; j++){
            QuadTreeNode *node = new QuadTreeNode();
            node->tlNode = lastLevel[i*2][j*2];
            node->trNode = lastLevel[(i*2)+1][j*2];
            node->blNode = lastLevel[i*2][(j*2)+1];
            node->brNode = lastLevel[(i*2)+1][(j*2)+1];
            lastLevel.at(i).at(j) = node;
        }
    }
    createLevel(lastLevel, newSize);
}
