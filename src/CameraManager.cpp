#include "CameraManager.h"

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265
#define PI_2 6.2831853

CameraManager::CameraManager(int winsize_x, int winsize_y){
    yaw = 0.0;
    pitch = 0.0;
    setPos(0, 0, 0);

    window_width = winsize_x;
    window_height = winsize_y;
}

CameraManager::~CameraManager(){

}

void CameraManager::setPos(int a, int b, int c){
    x=a;
    y=b;
    z=c;
}

void CameraManager::move(float d){
    float tmp_lx = cos(yaw)*cos(pitch);
    float tmp_ly = sin(pitch);
    float tmp_lz = sin(yaw)*cos(pitch);

    x += d*tmp_lx;
    y += d*tmp_ly;
    z += d*tmp_lz;
}

void CameraManager::strafe(float d){
    x += d*strafe_lz;
    z += d*strafe_lx;
}

void CameraManager::recalcStrafeVector(){
    strafe_lx = -cos(yaw - PI_2);
    strafe_lz = sin(yaw - PI_2);
}

void CameraManager::rotate(float y, float p){
    float lastPitch = pitch;
    yaw += y;
    pitch += p;

    printf("yaw: %f, pitch: %f\n", yaw, pitch);

    if(pitch > PI/2.1)
        pitch = PI/2.1;
    if(pitch < -PI/2.1)
        pitch = -PI/2.1;

    recalcStrafeVector();
}

void CameraManager::applyPosition(){

    lx = cos(yaw) * cos(pitch);
    ly = sin(pitch);
    lz = sin(yaw) * cos(pitch);

    gluLookAt(x, y, z, x+lx, y+ly, z+lz, 0.0,1.0,0.0);
}

void CameraManager::inputTimer(){
    //if a key is pressed, move the camera by a small increment
    if(up_key == STATE_DOWN){
        move(0.125);
    }
    if(down_key == STATE_DOWN){
        move(-0.125);
    }
    if(left_key == STATE_DOWN){
        strafe(0.125);
    }
    if(right_key == STATE_DOWN){
        strafe(-0.125);
    }

    //mouse processing
    if(mouse_left == STATE_DOWN){
        if(cursor_x != window_width/2 || cursor_y != window_height/2){
            int delta_x = cursor_x - (window_width/2);
            int delta_y = cursor_y - (window_height/2);
            float delta_yaw = (float) delta_x / 120.0;
            float delta_pitch = (float) -delta_y / 120.0;
            rotate(delta_yaw, delta_pitch);
        }
        glutWarpPointer(window_width/2, window_height/2);
        
    }
}

void CameraManager::onKey(char key, int state){
    if(key == 'w'){
        up_key = state;
    }else if(key == 's'){
        down_key = state;
    }else if(key == 'a'){
        left_key = state;
    }else if(key == 'd'){
        right_key = state;
    }
}

void CameraManager::onMouseMove(int x, int y){
    cursor_x = x;
    cursor_y = y;
}

void CameraManager::onMouseButton(int button, int state, int x, int y){
    printf("Button! %i, %i\n", button, state);

    if(button == 0 && state == GLUT_DOWN){
        //reset mouse pos
        glutWarpPointer(window_width/2, window_height/2);
        glutSetCursor(GLUT_CURSOR_NONE);
        
        mouse_left = STATE_DOWN;
    }else if(button == 0 && state == GLUT_UP){
        mouse_left = STATE_UP;
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }


}
