#include <cmath>
#include "tgaimage.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void drawline(int ax,int ay,int bx,int by,TGAImage& image,TGAColor color){
    for(double t=0.0;t<1;t+=.01){
        int x = std::round(ax+(bx-ax)*t);
        int y = std::round(ay+(by-ay)*t);
        image.set(x,y,color); // 设置颜色
    }    
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    int ax =  7, ay =  3;
    int bx = 12, by = 37;
    int cx = 62, cy = 53;

    // framebuffer.set(ax, ay, white); //给TGA上面的点 设置颜色
    // framebuffer.set(bx, by, white); 
    // framebuffer.set(cx, cy, white);
    
    drawline(ax, ay, bx,by,framebuffer,white);  //给TGA上面的点 设置颜色
    drawline(bx, by, cx,cy,framebuffer,white); 
    drawline(cx, cy,  ax,ay,framebuffer,white);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

