#include "QuadTreeNode.h"

#include <stdio.h>

QuadTreeNode::QuadTreeNode(){
    tlNode = 0;
    trNode = 0;
    blNode = 0;
    brNode = 0;
}

bool QuadTreeNode::isLeaf(){
    return tlNode == 0 && trNode == 0 && blNode == 0 && brNode == 0;
}

void QuadTreeNode::generateMatrixFromChildren(){
    if(isLeaf()) return;
    int i, j, x, y;
    for(i=0; i<2; i++){
        for(j=0; j<2; j++){
            QuadTreeNode *child;
            if(i==0 && j==0) child = tlNode;
            if(i==1 && j==0) child = trNode;
            if(i==0 && j==1) child = blNode;
            if(i==1 && j==1) child = brNode;
            child->generateMatrixFromChildren();
            for(x=0; x<HEIGHTMAP_SIZE; x+=2){
                for(y=0; y<HEIGHTMAP_SIZE; y+=2){
                    heightmap[(i*(HEIGHTMAP_SIZE/2))+(x/2)][(j*(HEIGHTMAP_SIZE/2))+(y/2)] = child->heightmap[x][y];
                }
            }
        }
    }
}

void QuadTreeNode::printHeightmap(){
    int i, j;
    for(i=0; i<32; i++){
        for(j=0; j<32; j++){
            printf("%3d ", heightmap[i][j]);
        }
        printf("\n");
    }
}
