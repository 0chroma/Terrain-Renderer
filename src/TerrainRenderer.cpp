#include "TerrainRenderer.h"

#include <math.h>
#include <algorithm>
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
    vertexSpacing = size / (HEIGHTMAP_SIZE-1);

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

    glEnd();

    glPopMatrix();
}

void TerrainRenderer::drawNode(GLdouble offsetX, GLdouble offsetY, GLdouble size, QuadTreeNode *node){
    size = size/2;
    for(int i=0; i<2; i++){
        for(int j=0; j<2; j++){
            QuadTreeNode *child;
            if(i==0 && j==0) child = node->tlNode;
            if(i==1 && j==0) child = node->trNode;
            if(i==0 && j==1) child = node->blNode;
            if(i==1 && j==1) child = node->brNode;
            GLfloat tile_x = size*i+offsetX;
            GLfloat tile_y = size*i+offsetY;
            
            GLfloat d = getTileDistance(tile_x, tile_y, transformHeight(child->min), transformHeight(child->max), size);

            if(tile_x == 0.0 && tile_y == 0.0 && size == 0.5)
                printf("Distance from %i, %i: %f\n", i, j, d);
            if(d < size*2 && !child->isLeaf()){
                drawNode(size*i+offsetX, size*j+offsetY, size, child);
            }else{
                drawHeightmap(size*i+offsetX, size*j+offsetY, size, child);
            }
        }
    }
}

float TerrainRenderer::getTileDistance(float tile_x, float tile_y, float min, float max, float size){
    float cam_x, cam_y, cam_z;
    cameraManager->getPos(&cam_x, &cam_y, &cam_z);
            
    /*GLfloat d1 = sqrt((cam_x-tile_x)*(cam_x-tile_x)+(cam_y-tile_y)*(cam_y-tile_y) + (cam_z*cam_z));
    tile_x += size;
    GLfloat d2 = sqrt((cam_x-tile_x)*(cam_x-tile_x)+(cam_y-tile_y)*(cam_y-tile_y) + (cam_z*cam_z));
    tile_x -= size; tile_y += size;
    GLfloat d3 = sqrt((cam_x-tile_x)*(cam_x-tile_x)+(cam_y-tile_y)*(cam_y-tile_y) + (cam_z*cam_z));
    tile_x += size;
    GLfloat d4 = sqrt((cam_x-tile_x)*(cam_x-tile_x)+(cam_y-tile_y)*(cam_y-tile_y) + (cam_z*cam_z));

    using namespace std;
    return min(min(d1, d2), min(d3, d4));
    */

    //get the center of the tile
    tile_x += size/2;
    tile_y += size/2;
    float tile_z = (min+max)/2;

    return sqrt((cam_x-tile_x)*(cam_x-tile_x)+(cam_y-tile_y)*(cam_y-tile_y) + ((cam_z-tile_z)*(cam_z-tile_z)));
 
}

void TerrainRenderer::render(){
    updateMatricies();

    float cam_x, cam_y, cam_z;
    cameraManager->getPos(&cam_x, &cam_y, &cam_z);






    //Test code
    
    /*QuadTreeNode *cursor = quadTree->root;
    while(cursor->tlNode){
        cursor = cursor->tlNode;
    }
    drawHeightmap(0.0, 0.0, 1.0, cursor->heightmap);*/

    QuadTreeNode *r = quadTree->root;
    drawNode(0.0, 0.0, 1.0, r);

    /*
    printf("Camera: (%f, %f, %f)\n", cam_x, cam_y, cam_z);

    if(isVisible(0.0, 0.0, 0.0, 1.0, 1.0)){
        printf("Visible!\n");
        glutSolidCube(1.0f);
    }*/
}
