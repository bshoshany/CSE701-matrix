#pragma once

/**
 * @file matrix.hpp
 * @author Barak Shoshany (baraksh@gmail.com) (http://baraksh.com)
 * @version 0.1
 * @date 2020-11-30
 * @copyright Copyright (c) 2020
 *
 * @brief A simple C++ matrix class template with dynamic memory allocation and overloaded operators for common matrix operations.
 *
 * @details This library contains a simple C++ class template for matrices. The matrices can be of arbitrary size. Memory is allocated dynamically on the heap using smart pointers. Overloaded operators for common matrix operations such as addition and multiplication are defined.
 */

#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

/**
 * @brief A class template for matrices.
 *
 * @tparam T The type to use for the matrix elements. Can be any type that has addition, subtraction, negation, and multiplication defined.
 */
template <typename T>
class matrix
{
public:
    // ============
    // Constructors
    // ============

    /**
     * @brief Constructor to create an UNINITIALIZED matrix. WARNING: Good for performance, but make sure to never use any uninitialized elements!
     *
     * @param input_rows The number of rows.
     * @param input_cols The number of columns.
     * @throws zero_size if the number of rows or columns is zero.
     */
    matrix(const size_t &input_rows, const size_t &input_cols)
        : rows(input_rows), cols(input_cols)
    {
        if (rows == 0 or cols == 0)
            throw zero_size{};
        smart.reset(new T[rows * cols]);
        elements = smart.get();
    }

    /**
     * @brief Constructor to create a matrix with all of its elements initialized to a specific value.
     *
     * @param input_rows The number of rows.
     * @param input_cols The number of columns.
     * @param input_init The value to initialize all of the elements to.
     * @throws zero_size if the number of rows or columns is zero.
     */
    matrix(const size_t &input_rows, const size_t &input_cols, const T &input_init)
        : rows(input_rows), cols(input_cols)
    {
        if (rows == 0 or cols == 0)
            throw zero_size{};
        smart.reset(new T[rows * cols]);
        elements = smart.get();
        for (size_t i{0}; i < rows * cols; i++)
            elements[i] = input_init;
    }

    /**
     * @brief Constructor to create a diagonal matrix from a `vector`.
     *
     * @param input_diagonal A `vector` containing the elements on the diagonal. The number of rows and columns is inferred automatically from the size of the `vector`.
     * @throws zero_size if the size of the `vector` is zero.
     */
    matrix(const std::vector<T> &input_diagonal)
        : rows(input_diagonal.size()), cols(input_diagonal.size())
    {
        if (rows == 0)
            throw zero_size{};
        smart.reset(new T[rows * cols]);
        elements = smart.get();
        for (size_t i{0}; i < rows; i++)
            for (size_t j{0}; j < cols; j++)
                elements[(cols * i) + j] = ((i == j) ? input_diagonal[i] : 0);
    }

    /**
     * @brief Constructor to create a diagonal matrix from an `initializer_list`.
     *
     * @param input_diagonal An `initializer_list` containing the elements on the diagonal. The number of rows and columns is inferred automatically from the size of the `initializer_list`.
     * @throws zero_size if the size of the `initializer_list` is zero.
     */
    matrix(const std::initializer_list<T> &input_diagonal)
        : matrix(std::vector<T>{input_diagonal}) {}

    /**
     * @brief Constructor to create a matrix and initialize it to the elements given by a `vector`.
     * @details The elements should be given in flattened 1-dimensional form, with the matrix element at row `i` and column `j` (counting from zero) given by element number `(cols * i) + j` of the `vector` (also counting from zero), where `cols` is the number of columns, that is, the number of element in each row. For example, for a 2x2 matrix A, the `vector` will be [A(0, 0), A(0, 1), A(1, 0), A(1, 1)].
     *
     * @param input_rows The number of rows.
     * @param input_cols The number of columns.
     * @param input_elements A `vector` containing the elements in flattened 1-dimensional form.
     * @throws zero_size if the number of rows or columns is zero.
     * @throws initializer_wrong_size if the size of the `vector` does not equal the total number of matrix elements.
     */
    matrix(const size_t &input_rows, const size_t &input_cols, const std::vector<T> &input_elements)
        : rows(input_rows), cols(input_cols)
    {
        if (rows == 0 or cols == 0)
            throw zero_size{};
        if (input_elements.size() != rows * cols)
            throw initializer_wrong_size{};
        smart.reset(new T[rows * cols]);
        elements = smart.get();
        for (size_t i{0}; i < rows * cols; i++)
            elements[i] = input_elements[i];
    }

    /**
     * @brief Constructor to create a matrix and initialize it to the elements given by an `initializer_list`.
     * @details The elements should be given in flattened 1-dimensional form, with the matrix element at row `i` and column `j` (counting from zero) given by element number `(cols * i) + j` of the `initializer_list` (also counting from zero), where `cols` is the number of columns, that is, the number of element in each row. For example, for a 2x2 matrix A, the `initializer_list` will be [A(0, 0), A(0, 1), A(1, 0), A(1, 1)].
     *
     * @param input_rows The number of rows.
     * @param input_cols The number of columns.
     * @param input_elements An `initializer_list` containing the elements in flattened 1-dimensional form.
     * @throws zero_size if the number of rows or columns is zero.
     * @throws initializer_wrong_size if the size of the `initializer_list` does not equal the total number of matrix elements.
     */
    matrix(const size_t &input_rows, const size_t &input_cols, const std::initializer_list<T> &input_elements)
        : matrix(input_rows, input_cols, std::vector<T>{input_elements}) {}

    /**
     * @brief Copy constructor to create a new matrix with the same elements as an existing matrix.
     *
     * @param m The matrix to be copied.
     */
    matrix(const matrix<T> &m)
        : rows(m.rows), cols(m.cols)
    {
        smart.reset(new T[rows * cols]);
        elements = smart.get();
        for (size_t i{0}; i < rows * cols; i++)
            elements[i] = m.elements[i];
    }

    /**
     * @brief Move constructor to move the elements of an existing matrix to a new matrix.
     *
     * @param m The matrix to be moved.
     */
    matrix(matrix<T> &&m)
        : rows(m.rows), cols(m.cols)
    {
        smart = move(m.smart);
        elements = smart.get();
        m.rows = 0;
        m.cols = 0;
        m.elements = nullptr;
    }

    // ================
    // Member functions
    // ================

    /**
     * @brief Overloaded operator = to copy the elements of one matrix to another matrix.
     *
     * @param m The matrix to be copied.
     * @return matrix<T>& A reference to the target matrix.
     */
    matrix<T> &operator=(const matrix<T> &m)
    {
        rows = m.rows;
        cols = m.cols;
        smart.reset(new T[rows * cols]);
        elements = smart.get();
        for (size_t i{0}; i < rows * cols; i++)
            elements[i] = m.elements[i];
        return *this;
    }

    /**
     * @brief Overloaded operator = to move the elements of one matrix to another matrix.
     *
     * @param m The matrix to be moved.
     * @return matrix<T>& A reference to the target matrix.
     */
    matrix<T> &operator=(matrix<T> &&m)
    {
        rows = m.rows;
        cols = m.cols;
        smart = move(m.smart);
        elements = smart.get();
        m.rows = 0;
        m.cols = 0;
        m.elements = nullptr;
        return *this;
    }

    /**
     * @brief Member function used to obtain (but not modify) the number of rows in the matrix.
     *
     * @return The number of rows.
     */
    inline size_t get_rows() const
    {
        return rows;
    }

    /**
     * @brief Member function used to obtain (but not modify) the number of columns in the matrix
     *
     * @return The number of columns.
     */
    inline size_t get_cols() const
    {
        return cols;
    }

    /**
     * @brief Overloaded operator () used to access matrix elements WITHOUT range checking.
     *
     * @param row The row index (starting from zero).
     * @param col The column index (starting from zero).
     * @return A reference to the element, which can also be used to modify it.
     */
    inline T &operator()(const size_t &row, const size_t &col)
    {
        return elements[(cols * row) + col];
    }

    /**
     * @brief Overloaded operator () used to access matrix elements WITHOUT range checking.
     *
     * @param row The row index (starting from zero).
     * @param col The column index (starting from zero).
     * @return The value of the element.
     */
    inline T operator()(const size_t &row, const size_t &col) const
    {
        return elements[(cols * row) + col];
    }

    /**
     * @brief Member function used to access matrix elements WITH range checking.
     *
     * @param row The row index (starting from zero).
     * @param col The column index (starting from zero).
     * @return A reference to the element, which can also be used to modify it.
     * @throws index_out_of_range if the requested element is out of range.
     */
    inline T &at(const size_t &row, const size_t &col)
    {
        if (row >= rows or col >= cols)
            throw index_out_of_range{};
        return elements[(cols * row) + col];
    }

    /**
     * @brief Member function used to access matrix elements WITH range checking.
     *
     * @param row The row index (starting from zero).
     * @param col The column index (starting from zero).
     * @return The value of the element.
     * @throws index_out_of_range if the requested element is out of range.
     */
    inline T at(const size_t &row, const size_t &col) const
    {
        if (row >= rows or col >= cols)
            throw index_out_of_range{};
        return elements[(cols * row) + col];
    }

    /**
     * @brief Static member function used to set the character width of the matrix elements when printing a matrix to a stream.
     *
     * @param w The new character width of the matrix elements. Will be used in operator<<() by inserting `std::setw` into the output stream.
     */
    inline static void set_output_width(const int &w)
    {
        output_width = w;
    }

    // ================
    // Friend functions
    // ================

    /**
     * @brief Overloaded binary operator `<<` used to easily print out a matrix to a stream.
     *
     * @param out The output stream.
     * @param m The matrix to be printed.
     * @return A reference to the output stream.
     */
    friend std::ostream &operator<<(std::ostream &out, const matrix<T> &m)
    {
        if (m.rows == 0 and m.cols == 0)
            out << "()\n";
        else
        {
            for (size_t i{0}; i < m.rows; i++)
            {
                out << "( ";
                for (size_t j{0}; j < m.cols; j++)
                    out << std::setw(m.output_width) << m(i, j) << ' ';
                out << ")\n";
            }
            out << '\n';
        }
        return out;
    }

    /**
     * @brief Overloaded binary operator `+` used to add two matrices.
     *
     * @param a The first matrix to be added.
     * @param b The second matrix to be added.
     * @return The sum of the matrices.
     * @throws incompatible_sizes_add if the matrices do not have the same number of rows and columns.
     */
    friend matrix<T> operator+(const matrix<T> &a, const matrix<T> &b)
    {
        if ((a.rows != b.rows) or (a.cols != b.cols))
            throw typename matrix<T>::incompatible_sizes_add{};
        matrix<T> c(a.rows, a.cols);
        for (size_t i{0}; i < a.rows; i++)
            for (size_t j{0}; j < a.cols; j++)
                c(i, j) = a(i, j) + b(i, j);
        return c;
    }

    /**
     * @brief Overloaded binary operator `+=` used to add two matrices and assign the result to the first one.
     *
     * @param a The first matrix to be added. Will be replaced with the sum of the matrices.
     * @param b The second matrix to be added.
     * @return The sum of the matrices.
     * @throws incompatible_sizes_add if the matrices do not have the same number of rows and columns.
     */
    inline friend matrix<T> operator+=(matrix<T> &a, const matrix<T> &b)
    {
        a = a + b;
        return a;
    }

    /**
     * @brief Overloaded unary operator `-` used to take the negative of a matrix.
     *
     * @param m The matrix to be negated.
     * @return The negative of the matrix.
     */
    friend matrix<T> operator-(const matrix<T> &m)
    {
        matrix<T> c(m.rows, m.cols);
        for (size_t i{0}; i < m.rows; i++)
            for (size_t j{0}; j < m.cols; j++)
                c(i, j) = -m(i, j);
        return c;
    }

    /**
     * @brief Overloaded binary operator `-` used to subtract two matrices.
     *
     * @param a The first matrix to be subtracted.
     * @param b The second matrix to be subtracted.
     * @return The first matrix minus the second matrix.
     * @throws incompatible_sizes_add if the matrices do not have the same number of rows and columns.
     */
    friend matrix<T> operator-(const matrix<T> &a, const matrix<T> &b)
    {
        if ((a.rows != b.rows) or (a.cols != b.cols))
            throw typename matrix<T>::incompatible_sizes_add{};
        matrix<T> c(a.rows, a.cols);
        for (size_t i{0}; i < a.rows; i++)
            for (size_t j{0}; j < a.cols; j++)
                c(i, j) = a(i, j) - b(i, j);
        return c;
    }

    /**
     * @brief Overloaded binary operator `-=` used to subtract two matrices and assign the result to the first one.
     *
     * @param a The first matrix to be subtracted. Will be replaced with first matrix minus the second matrix.
     * @param b The second matrix to be subtracted.
     * @return The first matrix minus the second matrix.
     * @throws incompatible_sizes_add if the matrices do not have the same number of rows and columns.
     */
    inline friend matrix<T> operator-=(matrix<T> &a, const matrix<T> &b)
    {
        a = a - b;
        return a;
    }

    /**
     * @brief Overloaded binary operator `*` used to multiply two matrices.
     *
     * @param a The first matrix to be multiplied.
     * @param b The second matrix to be multiplied.
     * @return The product of the matrices.
     * @throws incompatible_sizes_multiply if the number of columns in the first matrix is not the same as the number of rows in the second matrix.
     */
    friend matrix<T> operator*(const matrix<T> &a, const matrix<T> &b)
    {
        if (a.cols != b.rows)
            throw typename matrix<T>::incompatible_sizes_multiply{};
        matrix<T> c(a.rows, b.cols);
        for (size_t i{0}; i < a.rows; i++)
            for (size_t j{0}; j < b.cols; j++)
            {
                c(i, j) = 0;
                for (size_t k{0}; k < a.cols; k++)
                    c(i, j) += a(i, k) * b(k, j);
            }
        return c;
    }

    /**
     * @brief Overloaded binary operator `*` used to multiply a scalar on the left and a matrix on the right.
     *
     * @param s The scalar.
     * @param m The matrix.
     * @return The product of the scalar with the matrix.
     */
    friend matrix<T> operator*(const T &s, const matrix<T> &m)
    {
        matrix<T> c(m.rows, m.cols);
        for (size_t i{0}; i < m.rows; i++)
            for (size_t j{0}; j < m.cols; j++)
                c(i, j) = s * m(i, j);
        return c;
    }

    /**
     * @brief Overloaded binary operator `*` used to multiply a matrix on the left and a scalar on the right.
     *
     * @param m The matrix.
     * @param s The scalar.
     * @return The product of the scalar with the matrix.
     */
    inline friend matrix<T> operator*(const matrix<T> &m, const T &s)
    {
        return s * m;
    }

    // ==========
    // Exceptions
    // ==========

    /**
     * @brief Exception to be thrown if the number of rows or columns given to the constructor is zero.
     */
    class zero_size
    {
    };

    /**
     * @brief Exception to be thrown if the size of the `vector` or `initializer_list` provided to the constructor does not equal the total number of matrix elements.
     */
    class initializer_wrong_size
    {
    };

    /**
     * @brief Exception to be thrown if two matrices that are added or subtracted do not have the same number of rows and columns.
     */
    class incompatible_sizes_add
    {
    };

    /**
     * @brief Exception to be thrown when multiplying two matrices if the number of columns in the first matrix is not the same as the number of rows in the second matrix.
     */
    class incompatible_sizes_multiply
    {
    };

    /**
     * @brief Exception to be thrown if the requested matrix element is out of range.
     */
    class index_out_of_range
    {
    };

private:
    /**
     * @brief The number of rows.
     */
    size_t rows{0};

    /**
     * @brief The number of columns.
     */
    size_t cols{0};

    /**
     * @brief A pointer to an array storing the elements of the matrix in flattened 1-dimensional form.
     */
    T *elements{nullptr};

    /**
     * @brief A smart pointer to manage the memory allocated for the matrix elements.
     */
    std::unique_ptr<T[]> smart{nullptr};

    /**
     * @brief The character width of the matrix elements. Will be used in operator<<() by inserting std::setw into the output stream.
     */
    static int output_width;
};

// Initialize output_width to have a default value of 5
template <typename T>
int matrix<T>::output_width{5};
