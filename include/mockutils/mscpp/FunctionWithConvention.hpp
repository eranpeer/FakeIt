/// This file contains logic for dealing with the varying calling conventions of MSVC.
#pragma once

namespace fakeit {

    // Taking the calling convention from a type parameter is not valid syntax,
    // so we have to represent the calling conventions with marker types
    // and specialize the relevant bits of codes for them.

    // Marker type for __stdcall functions.
    struct Stdcall {};

    // Marker type for __thiscall functions.
    struct Thiscall {};

    // Marker type for __cdecl functions.
    struct Cdecl {};

    // Base case for a function pointer wrapper with the calling convention included.
    template<typename C, typename R, typename CONVENTION, typename ... arglist>
    class FuncWithConvention
    {};

    // Wrapper for a function pointer to a __thiscall function.
    template<typename C, typename R, typename ... arglist>
    class FuncWithConvention<C, R, Thiscall, arglist...>
    {
    public:
            R (__thiscall C::*_vMethod)(arglist...);

            FuncWithConvention( R (__thiscall C::*vMethod)(arglist...) ): _vMethod(vMethod)
            {
            };
    };

    // Wrapper for a function pointer to a __stdcall function.
    template<typename C, typename R, typename ... arglist>
    class FuncWithConvention<C, R, Stdcall, arglist...>
    {
    public:
            R (__stdcall C::*_vMethod)(arglist...);

            FuncWithConvention( R (__stdcall C::*vMethod)(arglist...) ): _vMethod(vMethod)
            {
            };
    };

    // Wrapper for a function pointer to a __cdecl function.
    template<typename C, typename R, typename ... arglist>
    class FuncWithConvention<C, R, Cdecl, arglist...>
    {
    public:
            R (__cdecl C::*_vMethod)(arglist...);

            FuncWithConvention( R (__cdecl C::*vMethod)(arglist...) ): _vMethod(vMethod)
            {
            };
    };

    class ConventionHelper
    {
    public:

// __stdcall and __thiscall are the same in x64, so we get errors if we try to overload them there.
#ifndef _WIN64
            // GetConvention overload that recognizes __stdcall and maps it to the Stdcall marker.
            template<typename C, typename R, typename ... arglist>
            static Stdcall GetConvention( R (__stdcall C::*vMethod)(arglist...) )
            {
            }

            // GetConvention overload that recognizes __stdcall and maps it to the Thiscall marker.
            template<typename C, typename R, typename ... arglist>
            static Thiscall GetConvention( R (__thiscall C::*vMethod)(arglist...) )
            {
            }
#endif
            // GetConvetion overload that recognizes __cdecl and maps it to the Cdecl marker.
            template<typename C, typename R, typename ... arglist>
            static Cdecl GetConvention( R (__cdecl C::*vMethod)(arglist...) )
            {
            }

            // Member function with no calling convention defined, used to determine what is the default.
            void MemberFunction(){};

            // The marker type for the caling convention used for a member when no convention is explicitly defined.
            typedef decltype(GetConvention(&MemberFunction)) DefaultConvention;

            // Helper function to use type-deduction for constructing the appropriate wrapper type.
            // This case is for __cdecl.
            template<typename C, typename R, typename ... arglist>
            static FuncWithConvention<C, R, Cdecl, arglist...> Wrap(R (__cdecl C::*vMethod)(arglist...))
            {
                    return FuncWithConvention<C, R, Cdecl, arglist...>( vMethod );
            };

// __stdcall and __thiscall are the same in x64, so we get errors if we try to overload them there.
#ifndef _WIN64

            template<typename C, typename R, typename ... arglist>
            static FuncWithConvention<C, R, Thiscall, arglist...> Wrap(R (__thiscall C::*vMethod)(arglist...))
            {
                    return FuncWithConvention<C, R, Thiscall, arglist...>( vMethod );
            };

            // Helper function to use type-deduction for constructing the appropriate wrapper type.
            // This case is for the __stdcall convention.
            template<typename C, typename R, typename ... arglist>
            static FuncWithConvention<C, R, Stdcall, arglist...> Wrap(R (__stdcall C::*vMethod)(arglist...))
            {
                    return FuncWithConvention<C, R, Stdcall, arglist...>( vMethod );
            };
#endif

    };
}
