/**
 * @file request_broker.hpp
 *
 * @version $Id:$
 *
 * @brief
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#ifndef KJSD_REQUEST_BROKER_HPP
#define KJSD_REQUEST_BROKER_HPP

#error "Not implemented"

#include "delegate.h"

namespace kjsd
{
    template<typename T, typename A>
    class RequestBroker
    {
    public:
        typedef typename kjsd::Delegate<typename T::State(const A&)> Handler;

        typedef typename kjsd::
        InstanceFunction<T, typename T::State(const A&)>
        HandlerSkeleton;

        typedef typename T::State (T::*FunctionSkeleton)(const A&);

        RequestBroker(typename T::State initial_state) :
            state_(initial_state) {}
        virtual ~RequestBroker() {}

        typename T::State get_state() const
        {
            return state_;
        }

        virtual Handler get_handler(typename T::State state_) = 0;

        typename T::State exec(const A&);

    private:
        typename T::State state_;
    };

    template<typename T, typename A>
    typename T::State RequestBroker<T, A>::exec(const A& arg)
    {
        assert(state_ < T::get_num_of_state());

        Handler hdl = get_handler(state_);
        assert(hdl != 0);

        state_ = hdl(arg);

        return state_;
    }
}

#endif /* KJSD_REQUEST_BROKER_HPP */
