#include <math/Matrix.h>
namespace YYLB
{
    Matrix4f translation_matrix4x4(const float tx, const float ty, const float tz)
    {
        Matrix4f t;
        t[0][3] = tx;
        t[1][3] = ty;
        t[2][3] = tz;
        return t;
    }

    Matrix4f translation_matrix4x4(const Vec4f &vec)
    {
        Matrix4f t;
        t[0][3] = vec.x();
        t[1][3] = vec.y();
        t[2][3] = vec.z();
        return t;
    }

    Matrix4f scale_matrix4x4(const float sx, const float sy, const float sz, const float sw = 1)
    {
        Matrix4f s;
        s[0][0] = sx;
        s[1][1] = sy;
        s[2][2] = sz;
        s[3][3] = sw;
        return s;
    }

    void translate(Matrix3f &t, const Vec3f &offset)
    {
        t[0][3] = offset.x();
        t[1][3] = offset.y();
        t[2][3] = offset.z();
    }

    void translate(Matrix4f &t, const Vec3f &offset)
    {
        t[0][3] = offset.x();
        t[1][3] = offset.y();
        t[2][3] = offset.z();
    }

    void set_identyti(Matrix4f &mat)
    {
        mat = M_identity_4x4;
    }

    void set_zero(Matrix4f &mat)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] = 0;
    }

    // void set_identyti(Matrix<T, R, C> &mat)
    // {
    //     for (int r = 0; r < R && r < C; r++)
    //     {
    //         mat[r][r] = 1;
    //     }
    // }
}