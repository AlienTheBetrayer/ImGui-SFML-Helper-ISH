// macros.hpp
//
// All the needed preprocessor macros for ISH.

#pragma once

// CPP20GRT = C++ version 20 or higher.
#if defined(_MSVC_LANG) && _MSVC_LANG > 201703L || __cplusplus >= 201703L
#define CPP20GRT
#endif // defined(_MSVC_LANG) && _MSVC_LANG > 201703L || __cplusplus >= 201703L