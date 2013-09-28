#ifndef MockRepository_h__
#define MockRepository_h__

#include "ActualInvocationSchenario.h"

namespace mock4cpp {

	struct MockBase;

	struct MockRepository {

		static MockRepository & getDefaultRepository(){
			static MockRepository defaultRepository;
			return defaultRepository;
		}

		ActualInvocationSchenario & getInvovationSchenario(){
			return invovationSchenario;
		}

	private: 
		ActualInvocationSchenario invovationSchenario;
	};

	struct MockBase {
		MockBase(MockRepository & repository) :_repository{ repository }{}
		
		MockRepository & repository(){
			return _repository;
		}
	private:
		MockRepository & _repository;
	};
}

#endif // MockRepository_h__