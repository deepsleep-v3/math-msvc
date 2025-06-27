#pragma once

#ifndef DEEPSLEEPV3_ADVANCED_MATH_HEADER
#define DEEPSLEEPV3_ADVANCED_MATH_HEADER
#define DLLEXPORT __declspec(dllexport)
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <vector>
using namespace std;

namespace math
{
    // 進階數學函式庫
    // 高等數學函式
    // 包含矩陣乘法、矩陣快速冪、斐波那契數列等

    typedef vector<vector<int64_t>> Matrix;

    DLLEXPORT Matrix matrixMultiply(const Matrix& a, const Matrix& b) {
        int n = a.size();
        int m = b[0].size();
        int k = a[0].size();
        Matrix res(n, vector<long long>(m, 0));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                for (int p = 0; p < k; ++p) {
                    res[i][j] += a[i][p] * b[p][j];
                }
            }
        }
        return res;
    }

    DLLEXPORT Matrix matrixPower(Matrix mat, int64_t power) {
        int n = mat.size();
        Matrix result(n, vector<long long>(n, 0));
        // 初始化为单位矩阵
        for (int i = 0; i < n; ++i) {
            result[i][i] = 1;
        }

        while (power > 0) {
            if (power % 2 == 1) {
                result = matrixMultiply(result, mat);
            }
            mat = matrixMultiply(mat, mat);
            power /= 2;
        }
        return result;
    }

    // 使用矩陣快速冪計算斐波那契數列，O(log n)
    DLLEXPORT uint64_t fib_matrix(uint64_t n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        Matrix mat = { {1, 1}, {1, 0} };
        Matrix result = matrixPower(mat, n - 1);
        return result[0][0];
    }

    // 斐波那契數列，时间复杂度：O(N)
    DLLEXPORT uint64_t fib_fast(uint64_t n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        if (n >= 1e6) return fib_matrix(n);
        uint64_t a = 0, b = 1;
        for (uint64_t i = 0; i < n; ++i) {
            uint64_t temp = b;
            b = a + b;
            a = temp;
        }
        return a;
    }

    // 斐波那契數列，时间复杂度：O(2ⁿ)
    DLLEXPORT uint64_t fib(uint64_t n) { // 負數不在斐波那契數列中
        if (n > 40) return fib_fast(n); // 超過40項使用快速算法，避免太慢
        if (n <= 1) return n;
        return fib(n - 1) + fib(n - 2);
    }
}
#endif