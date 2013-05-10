#ifndef CameraManager_h
#define CameraManager_h

class CameraManager {
    public:
        static const int STATE_UP = 0;
        static const int STATE_DOWN = 1;

        CameraManager(int winsize_x, int winsize_y);
        ~CameraManager();
        void applyPosition();
        void onKey(char key, int state);
        void onMouseMove(int x, int y);
        void onMouseButton(int button, int state, int x, int y);
        void inputTimer();

        void setPos(int a, int b, int c);
        void rotate(float y, float p);
        void move(float d);
        void strafe(float d);
        
        int window_width;
        int window_height;
    private:

        int up_key;
        int down_key;
        int left_key;
        int right_key;
        int mouse_left;
        int cursor_x;
        int cursor_y;

        void recalcStrafeVector();

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
