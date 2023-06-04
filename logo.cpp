#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <vector>
#include "stb_image_write.h"

using u8 = unsigned char;
using f32 = float;

struct Color
{
    u8 r,g,b,a;
};

struct vec2;

vec2 operator + (vec2 l, vec2 r);
vec2 operator - (vec2 l, vec2 r);
vec2 operator * (vec2 l, vec2 r);
vec2 operator / (vec2 l, vec2 r);
struct vec2
{
    f32 x,y;
    vec2(f32 x, f32 y) : x(x), y(y) {}
    vec2(f32 s = 0) : x(s), y(s) {}

    f32 dot(vec2 r) const { r = r**this; return r.x+r.y; }
    f32 cross(vec2 r) const { return x * r.y - y * r.x; }
    f32 len() const { return sqrtf(dot(*this)); }
};

vec2 operator + (vec2 l, vec2 r){ return {l.x + r.x, l.y + r.y};}
vec2 operator - (vec2 l, vec2 r){ return {l.x - r.x, l.y - r.y};}
vec2 operator * (vec2 l, vec2 r){ return {l.x * r.x, l.y * r.y};}
vec2 operator / (vec2 l, vec2 r){ return {l.x / r.x, l.y / r.y};}

vec2 operator + (f32 l, vec2 r){ return vec2(l) + r; }
vec2 operator - (f32 l, vec2 r){ return vec2(l) - r; }
vec2 operator * (f32 l, vec2 r){ return vec2(l) * r; }
vec2 operator / (f32 l, vec2 r){ return vec2(l) / r; }

vec2 operator + (vec2 l, f32 r){ return l + vec2(r); }
vec2 operator - (vec2 l, f32 r){ return l - vec2(r); }
vec2 operator * (vec2 l, f32 r){ return l * vec2(r); }
vec2 operator / (vec2 l, f32 r){ return l / vec2(r); }


vec2 bary(vec2 p, vec2 p0, vec2 p1, vec2 p2)
{
    vec2 v0 = p1 - p0;
    vec2 v1 = p2 - p0;
    vec2 v2 = p - p0;
    return vec2(v2.cross(v1), v0.cross(v2)) / v0.cross(v1);
}

void gen_logo(const char* out, const Color fill, const Color empty, vec2 extent)
{
    std::vector<Color> img = {size_t(extent.y * extent.x), empty};

    const vec2 N = vec2(2,-2) / extent;
    const f32 sq3h = sqrt(3) *.5;
    const f32 outer = 236 / 256.f;
    const f32 inner = 216 / 256.f;

    const vec2 p0 = inner * vec2(0, 1);
    const vec2 p1 = inner * vec2(-sq3h, -.5);
    const vec2 p2 = inner * vec2(+sq3h, -.5);

    for(int j = 0; j < extent.y; ++j)
    {
        for(int i = 0; i < extent.x; ++i)
        {
            vec2 uv = vec2(i, j) * N - vec2(1, -1);
            vec2 coords = bary(uv, p0, p1, p2);
            f32 len = uv.len();
            if((coords.x > 0 && coords.x < 1 && coords.y > 0 && coords.y < 1 && coords.x + coords.y  < 1) || (len > inner && len < outer))
                img[j * extent.x + i]  = fill;
        }
    }
        
    stbi_write_png(out, extent.x, extent.y, 4, img.data(), extent.x * 4);
}

int main()
{
    gen_logo("logo_black_transparent.png", Color{0,0,0,255}, Color{}, 4096);
    gen_logo("logo_white_transparent.png", Color{255,255,255,255}, Color{}, 4096);
    gen_logo("logo_black.png", Color{0,0,0,255}, Color{255,255,255,255}, 4096);
    gen_logo("logo_white.png", Color{255,255,255,255}, Color{0,0,0,255}, 4096);
}

