#ifndef YYLB_MATRIX
#define YYLB_MATRIX
#include <cmath>
#include <vector>
#include <iostream>
namespace YYLB
{
    template <typename T, int R, int C>
    class Matrix
    {
    private:
    public:
        T data[R][C];
        Matrix()
        {
            //默认单位矩阵
            for (int r = 0; r < R && r >= C; r++)
                data[r][r] = 1;
        }
        Matrix(std::initializer_list<T> &&list) : Matrix()
        {
            auto c = list.begin();
            for (int i = 0; i < R; i++)
            {
                for (int j = 0; j < C && c != list.end(); j++, c++)
                {
                    data[i][j] = *c;
                }
            }
        }

        Matrix(const Matrix<T, R, C> &rhs)
        {
            for (int r = 0; r < R; r++)
            {
                for (int c = 0; c < C; c++)
                {
                    data[r][c] = rhs.data[r][c];
                }
            }
        }

        inline const T &x() const { return data[0][0]; }
        inline T &x() { return data[0][0]; }
        inline const T &y() const { return data[0][1]; }
        inline T &y() { return data[0][1]; }
        inline const T &z() const { return data[0][2]; }
        inline T &z() { return data[0][2]; }
        inline T &w() { return data[0][3]; }
        inline const T &w() const { return data[0][3]; }
        const T *operator[](const int index) const { return data[index]; }
        T *operator[](const int index) { return data[index]; }
        Matrix<T, R, C> operator+(const Matrix<T, R, C> &rhs) const
        {
            Matrix<T, R, C> result;
            for (int r = 0; r < R; r++)
            {
                for (int c = 0; c < C; c++)
                {
                    result[r][c] = data[r][c] + rhs.data[r][c];
                }
            }
            return result;
        }
        Matrix<T, R, C> operator-(const Matrix<T, R, C> &rhs) const
        {
            Matrix<T, R, C> result;
            for (int r = 0; r < R; r++)
            {
                for (int c = 0; c < C; c++)
                {
                    result[r][c] = data[r][c] - rhs.data[r][c];
                }
            }
            return result;
        }
        Matrix<T, R, C> operator*(float val) const
        {
            Matrix<T, R, C> result;
            for (int r = 0; r < R; r++)
            {
                for (int c = 0; c < C; c++)
                {
                    result[r][c] = data[r][c] * val;
                }
            }
            return result;
        }

        template <int S>
        Matrix<T, C, S> operator*(Matrix<T, C, S> &Vec) const
        {
            Matrix<T, C, S> result;
            for (int r = 0; r < C; r++)
            {

                for (int c = 0; c < S; c++)
                {
                    result[r][c] = 0;
                    for (int i = 0; i < C; i++)
                        result[r][c] += data[r][i] * Vec.data[i][c];
                }
            }
            return result;
        }

        template <int S>
        Matrix<T, C, S> &operator*=(Matrix<T, C, S> &Vec)
        {
            Matrix<T, C, S> result;
            for (int r = 0; r < C; r++)
            {

                for (int c = 0; c < S; c++)
                {
                    result[r][c] = 0;
                    for (int i = 0; i < C; i++)
                        result[r][c] += data[r][i] * Vec.data[i][c];
                }
            }
            *this = result;
            return *this;
        }

        Matrix<T, R, C> operator/(const float val)
        {
            Matrix<T, R, C> result;
            for (int r = 0; r < R; r++)
            {
                for (int c = 0; c < C; c++)
                {
                    result[r][c] = data[r][c] / val;
                }
            }
            return result;
        }

        Matrix<T, R, C> &operator/=(const float val)
        {
            for (int r = 0; r < R; r++)
            {
                for (int c = 0; c < C; c++)
                {
                    data[r][c] /= val;
                }
            }
            return *this;
        }

        double scalar() const
        {
            T scalar = 0;
            for (int r = 0; r < R; r++)
            {
                for (int c = 0; c < C; c++)
                {
                    scalar += data[r][c] * data[r][c];
                }
            }
            return scalar;
        }

        void normalized()
        {
            (*this) /= std::sqrt(this->scalar());
        }
    };
    typedef Matrix<int, 2, 1> Vec2i;
    typedef Matrix<float, 2, 1> Vec2f;
    typedef Matrix<float, 3, 1> Vec3f;
    typedef Matrix<float, 4, 1> Vec4f;
    typedef Matrix<unsigned char, 3, 1> RGB;
    typedef Matrix<unsigned char, 4, 1> RGBA;
    typedef Matrix<int, 4, 1> Vec4i;
    typedef Matrix<float, 4, 4> Matrix4f;
    typedef Matrix<float, 3, 3> Matrix3f;
    const Matrix<float, 3, 3> M_identity_3x3{1, 0, 0, 0, 1, 0, 0, 0, 1};
    const Matrix4f M_identity_4x4{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    template <int R, int C>
    const Matrix<float, R, C> scale_matrix(int scaleCoef);


    inline float dot_product(const Vec3f& lhs,const Vec3f& rhs)
    {
        float sum = 0;
        for(int i = 0 ; i < 3 ; i++)
            sum += lhs[0][i] * rhs[0][i];
        return sum;
    }

    inline float cross_product(const Vec2f &lhs, const Vec2f &rhs)
    {
        return lhs.x() * rhs.y() - lhs.y() * rhs.x();
    }

    inline Vec3f cross_product_3d(Vec3f &lhs, Vec3f &rhs)
    {
        return Vec3f{
            lhs.y() * rhs.z() - lhs.z() * rhs.y(),
            lhs.z() * rhs.x() - lhs.x() * rhs.z(),
            lhs.x() * rhs.y() - lhs.y() * rhs.x()};
    }

    template <typename T, int R, int C>
    std::ostream &operator<<(std::ostream &out, const Matrix<T, R, C> &matrix)
    {
        for (int r = 0; r < R; r++)
        {
            for (int c = 0; c < C; c++)
            {
                out << matrix[r][c] << " ";
            }
            out << "\n";
        }
        return out;
    }

    //平移
    Matrix4f translation_matrix4x4(const float tx, const float ty, const float tz);
    Matrix4f translation_matrix4x4(const Vec4f &vec);
    void translate(Matrix4f &mat, const Vec3f &offset);
    void translate(Matrix3f &mat, const Vec3f &offset);

    //缩放
    Matrix4f scale_matrix4x4(const float sx, const float sy, const float sz, const float sw);

    //旋转
    Matrix4f rotation_z_matrix4f(float theta);

    Matrix4f rotation_y_matrix4f(float theta);

    Matrix4f rotation_x_matrix4f(float theta);

    void set_identyti(Matrix4f &mat);

    void set_zero(Matrix4f &mat);
    

}
#endif