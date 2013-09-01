#ifndef utils_h__
#define utils_h__

template <typename TARGET, typename SOURCE>
TARGET union_cast(SOURCE source)
{
	static_assert(sizeof(TARGET) == sizeof(SOURCE), "can't convert");
	union
	{
		SOURCE source;
		TARGET target;
	} u;
	u.source = source;
	return u.target;
}
#endif // utils_h__


