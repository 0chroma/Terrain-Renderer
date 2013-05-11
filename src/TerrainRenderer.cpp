#include "TerrainRenderer.h"

#include <math.h>
#include <GL/glut.h>

TerrainRenderer::TerrainRenderer(CameraManager *cm, QuadTree *qt){
    cameraManager = cm;
    quadTree = qt;
}

void TerrainRenderer::updateMatricies(){
    glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    //multiply the two matricies to combine them
    clip[ 0] = modelView[ 0] * projection[ 0] + modelView[ 1] * projection[ 4] + modelView[ 2] * projection[ 8] + modelView[ 3] * projection[12];
    clip[ 1] = modelView[ 0] * projection[ 1] + modelView[ 1] * projection[ 5] + modelView[ 2] * projection[ 9] + modelView[ 3] * projection[13];
    clip[ 2] = modelView[ 0] * projection[ 2] + modelView[ 1] * projection[ 6] + modelView[ 2] * projection[10] + modelView[ 3] * projection[14];
    clip[ 3] = modelView[ 0] * projection[ 3] + modelView[ 1] * projection[ 7] + modelView[ 2] * projection[11] + modelView[ 3] * projection[15];

    clip[ 4] = modelView[ 4] * projection[ 0] + modelView[ 5] * projection[ 4] + modelView[ 6] * projection[ 8] + modelView[ 7] * projection[12];
    clip[ 5] = modelView[ 4] * projection[ 1] + modelView[ 5] * projection[ 5] + modelView[ 6] * projection[ 9] + modelView[ 7] * projection[13];
    clip[ 6] = modelView[ 4] * projection[ 2] + modelView[ 5] * projection[ 6] + modelView[ 6] * projection[10] + modelView[ 7] * projection[14];
    clip[ 7] = modelView[ 4] * projection[ 3] + modelView[ 5] * projection[ 7] + modelView[ 6] * projection[11] + modelView[ 7] * projection[15];

    clip[ 8] = modelView[ 8] * projection[ 0] + modelView[ 9] * projection[ 4] + modelView[10] * projection[ 8] + modelView[11] * projection[12];
    clip[ 9] = modelView[ 8] * projection[ 1] + modelView[ 9] * projection[ 5] + modelView[10] * projection[ 9] + modelView[11] * projection[13];
    clip[10] = modelView[ 8] * projection[ 2] + modelView[ 9] * projection[ 6] + modelView[10] * projection[10] + modelView[11] * projection[14];
    clip[11] = modelView[ 8] * projection[ 3] + modelView[ 9] * projection[ 7] + modelView[10] * projection[11] + modelView[11] * projection[15];

    clip[12] = modelView[12] * projection[ 0] + modelView[13] * projection[ 4] + modelView[14] * projection[ 8] + modelView[15] * projection[12];
    clip[13] = modelView[12] * projection[ 1] + modelView[13] * projection[ 5] + modelView[14] * projection[ 9] + modelView[15] * projection[13];
    clip[14] = modelView[12] * projection[ 2] + modelView[13] * projection[ 6] + modelView[14] * projection[10] + modelView[15] * projection[14];
    clip[15] = modelView[12] * projection[ 3] + modelView[13] * projection[ 7] + modelView[14] * projection[11] + modelView[15] * projection[15];

    //next we grab the planes from the clip matrix and normalize them
    GLdouble t;

    //right
    frustum[0][0] = clip[ 3] - clip[ 0];
    frustum[0][1] = clip[ 7] - clip[ 4];
    frustum[0][2] = clip[11] - clip[ 8];
    frustum[0][3] = clip[15] - clip[12];

    t = sqrt(frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2]);
    frustum[0][0] /= t;
    frustum[0][1] /= t;
    frustum[0][2] /= t;
    frustum[0][3] /= t;

    //left
    frustum[1][0] = clip[ 3] + clip[ 0];
    frustum[1][1] = clip[ 7] + clip[ 4];
    frustum[1][2] = clip[11] + clip[ 8];
    frustum[1][3] = clip[15] + clip[12];

    t = sqrt(frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2]);
    frustum[1][0] /= t;
    frustum[1][1] /= t;
    frustum[1][2] /= t;
    frustum[1][3] /= t;

    //bottom
    frustum[2][0] = clip[ 3] + clip[ 1];
    frustum[2][1] = clip[ 7] + clip[ 5];
    frustum[2][2] = clip[11] + clip[ 9];
    frustum[2][3] = clip[15] + clip[13];

    t = sqrt(frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2]);
    frustum[2][0] /= t;
    frustum[2][1] /= t;
    frustum[2][2] /= t;
    frustum[2][3] /= t;

    //top
    frustum[3][0] = clip[ 3] - clip[ 1];
    frustum[3][1] = clip[ 7] - clip[ 5];
    frustum[3][2] = clip[11] - clip[ 9];
    frustum[3][3] = clip[15] - clip[13];

    t = sqrt(frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2]);
    frustum[3][0] /= t;
    frustum[3][1] /= t;
    frustum[3][2] /= t;
    frustum[3][3] /= t;

    //far
    frustum[4][0] = clip[ 3] - clip[ 2];
    frustum[4][1] = clip[ 7] - clip[ 6];
    frustum[4][2] = clip[11] - clip[10];
    frustum[4][3] = clip[15] - clip[14];

    t = sqrt(frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2]);
    frustum[4][0] /= t;
    frustum[4][1] /= t;
    frustum[4][2] /= t;
    frustum[4][3] /= t;

    //near
    frustum[5][0] = clip[ 3] + clip[ 2];
    frustum[5][1] = clip[ 7] + clip[ 6];
    frustum[5][2] = clip[11] + clip[10];
    frustum[5][3] = clip[15] + clip[14];

    t = sqrt(frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2]);
    frustum[5][0] /= t;
    frustum[5][1] /= t;
    frustum[5][2] /= t;
    frustum[5][3] /= t;
}

bool TerrainRenderer::isVisible(double x, double y, double z, double size, double height){
    //do a basic sphere test cause it's fast and easy
    double halfsize = size / 2;
    double halfheight = height / 2;
    double radiusToEdge = sqrt(halfsize*halfsize + height*height);
    double radius = sqrt(radiusToEdge*radiusToEdge + halfsize*halfsize);
    for(int i=0; i<6; i++){
        if(frustum[i][0]*x + frustum[i][1]*(y+halfheight) + frustum[i][2]*z + frustum[i][3] <= -radius)
            return false;
    }
    return true;
}

GLdouble TerrainRenderer::transformHeight(int h){
    //maps height from the quadtree to a height in the modelview
    return h / (255.0*2.0);
}

void TerrainRenderer::drawHeightmap(GLdouble offsetX, GLdouble offsetY, GLdouble size, QuadTreeNode *node){

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glPushMatrix();
    glTranslatef(offsetX, 0.0, offsetY);

    glBegin(GL_TRIANGLES);

    GLdouble vertexSpacing;
    if(node->isRoot())
        vertexSpacing = size / (HEIGHTMAP_SIZE-1);
    else
        vertexSpacing = size / (HEIGHTMAP_SIZE);

    for(int i=0; i<(HEIGHTMAP_SIZE-1); i++){
        for(int j=0; j<(HEIGHTMAP_SIZE-1); j++){
            GLdouble x = (i)*vertexSpacing;
            GLdouble z = (j)*vertexSpacing;

            GLdouble h1 = transformHeight(node->heightmap[i][j]);
            GLdouble h2 = transformHeight(node->heightmap[i+1][j]);
            GLdouble h3 = transformHeight(node->heightmap[i][j+1]);
            GLdouble h4 = transformHeight(node->heightmap[i+1][j+1]);
            //triangle 1
            glVertex3d(x, h1, z);
            glVertex3d(x+vertexSpacing, h2, z);
            glVertex3d(x+vertexSpacing, h4, z+vertexSpacing);
            //triangle 2
            glVertex3d(x, h1, z);
            glVertex3d(x, h3, z+vertexSpacing);
            glVertex3d(x+vertexSpacing, h4, z+vertexSpacing);
        }
    }

    //unless we're rendering the root, we need to take into account that we need an extra row/column of verticies to join
    //the four heightmaps together
    if(!node->isRoot()){

    glPushMatrix();
    glTranslated(-vertexSpacing, 0.0, -vertexSpacing);

        //for nodes that have a neighbor above, draw the bottom-most row from the top neighbor
        if(node->location == QuadTreeNode::BOTTOM_LEFT || node->location == QuadTreeNode::BOTTOM_RIGHT){
            for(int i=0; i<(HEIGHTMAP_SIZE-1); i++){
                int h1, h2, h3, h4;
                if(node->location == QuadTreeNode::BOTTOM_LEFT){
                    h1 = node->parent->tlNode->heightmap[i][HEIGHTMAP_SIZE-1];
                    h2 = node->parent->tlNode->heightmap[i+1][HEIGHTMAP_SIZE-1];
                }else{
                    h1 = node->parent->trNode->heightmap[i][HEIGHTMAP_SIZE-1];
                    h2 = node->parent->trNode->heightmap[i+1][HEIGHTMAP_SIZE-1];
                }
                h3 = node->heightmap[i][0];
                h4 = node->heightmap[i+1][0];

                GLfloat y1 = transformHeight(h1);
                GLfloat y2 = transformHeight(h2);
                GLfloat y3 = transformHeight(h3);
                GLfloat y4 = transformHeight(h4);
                GLdouble x = (i+1)*vertexSpacing;
                GLdouble z = 0;
                //triangle 1
                glVertex3d(x, y1, z);
                glVertex3d(x+vertexSpacing, y2, z);
                glVertex3d(x+vertexSpacing, y4, z+vertexSpacing);
                //triangle 2
                glVertex3d(x, y1, z);
                glVertex3d(x, y3, z+vertexSpacing);
                glVertex3d(x+vertexSpacing, y4, z+vertexSpacing);
                
            }

        }
        //for nodes that have a neighbor to the right, draw the leftmost row from the right neighbor
        if(node->location == QuadTreeNode::BOTTOM_LEFT || node->location == QuadTreeNode::TOP_LEFT){
            for(int i=0; i<(HEIGHTMAP_SIZE-1); i++){
                int h1, h2, h3, h4;
                if(node->location == QuadTreeNode::BOTTOM_LEFT){
                    h2 = node->parent->brNode->heightmap[0][i];
                    h4 = node->parent->brNode->heightmap[0][i+1];
                }else{
                    h2 = node->parent->trNode->heightmap[0][i];
                    h4 = node->parent->trNode->heightmap[0][i+1];
                }
                h1 = node->heightmap[HEIGHTMAP_SIZE-1][i];
                h3 = node->heightmap[HEIGHTMAP_SIZE-1][i+1];

                GLfloat y1 = transformHeight(h1);
                GLfloat y2 = transformHeight(h2);
                GLfloat y3 = transformHeight(h3);
                GLfloat y4 = transformHeight(h4);

                GLdouble x = (HEIGHTMAP_SIZE)*vertexSpacing;
                GLdouble z = (i+1)*vertexSpacing; //(HEIGHTMAP_SIZE-1)*vertexSpacing;
                //triangle 1
                glVertex3d(x, y1, z);
                glVertex3d(x+vertexSpacing, y2, z);
                glVertex3d(x+vertexSpacing, y4, z+vertexSpacing);
                //triangle 2
                glVertex3d(x, y1, z);
                glVertex3d(x, y3, z+vertexSpacing);
                glVertex3d(x+vertexSpacing, y4, z+vertexSpacing);
                
            }

        }
        //for the bottom left node, create a central quad that joins the three other neighbors to us
        if(node->location == QuadTreeNode::BOTTOM_LEFT){
            GLdouble h1 = transformHeight(node->parent->tlNode->heightmap[HEIGHTMAP_SIZE-1][HEIGHTMAP_SIZE-1]);
            GLdouble h2 = transformHeight(node->parent->trNode->heightmap[0][HEIGHTMAP_SIZE-1]);
            GLdouble h3 = transformHeight(node->heightmap[HEIGHTMAP_SIZE-1][0]);
            GLdouble h4 = transformHeight(node->parent->brNode->heightmap[0][0]);
            GLdouble x = (HEIGHTMAP_SIZE)*vertexSpacing;
            GLdouble z = 0; //(HEIGHTMAP_SIZE-1)*vertexSpacing;

            //triangle 1
            glVertex3d(x, h1, z);
            glVertex3d(x+vertexSpacing, h2, z);
            glVertex3d(x+vertexSpacing, h4, z+vertexSpacing);
            //triangle 2
            glVertex3d(x, h1, z);
            glVertex3d(x, h3, z+vertexSpacing);
            glVertex3d(x+vertexSpacing, h4, z+vertexSpacing);

        }
        glPopMatrix();
    }
    glEnd();

    glPopMatrix();
}

void TerrainRenderer::render(){
    updateMatricies();

    //Test code
    float cam_x, cam_y, cam_z;
    cameraManager->getPos(&cam_x, &cam_y, &cam_z);

    
    /*QuadTreeNode *cursor = quadTree->root;
    while(cursor->tlNode){
        cursor = cursor->tlNode;
    }
    drawHeightmap(0.0, 0.0, 1.0, cursor->heightmap);*/

    QuadTreeNode *r = quadTree->root;
    //drawHeightmap(0.0, 0.0, 1.0, r->tlNode);
    drawHeightmap(0.0, 0.0, 0.5, r->tlNode->tlNode);
    drawHeightmap(0.0, 0.5, 0.5, r->tlNode->blNode);
    drawHeightmap(0.5, 0.0, 0.5, r->tlNode->trNode);
    drawHeightmap(0.5, 0.5, 0.5, r->tlNode->brNode);

    drawHeightmap(0.0, 1.0, 1.0, r->blNode);
    drawHeightmap(1.0, 0.0, 1.0, r->trNode);

    drawHeightmap(1.0, 1.0, 0.5, r->brNode->tlNode);
    drawHeightmap(1.0, 1.5, 0.5, r->brNode->blNode);
    drawHeightmap(1.5, 1.0, 0.5, r->brNode->trNode);
    drawHeightmap(1.5, 1.5, 0.5, r->brNode->brNode);


    /*
    printf("Camera: (%f, %f, %f)\n", cam_x, cam_y, cam_z);

    if(isVisible(0.0, 0.0, 0.0, 1.0, 1.0)){
        printf("Visible!\n");
        glutSolidCube(1.0f);
    }*/
}
