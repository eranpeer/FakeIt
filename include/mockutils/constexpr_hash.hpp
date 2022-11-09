#pragma once

namespace fakeit {

    template<size_t N>
    constexpr size_t constExprHash( const char (&str)[N] ) {
        size_t           _Val = 14695981039346656037ULL; // _FNV_offset_basis
        constexpr size_t _FNV_prime = 1099511628211ULL;

        const char* pStr(str);
        auto last = pStr + N - 1;
        for (; pStr != last; ++pStr)
        {
            _Val ^= static_cast<size_t>(*pStr);
            _Val *= _FNV_prime;
        }

        return _Val;
    }

}