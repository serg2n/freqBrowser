#ifndef UTFWSTRING_H
#define UTFWSTRING_H

#include <string>
#include <codecvt>
#include <vector>
#include<sstream>

std::wstring utf8_to_wstring(std::string const& str);
std::string wstring_to_utf8(std::wstring const& str);
std::vector<std::wstring> Split(const std::wstring& str, wchar_t delimiter);

#endif // UTFWSTRING_H
