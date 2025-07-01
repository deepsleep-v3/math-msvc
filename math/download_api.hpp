#pragma once
#include <windows.h>
#include <urlmon.h>
#include <iostream>
#include <string>

#pragma comment(lib, "urlmon.lib")

// 回调函数，用于显示下载进度
HRESULT __stdcall DownloadCallback(LPCVOID pCaller, LPCWSTR szURL, LPCWSTR szFileName,
    DWORD dwReserved, LPVOID lpvContext) {
    std::wcout << L"Downloading: " << szURL << L" to " << szFileName << std::endl;
    return S_OK;
}

HRESULT DownloadFileSimplified(const std::wstring& url, const std::wstring& filePath) {
    HRESULT hr = URLDownloadToFile(NULL, url.c_str(), filePath.c_str(), NULL, NULL);
    return hr;
}