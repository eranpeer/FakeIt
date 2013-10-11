#ifndef MethodInvocationSchenario_h__
#define MethodInvocationSchenario_h__
namespace mock4cpp {

	template <typename... arglist>
	struct MethodInvocation:public Destructable
	{
		virtual ~MethodInvocation() = 0 {}

		virtual bool matchesActual(const arglist&... args) = 0;

		virtual bool matchesExpected(const arglist&... args) = 0;
	};
}
#endif // MethodInvocationSchenario_h__


