/*
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 *
 * Created on Mar 10, 2014
 */

#pragma once

#include <vector>
#include "fakeit/EventHandler.hpp"
#include "fakeit/EventFormatter.hpp"

namespace fakeit {

    struct FakeitContext : public EventHandler, protected EventFormatter {

        virtual ~FakeitContext() = default;

        void handle(const UnexpectedMethodCallEvent &e) override {
            fireEvent(e);
            auto &eh = getTestingFrameworkAdapter();
            eh.handle(e);
        }

        void handle(const SequenceVerificationEvent &e) override {
            fireEvent(e);
            auto &eh = getTestingFrameworkAdapter();
            return eh.handle(e);
        }

        void handle(const NoMoreInvocationsVerificationEvent &e) override {
            fireEvent(e);
            auto &eh = getTestingFrameworkAdapter();
            return eh.handle(e);
        }

        std::string format(const UnexpectedMethodCallEvent &e) override {
            auto &eventFormatter = getEventFormatter();
            return eventFormatter.format(e);
        }

        std::string format(const SequenceVerificationEvent &e) override {
            auto &eventFormatter = getEventFormatter();
            return eventFormatter.format(e);
        }

        std::string format(const NoMoreInvocationsVerificationEvent &e) override {
            auto &eventFormatter = getEventFormatter();
            return eventFormatter.format(e);
        }

        void addEventHandler(EventHandler &eventListener) {
            _eventListeners.push_back(&eventListener);
        }

        void clearEventHandlers() {
            _eventListeners.clear();
        }

    protected:
        virtual EventHandler &getTestingFrameworkAdapter() = 0;

        virtual EventFormatter &getEventFormatter() = 0;

    private:
        std::vector<EventHandler *> _eventListeners;

        void fireEvent(const NoMoreInvocationsVerificationEvent &evt) {
            for (auto listener : _eventListeners)
                listener->handle(evt);
        }

        void fireEvent(const UnexpectedMethodCallEvent &evt) {
            for (auto listener : _eventListeners)
                listener->handle(evt);
        }

        void fireEvent(const SequenceVerificationEvent &evt) {
            for (auto listener : _eventListeners)
                listener->handle(evt);
        }

    };

}
