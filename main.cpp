#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include "tgaimage.h"

constexpr TGAColor white  = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green  = {  0, 255,   0, 255};
constexpr TGAColor red    = {  0,   0, 255, 255};
constexpr TGAColor blue   = {255, 128,  64, 255};
constexpr TGAColor yellow = {  0, 200, 255, 255};

// ---- 声明 model.cpp 里的函数 ----
void read_vertex(const std::string& filename);
void project_and_save(const std::string& infilename,
                      const std::string& outfilename,
                      int width, int height);

// ---- 画线算法 ----
void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color) {
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) { std::swap(ax, ay); std::swap(bx, by); }
    if (ax > bx) { std::swap(ax, bx); std::swap(ay, by); }

    int y = ay;
    int ierror = 0;
    for (int x = ax; x <= bx; x++) {
        if (steep)
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);

        ierror += 2 * std::abs(by - ay);
        if (ierror > bx - ax) {
            y += by > ay ? 1 : -1;
            ierror -= 2 * (bx - ax);
        }
    }
}

// ---- cmp 用于按 y 排序，y 相同按 x ----
bool cmp(const std::pair<int,int>& a, const std::pair<int,int>& b) {
    if (a.second != b.second) return a.second < b.second;
    return a.first < b.first;
}

// ---- 2D 三角形光栅化（填色） ----
void triangle_Scanline_Rasterization(int ax, int ay, int bx, int by, int cx, int cy,
              TGAImage& image, TGAColor color) {
    // 将三个顶点按 y 从小到大排序
    std::vector<std::pair<int,int>> pts;
    pts.push_back(std::make_pair(ax, ay));
    pts.push_back(std::make_pair(bx, by));
    pts.push_back(std::make_pair(cx, cy));
    std::sort(pts.begin(), pts.end(), cmp);

    auto p1 = pts[0];  // y 最小
    auto p2 = pts[1];  // y 中间
    auto p3 = pts[2];  // y 最大

    // 上半部分：y 从 p1.y 到 p2.y
    if (p1.second != p2.second) {
        for (int y = p1.second; y <= p2.second; y++) {
            int x1 = p1.first + (y - p1.second) * (p3.first - p1.first) / (p3.second - p1.second);
            int x2 = p1.first + (y - p1.second) * (p2.first - p1.first) / (p2.second - p1.second);
            if (x1 > x2) std::swap(x1, x2);
            line(x1, y, x2, y, image, color);
        }
    }

    // 下半部分：y 从 p2.y 到 p3.y
    if (p2.second != p3.second) {
        for (int y = p2.second; y <= p3.second; y++) {
            int x1 = p1.first + (y - p1.second) * (p3.first - p1.first) / (p3.second - p1.second);
            int x2 = p2.first + (y - p2.second) * (p3.first - p2.first) / (p3.second - p2.second);
            if (x1 > x2) std::swap(x1, x2);
            line(x1, y, x2, y, image, color);
        }
    }
}
// 有三角形 三个点 可以默认设置成 a,b,c 那么，只需要通过外积的正负号的一致性 就可以判断是否 是三角形内部的点 ,外积的时候使用外部 逆时针
bool judge(int x,int y,int ax,int ay,int bx,int by,int cx,int cy){
    int r1 = (x-ax)*(by-ay)-(y-ay)*(bx-ax);
    int r2 = (x-bx)*(cy-by) -(y-by)*(cx-bx);
    int r3 = (x-cx)*(ay-cy)-(y-cy)*(ax-cx);
    return (r1>=0&&r2>=0&&r3>=0)||(r1<=0&&r2<=0&&r3<=0);   
}
void triangle(int ax, int ay, int bx, int by, int cx, int cy,TGAImage& image, TGAColor color) {
    int lx = std::min({ax,bx,cx});
    int rx = std::max({ax,bx,cx});
    int ly = std::min({ay,by,cy});
    int hy = std::max({ay,by,cy});
    // 确定边框范围
    #pragma omp parallel for num_threads(4) // 这个 语句 也是相当神奇的
    for(int ix = lx;ix<=rx;ix++){
        for(int iy=ly;iy<=hy;iy++){
            if(judge(ix,iy,ax,ay,bx,by,cx,cy)) image.set(ix,iy,color);
        }
    }
}
// void triangle_
//==================================================================
// 主流程
//==================================================================
int main() {
    TGAImage framebuffer(800, 800, TGAImage::RGB);

    // ---- test triangle ----
    triangle(100, 100, 700, 150, 300, 650, framebuffer, green);

    // ---- 3D wireframe ----
    // read_vertex("obj/african_head/african_head.obj");
    // project_and_save("obj/african_head/african_head.obj", "lines.txt", 800, 800);

    // std::ifstream f("lines.txt");
    // if (!f) { printf("Cannot open lines.txt\n"); return 1; }

    // std::string str;
    // while (std::getline(f, str)) {
    //     std::istringstream iss(str);
    //     int x1, y1, x2, y2, r, g, b;
    //     iss >> x1 >> y1 >> x2 >> y2 >> r >> g >> b;

    //     line(x1, y1, x2, y2, framebuffer,
    //          {static_cast<std::uint8_t>(b),
    //           static_cast<std::uint8_t>(g),
    //           static_cast<std::uint8_t>(r), 255});
    // }

    framebuffer.write_tga_file("framebuffer.tga");
    printf("framebuffer.tga 已生成\n");
    return 0;
}
