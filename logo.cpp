#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <vector>
#include <thread>
#include "stb_image_write.h"

using u8 = unsigned char;
using f32 = float;
using f64 = double;

struct Color
{
    u8 r,g,b,a;
};

struct vec2
{
    f64 x,y;
    vec2(f64 x, f64 y) : x(x), y(y) {}
    vec2(f64 s = 0) : x(s), y(s) {}
};

vec2 operator + (vec2 l, vec2 r){ return {l.x + r.x, l.y + r.y};}
vec2 operator - (vec2 l, vec2 r){ return {l.x - r.x, l.y - r.y};}
vec2 operator * (vec2 l, vec2 r){ return {l.x * r.x, l.y * r.y};}
vec2 operator / (vec2 l, vec2 r){ return {l.x / r.x, l.y / r.y};}
vec2 operator + (f64 l, vec2 r){ return vec2(l) + r; }
vec2 operator - (f64 l, vec2 r){ return vec2(l) - r; }
vec2 operator * (f64 l, vec2 r){ return vec2(l) * r; }
vec2 operator / (f64 l, vec2 r){ return vec2(l) / r; }

f64 sum   (vec2 v)         { return v.x + v.y; }
f64 dot   (vec2 l, vec2 r) { return sum(l * r); }
f64 cross (vec2 l, vec2 r) { return dot(l, vec2(r.y, -r.x)); }
f64 length(vec2 v)         { return sqrt(dot(v,v)); }

vec2 bary(vec2 p, vec2 p0, vec2 p1, vec2 p2)
{
    vec2 v0 = p1 - p0;
    vec2 v1 = p2 - p0;
    vec2 v2 = p  - p0;
    return vec2(cross(v2,v1), cross(v0,v2)) / cross(v0,v1);
}

void gen_logo(const char* out, const Color fill, const Color empty, vec2 extent)
{
    std::vector<Color> img = {size_t(extent.y * extent.x), empty};

    const vec2 N = vec2(2,-2) / extent;
    const f64 sq3h = sqrt(3) *.5;
    const f64 outer = 236 / 256.f;
    const f64 inner = 216 / 256.f;

    const vec2 p0 = inner * vec2(0, 1);
    const vec2 p1 = inner * vec2(-sq3h, -.5);
    const vec2 p2 = inner * vec2(+sq3h, -.5);

    for(int j = 0; j < extent.y; ++j)
    {
        for(int i = 0; i < extent.x; ++i)
        {
            vec2 uv = vec2(i, j) * N - vec2(1, -1);
            vec2 coords = bary(uv, p0, p1, p2);
            f64 len = length(uv);
            if((coords.x > 0 && coords.y > 0 && coords.x + coords.y  < 1) || (len > inner && len < outer))
                img[j * extent.x + i] = fill;
        }
    }

    stbi_write_png(out, extent.x, extent.y, 4, img.data(), extent.x * 4);
}

int main()
{
    std::thread threads[4] = {
        std::thread([]{ gen_logo("logo_black_transparent.png", Color{0,0,0,255}, Color{}, 4096); }),
        std::thread([]{ gen_logo("logo_white_transparent.png", Color{255,255,255,255}, Color{}, 4096); }),
        std::thread([]{ gen_logo("logo_white.png", Color{255,255,255,255}, Color{0,0,0,255}, 4096); }),
        std::thread([]{ gen_logo("logo_black.png", Color{0,0,0,255}, Color{255,255,255,255}, 4096); }),
    };
    for(auto& t : threads) t.join();
}

