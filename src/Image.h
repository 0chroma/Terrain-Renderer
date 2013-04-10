#ifndef Image_h
#define Image_h

#include <png.h>
#include <vector>

class Image {
    public:
        Image(const char* file_name);
        ~Image();
        int width, height;
        std::vector<int> get_rgba(int x, int y);
    private:
        png_byte color_type;
        png_byte bit_depth;

        png_structp png_ptr;
        png_infop info_ptr;
        png_bytep *row_pointers;
        void abort_(const char *s, ...);
};

#endif   
