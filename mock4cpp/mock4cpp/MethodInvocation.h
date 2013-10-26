#ifndef InvocationMatcher_h__
#define InvocationMatcher_h__
namespace mock4cpp {

	template <typename... arglist>
	struct InvocationMatcher:public Destructable
	{
		virtual bool matchesActual(const arglist&... args) = 0;

		virtual bool matchesExpected(const arglist&... args) = 0;
	};
}
#endif // InvocationMatcher_h__


