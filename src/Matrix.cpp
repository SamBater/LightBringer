#include <math/Matrix.h>
namespace YYLB
{
    int cross_product(const Matrix<int, 2, 1> &lhs, const Matrix<int, 2, 1> &rhs)
    {
        return lhs.x() * rhs.y() - lhs.y() * rhs.x();
    }
}