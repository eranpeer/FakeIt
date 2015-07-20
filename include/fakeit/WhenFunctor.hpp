/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */
#pragma once

#include "fakeit/StubbingImpl.hpp"
#include "fakeit/StubbingProgress.hpp"
#include "fakeit/FakeitContext.hpp"

#include "mockutils/smart_ptr.hpp"
#include "mockutils/Destructible.hpp"
#include "Xaction.hpp"

namespace fakeit {

    class WhenFunctor {

        struct StubbingChange {

            friend class WhenFunctor;

            virtual ~StubbingChange() THROWS {

                if (std::uncaught_exception()) {
                    return;
                }

                _xaction.commit();
            }

            StubbingChange(StubbingChange &other) :
                    _xaction(other._xaction) {
            }

        private:

            StubbingChange(Xaction &xaction)
                    : _xaction(xaction) {
            }

            Xaction &_xaction;
        };

    public:

        template<typename R, typename ... arglist>
        struct MethodProgress : MethodStubbingProgress<R, arglist...> {

            friend class WhenFunctor;

            virtual ~MethodProgress() override = default;

            MethodProgress(MethodProgress &other) :
                    _progress(other._progress), _context(other._context) {
            }

            MethodProgress(StubbingContext<R, arglist...> &xaction) :
                    _progress(new StubbingChange(xaction)), _context(xaction) {
            }

        protected:

            virtual MethodStubbingProgress<R, arglist...> &DoImpl(Action<R, arglist...> *action) override {
                _context.appendAction(action);
                return *this;
            }

        private:
            smart_ptr<StubbingChange> _progress;
            StubbingContext<R, arglist...> &_context;
        };


        WhenFunctor() {
        }

        template<typename R, typename ... arglist>
        MethodProgress<R, arglist...> operator()(const StubbingContext<R, arglist...> &stubbingContext) {
            StubbingContext<R, arglist...> &rootWithoutConst = const_cast<StubbingContext<R, arglist...> &>(stubbingContext);
            MethodProgress<R, arglist...> progress(rootWithoutConst);
            return progress;
        }

    };

}
