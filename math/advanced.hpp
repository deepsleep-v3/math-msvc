#pragma once

#ifndef DEEPSLEEPV3_ADVANCED_MATH_HEADER
#define DEEPSLEEPV3_ADVANCED_MATH_HEADER
#define DLLEXPORT __declspec(dllexport)
#define CURRENT_VERSION "0.2.0.0"
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <vector>
#include "packages/pugixml.1.13.0/build/native/include/pugixml.hpp"
#include "download_api.hpp"
using namespace std;
using namespace pugi;

/** 
* @brief 進階數學函式庫
* 
* 高等數學函式
* 包含矩陣乘法、矩陣快速冪、斐波那契數列等
* @author deepsleep-v3
*/
namespace math
{
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

    /**
    * @brief 使用矩陣快速冪計算斐波那契數列，O(log n)
    * @param x：uint64_t型別。表示斐波那契數列的索引。
    * @return uint64_t，斐波那契數列的第n項。
    * @author deepsleep-v3
    */
    DLLEXPORT uint64_t fib_matrix(uint64_t n) {
        if (n == 0) return 0;
        if (n == 1) return 1;
        Matrix mat = { {1, 1}, {1, 0} };
        Matrix result = matrixPower(mat, n - 1);
        return result[0][0];
    }

    /**
    * @brief 斐波那契數列，时间复杂度：O(N)
    * @param x：uint64_t型別。表示斐波那契數列的索引。
    * @return uint64_t，斐波那契數列的第n項。  
    * 
    * 你也可以选択使用fib_matrix(n)，他更快一点。
    * @see fib_matrix
    * @author deepsleep-v3
    */
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

    /**
    * @brief 斐波那契數列，时间复杂度：O(2^n)
    * @param x：uint64_t型別。表示斐波那契數列的索引。
    * @return uint64_t，斐波那契數列的第n項。  
    * 
    * 你也可以选択使用fib_fast(n)，他更快一点。
    * @see fib_fast
    * @author deepsleep-v3
    */
    DLLEXPORT uint64_t fib(uint64_t n) { // 負數不在斐波那契數列中      
        if (n > 40) return fib_fast(n); // 超過40項使用快速算法，避免太慢
        if (n <= 1) return n;
        return fib(n - 1) + fib(n - 2);
    }

    namespace update_tools {
        class StatusOfCheckUpdate
        {
        public:
            bool m_failed = false;
            string m_description = "";
            CheckUpdateResultType m_type = CheckUpdateResultType::CURRENT_IS_NEWEST;

            StatusOfCheckUpdate(bool failed, string description, CheckUpdateResultType type) {
                m_failed = failed;
                m_description = description;
                m_type = type;
            }

            ~StatusOfCheckUpdate() {}

        private:

        };

        enum class CheckUpdateResultType : uint8_t
        {
            UNKNOWN,
            CURRENT_IS_NEWEST,
            HAVE_NEW_VERSION,
            LIFE_CYCLE_XML_DOWNLOAD_FAILED,
            LIFE_CYCLE_XML_LOADING_FAILED,
        };

        /**
        * @brief 檢查是否為最新版本。
        * @return 當前版本是否為最新版本。
        * @author deepsleep-v3
        */
        DLLEXPORT StatusOfCheckUpdate check_update() {
            string _ = "";
            cout << "Checking update…" << endl;
            wchar_t buffer[MAX_PATH];
            DWORD length = GetTempPathW(MAX_PATH, buffer);
            wstring tempPath = wstring(buffer, length);
            wstring actualDownloadPath = tempPath + L"deepsleep-v3\\math-msvc-v142\\lifeCycleSheet.xml";
            
            HRESULT successed = DownloadFileSimplified(L"https://github.com/deepsleep-v3/math-msvc/raw/refs/heads/main/docs/lifeCycleSheet.xml", actualDownloadPath);
            if (!SUCCEEDED(successed)) {
                cerr << "Download failed. if continue, then maybe lead to XML loading fail. so, if you press \"Enter\", we will stop \"Check update\". " << endl;
                cin >> _;
                return StatusOfCheckUpdate(true, "Download failed. ", CheckUpdateResultType::LIFE_CYCLE_XML_DOWNLOAD_FAILED);
            }
            xml_document lifeCycleSheet;
            xml_parse_result result = lifeCycleSheet.load_file(actualDownloadPath.c_str());
            if (!result){
                cerr << "Life Cycle XML loading fail: " << result.description();
                return StatusOfCheckUpdate(true, result.description(), CheckUpdateResultType::LIFE_CYCLE_XML_LOADING_FAILED);
            }
            pugi::xml_node root = lifeCycleSheet.child("versions");
            pugi::xml_node latestVersionNode = root.child("latest-version");
            string latest = latestVersionNode.value();
            if (latest == CURRENT_VERSION) {
                return StatusOfCheckUpdate(false, "Current is newest version! ", CheckUpdateResultType::CURRENT_IS_NEWEST);
            }
            else {
                return StatusOfCheckUpdate(false, "Have a new version! ", CheckUpdateResultType::HAVE_NEW_VERSION);
            }
            return StatusOfCheckUpdate(false, "???", CheckUpdateResultType::UNKNOWN);
        }
    }
}
#endif