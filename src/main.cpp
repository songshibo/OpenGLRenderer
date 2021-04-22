#include <iostream>
#include <Eigen/Core>

typedef Eigen::Matrix<double, 4, 1> double4;

int main()
{
    double4 v4d(1.0f, 2.0f, 3.0f, 4.0f);
    std::cout << v4d << std::endl;
    return 0;
}