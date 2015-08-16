/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

namespace fakeit {

    typedef unsigned long DWORD;

    struct TypeDescriptor {
        TypeDescriptor() :
                ptrToVTable(0), spare(0) {
            // ptrToVTable should contain the pointer to the virtual table of the type type_info!!!
            int **tiVFTPtr = (int **) (&typeid(void));
            int *i = (int *) tiVFTPtr[0];
			char *type_info_vft_ptr = (char *) i;
            ptrToVTable = type_info_vft_ptr;
        }

		char *ptrToVTable;
        DWORD spare;
        char name[8];
    };

    struct PMD {

        /************************************************************************/
        /* member displacement.
        /* For a simple inheritance structure the member displacement is always 0.
        /* since since the first member is placed at 0.
        /* In the case of multiple inheritance, this value may have a positive value.
        /************************************************************************/
        int mdisp;

        int pdisp;  // vtable displacement
        int vdisp;  //displacement inside vtable

        PMD() :
                mdisp(0), pdisp(-1), vdisp(0) {
        }
    };

    struct RTTIBaseClassDescriptor {
        RTTIBaseClassDescriptor() :
                pTypeDescriptor(nullptr), numContainedBases(0), attributes(0) {
        }

        const std::type_info *pTypeDescriptor; //type descriptor of the class
        DWORD numContainedBases; //number of nested classes following in the Base Class Array
        struct PMD where;        //pointer-to-member displacement info
        DWORD attributes;        //flags, usually 0
    };

    template<typename C, typename... baseclasses>
    struct RTTIClassHierarchyDescriptor {
        RTTIClassHierarchyDescriptor() :
                signature(0),
                attributes(0),
                numBaseClasses(0),
                pBaseClassArray(nullptr) {
            pBaseClassArray = new RTTIBaseClassDescriptor *[1 + sizeof...(baseclasses)];
            addBaseClass < C, baseclasses...>();
        }

        ~RTTIClassHierarchyDescriptor() {
            for (int i = 0; i < 1 + sizeof...(baseclasses); i++) {
                RTTIBaseClassDescriptor *desc = pBaseClassArray[i];
                delete desc;
            }
            delete[] pBaseClassArray;
        }

        DWORD signature;      //always zero?
        DWORD attributes;     //bit 0 set = multiple inheritance, bit 1 set = virtual inheritance
        DWORD numBaseClasses; //number of classes in pBaseClassArray
        RTTIBaseClassDescriptor **pBaseClassArray;

        template<typename BaseType>
        void addBaseClass() {
            static_assert(std::is_base_of<BaseType, C>::value, "C must be a derived class of BaseType");
            RTTIBaseClassDescriptor *desc = new RTTIBaseClassDescriptor();
            desc->pTypeDescriptor = &typeid(BaseType);
            pBaseClassArray[numBaseClasses] = desc;
            for (unsigned int i = 0; i < numBaseClasses; i++) {
                pBaseClassArray[i]->numContainedBases++;
            }
            numBaseClasses++;
        }

        template<typename head, typename B1, typename... tail>
        void addBaseClass() {
            static_assert(std::is_base_of<B1, head>::value, "invalid inheritance list");
            addBaseClass<head>();
            addBaseClass<B1, tail...>();
        }

    };

	template<typename C, typename... baseclasses>
	struct RTTICompleteObjectLocator {
#ifdef _WIN64
		RTTICompleteObjectLocator(const std::type_info &unused) :
			signature(0), offset(0), cdOffset(0),
			typeDescriptorOffset(0), classDescriptorOffset(0)
		{
		}

		DWORD signature; //always zero ?
		DWORD offset;    //offset of this vtable in the complete class
		DWORD cdOffset;  //constructor displacement offset
		DWORD typeDescriptorOffset;
		DWORD classDescriptorOffset;
#else
		RTTICompleteObjectLocator(const std::type_info &info) :
			signature(0), offset(0), cdOffset(0),
			pTypeDescriptor(&info),
			pClassDescriptor(new RTTIClassHierarchyDescriptor<C, baseclasses...>()) {
		}

		~RTTICompleteObjectLocator() {
			delete pClassDescriptor;
		}

		DWORD signature; //always zero ?
		DWORD offset;    //offset of this vtable in the complete class
		DWORD cdOffset;  //constructor displacement offset
		const std::type_info *pTypeDescriptor; //TypeDescriptor of the complete class
		struct RTTIClassHierarchyDescriptor<C, baseclasses...> *pClassDescriptor; //describes inheritance hierarchy
#endif
	};


    struct VirtualTableBase {

        static VirtualTableBase &getVTable(void *instance) {
            fakeit::VirtualTableBase *vt = (fakeit::VirtualTableBase *) (instance);
            return *vt;
        }

        VirtualTableBase(void **firstMethod) : _firstMethod(firstMethod) { }

        void *getCookie(int index) {
            return _firstMethod[-2 - index];
        }

        void setCookie(int index, void *value) {
            _firstMethod[-2 - index] = value;
        }

        void *getMethod(unsigned int index) const {
            return _firstMethod[index];
        }

        void setMethod(unsigned int index, void *method) {
            _firstMethod[index] = method;
        }

    protected:
        void **_firstMethod;
    };

    template<class C, class... baseclasses>
    struct VirtualTable : public VirtualTableBase {

        class Handle {

            friend struct VirtualTable<C, baseclasses...>;

            void **firstMethod;

            Handle(void **method) : firstMethod(method) { }

        public:

            VirtualTable<C, baseclasses...> &restore() {
                VirtualTable<C, baseclasses...> *vt = (VirtualTable<C, baseclasses...> *) this;
                return *vt;
            }
        };

        static VirtualTable<C, baseclasses...> &getVTable(C &instance) {
            fakeit::VirtualTable<C, baseclasses...> *vt = (fakeit::VirtualTable<C, baseclasses...> *) (&instance);
            return *vt;
        }

        void copyFrom(VirtualTable<C, baseclasses...> &from) {
            unsigned int size = VTUtils::getVTSize<C>();
            for (unsigned int i = 0; i < size; i++) {
                _firstMethod[i] = from.getMethod(i);
            }
        }

        VirtualTable() : VirtualTable(buildVTArray()) {
        }

        ~VirtualTable() {

        }

        void dispose() {
            _firstMethod--; // skip objectLocator
            RTTICompleteObjectLocator<C, baseclasses...> *locator = (RTTICompleteObjectLocator<C, baseclasses...> *) _firstMethod[0];
            delete locator;
            _firstMethod -= numOfCookies; // skip cookies
            delete[] _firstMethod;
        }

        // the dtor VC++ must of the format: int dtor(int)
        unsigned int dtor(int) {
            C *c = (C *) this;
            C &cRef = *c;
            auto vt = VirtualTable<C, baseclasses...>::getVTable(cRef);
            void *dtorPtr = vt.getCookie(numOfCookies - 1); // read the last cookie
            void(*method)(C *) = reinterpret_cast<void (*)(C *)>(dtorPtr);
            method(c);
            return 0;
        }

        void setDtor(void *method) {
            // the dtor VC++ must of the format: int dtor(int).
            // the method passed by the user is: void dtor().
            // store the user method in a cookie and put the
            // correct format method in the virtual table.
            // the method stored in the vt will call the method in the cookie when invoked.
            void *dtorPtr = union_cast<void *>(&VirtualTable<C, baseclasses...>::dtor);
            unsigned int index = VTUtils::getDestructorOffset<C>();
            _firstMethod[index] = dtorPtr;
            setCookie(numOfCookies - 1, method); // use the last cookie
        }

        unsigned int getSize() {
            return VTUtils::getVTSize<C>();
        }

        void initAll(void *value) {
            auto size = getSize();
            for (unsigned int i = 0; i < size; i++) {
                setMethod(i, value);
            }
        }

        Handle createHandle() {
            Handle h(_firstMethod);
            return h;
        }

    private:

        class SimpleType {
        };

        static_assert(sizeof(unsigned int (SimpleType::*)()) == sizeof(unsigned int (C::*)()),
                      "Can't mock a type with multiple inheritance");
        static const unsigned int numOfCookies = 3;

        static void **buildVTArray() {
            int vtSize = VTUtils::getVTSize<C>();
            auto array = new void *[vtSize + numOfCookies + 1]{};
            RTTICompleteObjectLocator<C, baseclasses...> *objectLocator = new RTTICompleteObjectLocator<C, baseclasses...>(
                    typeid(C));
            array += numOfCookies; // skip cookies
            array[0] = objectLocator; // initialize RTTICompleteObjectLocator pointer
            array++; // skip object locator
            return array;
        }

        VirtualTable(void **firstMethod) : VirtualTableBase(firstMethod) {
        }
    };
}
