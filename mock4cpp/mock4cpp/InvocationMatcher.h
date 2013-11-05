#ifndef InvocationMatcher_h__
#define InvocationMatcher_h__

#include <functional>

namespace mock4cpp {

	template <typename... arglist>
	struct InvocationMatcher:public Destructable
	{
		virtual bool matches(const std::tuple<arglist...>& actualArgs) = 0;

		virtual bool matches(const arglist&... actualArgs) = 0;
	};
}
#endif // InvocationMatcher_h__


