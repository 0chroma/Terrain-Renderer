#include "CameraManager.h"

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159
#define PI_2 PI*2

CameraManager::CameraManager(){
    yaw = 0.0;
    pitch = 0.0;
    setPos(0, 0, 0);

    last_cursor_x = -1;
    last_cursor_y = -1;
}

CameraManager::~CameraManager(){

}

void CameraManager::setPos(int a, int b, int c){
    x=a;
    y=b;
    z=c;
}

void CameraManager::frontBack(float d){
    float tmp_lx = cos(yaw)*cos(pitch);
    float tmp_ly = sin(pitch);
    float tmp_lz = sin(yaw)*cos(pitch);

    x = x + d*tmp_lx;
    y = y + d*tmp_ly;
    z = z + d*tmp_lz;
}

void CameraManager::applyPosition(){

    lx = cos(yaw) * cos(pitch);
    ly = sin(pitch);
    lz = sin(yaw) * cos(pitch);

    strafe_lx = cos(yaw - PI_2);
    strafe_lz = sin(yaw - PI_2);

    gluLookAt(x, y, z, x+lx, y+ly, z+lz, 0.0,1.0,0.0);
}

void CameraManager::inputTimer(){
    //if a key is pressed, move the camera by a small increment
    if(up_key == KEY_DOWN){
        frontBack(1.0);
    }else if(down_key == KEY_DOWN){
        frontBack(-1.0);
    }
    last_cursor_x = cursor_x;
    last_cursor_y = cursor_y;
}

void CameraManager::onKey(char key, int state){
    if(key == 'w'){
        up_key = state;
    }else if(key == 's'){
        down_key = state;
    }
}

void CameraManager::onMouseMove(int x, int y){
    cursor_x = x;
    cursor_y = y;
}
