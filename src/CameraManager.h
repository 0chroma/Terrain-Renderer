#ifndef CameraManager_h
#define CameraManager_h

class CameraManager {
    public:
        static const int KEY_UP = 0;
        static const int KEY_DOWN = 1;

        CameraManager();
        ~CameraManager();
        void applyPosition();
        void onKey(char key, int state);
        void onMouseMove(int x, int y);
        void inputTimer();

        void setPos(int a, int b, int c);
        void frontBack(float d);
    private:

        int up_key;
        int down_key;
        int left_key;
        int right_key;
        int last_cursor_x;
        int last_cursor_y;
        int cursor_x;
        int cursor_y;

        //position
        float x, y, z;
        //look vector
        float lx, ly, lz;
        //rot angles
        float pitch, yaw;
        //90 degrees to look vector
        float strafe_lx, strafe_lz;
};

#endif
