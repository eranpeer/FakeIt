#pragma once

namespace fakeit {

    constexpr size_t _FNV_prime = sizeof(size_t) == 4 ? 16777619ULL : 1099511628211ULL;
    constexpr size_t _FNV_offset_basis = sizeof(size_t) == 4 ? 2166136261ULL : 14695981039346656037ULL;

    constexpr size_t _constExprHashImpl(const char* str, size_t count) { 
        return count ? (_constExprHashImpl(str, count - 1) ^ str[count - 1]) * _FNV_prime : _FNV_offset_basis;
    }
    
    template<size_t N>
    constexpr size_t constExprHash(const char(&str)[N]) {
        return _constExprHashImpl(str, N);
    }

}