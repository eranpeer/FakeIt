#ifndef MethodProxy_h__
#define MethodProxy_h__

namespace fakeit {

template<typename R, typename ... arglist>
struct MethodProxy {
	virtual unsigned int getOffset() = 0;
	virtual void * getProxy() = 0;
};

}
#endif // MethodProxy_h__
