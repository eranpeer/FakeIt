#ifndef ExtractMemberType_h__
#define ExtractMemberType_h__

#include <typeinfo>

template< typename T > struct ExtractMemberTypeHelper;
template< typename R, typename T >
struct ExtractMemberTypeHelper< R(T::*) >
{
	typedef R Type;
	typedef T ParentType;
};

template< typename T >
struct ExtractMemberType : public ExtractMemberTypeHelper< T > {};
#endif // ExtractMemberType_h__


