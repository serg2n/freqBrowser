
#include "utfwstring.h"

//------------------------------------------
std::wstring utf8_to_wstring(std::string const& str)
{
  std::wstring_convert<std::conditional_t<
        sizeof(wchar_t) == 4,
        std::codecvt_utf8<wchar_t>,
        std::codecvt_utf8_utf16<wchar_t>>> converter;
  return converter.from_bytes(str);
}
std::string wstring_to_utf8(std::wstring const& str)
{
  std::wstring_convert<std::conditional_t<
        sizeof(wchar_t) == 4,
        std::codecvt_utf8<wchar_t>,
        std::codecvt_utf8_utf16<wchar_t>>> converter;
  return converter.to_bytes(str);
}
