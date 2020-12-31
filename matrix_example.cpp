/**
 * @file matrix_example.cpp
 * @author Barak Shoshany (baraksh@gmail.com) (http://baraksh.com)
 * @version 0.1
 * @date 2020-11-30
 * @copyright Copyright (c) 2020
 *
 * @brief An example file demonstrating the use of the matrix class template.
 */

#include <iostream>
#include <vector>

#include "matrix.hpp"

using std::cout;
using std::vector;

int main()
{
    try
    {
        // Set the printing width of each element to 3 characters.
        matrix<double>::set_output_width(3);

        // (size_t, size_t) constructor: create a 3x4 UNINITIALIZED matrix.
        matrix<double> A(3, 4);
        cout << "matrix<double> A(3, 4): (UNINITIALIZED!)\n"
             << A;
        // (size_t, size_t, T) constructor: create a 4x5 matrix of zeros.
        matrix<double> B(4, 5, 0);
        cout << "matrix<double> B(4, 5, 0):\n"
             << B;
        // (vector<T>) constructor: create a 3x3 matrix with 1, 2, 3 on the diagonal.
        matrix<double> C(vector<double>{1, 2, 3});
        cout << "matrix<double> C(vector<double>{1, 2, 3}):\n"
             << C;
        // (initializer_list<T>) constructor: create a 4x4 matrix with 1, 2, 3, 4 on the diagonal.
        matrix<double> D{1, 2, 3, 4};
        cout << "matrix<double> D{1, 2, 3, 4}:\n"
             << D;
        // (size_t, size_t, vector<T>) constructor: create a 2x3 matrix with the given elements.
        matrix<double> E(2, 3, vector<double>{1, 2, 3, 4, 5, 6});
        cout << "matrix<double> E(2, 3, vector<double>{1, 2, 3, 4, 5, 6}):\n"
             << E;
        // (size_t, size_t, initializer_list<T>) constructor: create a 2x2 matrix with the given elements.
        matrix<double> F(2, 2, {1, 2, 3, 4});
        cout << "matrix<double> F(2, 2, {1, 2, 3, 4}):\n"
             << F;

        // Demonstration of some of the overloaded operators.
        E(0, 2) = 7;
        cout << "E after E(0, 2) = 7:\n"
             << E;
        matrix<double> G = E * C;
        cout << "G = E * C:\n"
             << G;
        cout << "E + G:\n"
             << E + G;
        cout << "7.0 * C:\n"
             << 7.0 * C;

        // initializer_list<T> constructor will be used: create a 3x3 diagonal matrix with 1, 2, 3 on the diagonal.
        cout << "matrix<double>{1, 2, 3}:\n";
        cout << matrix<double>{1, 2, 3};
        // (size_t, size_t, T) constructor will be used: create a 1x2 matrix with its elements initialized to 3.
        cout << "matrix<double>(1, 2, 3):\n";
        cout << matrix<double>(1, 2, 3);
    }
    catch (const matrix<double>::zero_size &e)
    {
        cout << "Error: Cannot create a matrix with zero rows or columns!\n";
    }
    catch (const matrix<double>::initializer_wrong_size &e)
    {
        cout << "Error: Initializer size does not match the expected number of elements!\n";
    }
    catch (const matrix<double>::incompatible_sizes_add &e)
    {
        cout << "Error: Two matrices can only be added or subtracted if they are of the same size!\n";
    }
    catch (const matrix<double>::incompatible_sizes_multiply &e)
    {
        cout << "Error: Two matrices can only be multiplied if the number of columns in the first matrix is equal to the number of rows in the second matrix!\n";
    }
    catch (const matrix<double>::index_out_of_range &e)
    {
        cout << "Error: Requested matrix element is out of range!\n";
    };
}
