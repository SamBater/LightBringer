#ifndef YYLB_MATRIX
#define YYLB_MATRIX
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
        Matrix() = default;
        Matrix(std::initializer_list<T> list)
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
        inline T x() const { return data[0][0]; }
        inline T y() const { return data[0][1]; }
        const T *operator[](const int index) const { return data[index]; }
        T *operator[](const int index) { return data[index]; }
        Matrix<T, R, C> operator-(const Matrix<T, R, C> &rhs)
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
        Matrix<T, R, C> operator*(const float val)
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
        // Matrix<T, R, C> operator*(Matrix<T, R, 1> &Vec)
        // {
        //     Matrix<T, R, C> result;
        //     for (int r = 0; r < R; r++)
        //     {
        //         for (int c = 0; c < C; c++)
        //         {
        //             result[r][c] = 0;
        //             for (int vr = 0; vc < R; vc++)
        //             {
        //                 result[r][c] += data[r][c] * Vec[vc][0];
        //             }
        //         }
        //     }
        //     return result;
        // }
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
    };
    typedef Matrix<int, 2, 1> Vec2i;
    typedef Matrix<float, 2, 1> Vec2f;
    typedef Matrix<float, 3, 1> Vec3f;
    typedef Matrix<float, 4, 1> Vec4f;
    typedef Matrix<float, 4, 4> Matrix4f;
    typedef Matrix<float, 3, 3> Matrix3f;
    int cross_product(const Matrix<int, 2, 1> &lhs, const Matrix<int, 2, 1> &rhs);

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

    
}
#endif