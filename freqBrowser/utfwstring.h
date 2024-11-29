#ifndef UTFWSTRING_H
#define UTFWSTRING_H

#include <string>
#include <codecvt>

std::wstring utf8_to_wstring(std::string const& str);
std::string wstring_to_utf8(std::wstring const& str);

#endif // UTFWSTRING_H
