#include "CameraManager.h"
#include "QuadTree.h"
#include "QuadTreeNode.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class TerrainRenderer{
    public:

        TerrainRenderer(CameraManager *cm, QuadTree *qt);
        ~TerrainRenderer();

        void render();
    private:
        GLdouble modelView[16];
        GLdouble projection[16];
        GLdouble clip[16];
        GLdouble frustum[6][4];

        CameraManager *cameraManager;
        QuadTree *quadTree;

        void updateMatricies();
        bool isVisible(double x, double y, double z, double size, double height);
        GLdouble transformHeight(int h);
        void drawHeightmap(GLdouble offsetX, GLdouble offsetY, GLdouble size, QuadTreeNode *node);
        void drawNode(GLdouble offsetX, GLdouble offsetY, GLdouble size, QuadTreeNode *node);
        float getTileDistance(float tile_x, float tile_y, float min, float max, float size);
};
