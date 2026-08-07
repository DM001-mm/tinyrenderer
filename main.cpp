#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include "tgaimage.h"

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

//==================================================================
// 主流程
//==================================================================
int main() {
    // ---- 阶段1：3D → 2D，生成 lines.txt ----
    read_vertex("obj/african_head/african_head.obj");
    project_and_save("obj/african_head/african_head.obj", "lines.txt", 800, 800);

    // ---- 阶段2：读取 lines.txt，绘制 TGA ----
    TGAImage framebuffer(800, 800, TGAImage::RGB);

    std::ifstream f("lines.txt");
    if (!f) { printf("无法打开 lines.txt\n"); return 1; }

    std::string str;
    while (std::getline(f, str)) {
        std::istringstream iss(str);
        int x1, y1, x2, y2, r, g, b;
        iss >> x1 >> y1 >> x2 >> y2 >> r >> g >> b;

        // TGAColor 按 BGRA 顺序
        line(x1, y1, x2, y2, framebuffer,
             {static_cast<std::uint8_t>(b),
              static_cast<std::uint8_t>(g),
              static_cast<std::uint8_t>(r), 255});
    }

    framebuffer.write_tga_file("framebuffer.tga");
    printf("framebuffer.tga 已生成\n");
    return 0;
}
