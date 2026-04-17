#include <vector>
// Better to assert all sizes

typedef short int i16;
typedef int i32;
typedef long long int i64;

typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

typedef float f32;
typedef double f64;
typedef long double f128;


constexpr double PI = 3.14159265358979323846;


struct Color {
    f32 r, g, b, a;
};

struct Box {
    u32 vao;
    std::vector<u32> bufferObjects;
};

class Matrix4f
{
public:
    f32 m[4][4];

    Matrix4f() {}
    Matrix4f(const Matrix4f& other) {
        for (i32 i = 0; i < 4; ++i) {
            for (i32 j = 0; j < 4; ++j) {
                m[i][j] = other.m[i][j];
            }
        }
    }

    Matrix4f(
        f32 m00, f32 m01, f32 m02, f32 m03,
        f32 m10, f32 m11, f32 m12, f32 m13,
        f32 m20, f32 m21, f32 m22, f32 m23,
        f32 m30, f32 m31, f32 m32, f32 m33
    ) 
    : m{
        {m00, m01, m02, m03}, 
        {m10, m11, m12, m13}, 
        {m20, m21, m22, m23}, 
        {m30, m31, m32, m33}
    } {}

    Matrix4f operator*(const Matrix4f& other) const {
        Matrix4f result;

        for (i32 i = 0; i < 4; ++i) {
            for (i32 j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;

                for (i32 k = 0; k < 4; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
};

f32 toRadian(f32 degrees) {
    return (f32) ((f64) degrees * PI / 180.0);
}