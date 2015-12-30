/**
 * @file test_state_machine.cpp
 *
 * @brief A unit test suite of StateMachine
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <iostream>
#include <kjsd/cunit.h>
#include <kjsd/cutil.h>
#include <kjsd/state_machine.hpp>

using namespace std;
using namespace kjsd;

namespace test_state_machine_ns
{
    enum Event
    {
        event0,
        event1,
        event2,
        event3
    };
        
    class State0 : public State<Event>,
                   public Singleton<State0>
    {
    public:
        const char* name() const { return "State0"; }
        State<Event>* parent() const { return 0; }
    private:
        friend class Singleton<State0>;
        State0() {}
        ~State0() {}
    };
    class State1 : public State<Event>,
                   public Singleton<State1>
    {
    public:
        const char* name() const { return "State1"; }
        State<Event>* parent() const { return 0; }
    private:
        friend class Singleton<State1>;
        State1() {}
        ~State1() {}
    };

    class Context : public StateMachine<Event>
    {
    public:
        Context() : i_(0)
        {
            change_state(State0::instance());
        }
        int i_;
        void entry() { ++i_; }
        void exit() { ++i_; }
        void action() { i_ = 100; }
        static bool true_() { return true; }
        static bool false_() { return true; }
    };
}

using namespace test_state_machine_ns;

static Context* ctxt_;

static void setUp()
{
    ctxt_ = new Context();
}

static void tearDown()
{
    State0::instance()->clear();
    State1::instance()->clear();
    delete ctxt_;
    ctxt_ = 0;
}

static const char* test_current()
{
    KJSD_CUNIT_ASSERT(ctxt_->current() == State0::instance());
    Context::state_type* st = ctxt_->current();
    st = State1::instance();
    KJSD_CUNIT_ASSERT(ctxt_->current() == State0::instance());
    return 0;
}

static const char* test_change_state()
{
    State0::instance()->transitions[event0] =
        Context::transition_type(Context::End::instance());

    ctxt_->fire(event0);
    KJSD_CUNIT_ASSERT(ctxt_->current() == Context::End::instance());
    return 0;
}

static const char* test_fire()
{
    struct lambda
    {
        static bool true_() { return true; }
        static bool false_() { return false; }
    };

    State0::instance()->transitions[event1] =
        Context::transition_type(
            0,
            new InstanceFunction<Context, void()>(ctxt_, &Context::action),
            new StaticFunction<bool()>(lambda::false_));

    ctxt_->fire(event1);
    KJSD_CUNIT_ASSERT(ctxt_->i_ == 0);

    State0::instance()->transitions[event2] =
        Context::transition_type(
            0,
            new InstanceFunction<Context, void()>(ctxt_, &Context::action),
            new StaticFunction<bool()>(lambda::true_));

    ctxt_->fire(event2);
    KJSD_CUNIT_ASSERT(ctxt_->i_ == 100);
    return 0;
}

static const char* test_active()
{
    State1::instance()->entry =
        new InstanceFunction<Context, void()>(ctxt_, &Context::entry);
    State1::instance()->exit =
        new InstanceFunction<Context, void()>(ctxt_, &Context::exit);

    State0::instance()->transitions[event1] =
        Context::transition_type(State1::instance());
    State1::instance()->transitions[event2] =
        Context::transition_type(Context::End::instance());

    ctxt_->fire(event1);
    ctxt_->fire(event2);
    KJSD_CUNIT_ASSERT(ctxt_->i_ == 2);
    KJSD_CUNIT_ASSERT(ctxt_->current() == Context::End::instance());
    return 0;
}

static const char* test_passive()
{
    State1::instance()->entry =
        new InstanceFunction<Context, void()>(ctxt_, &Context::entry);
    State1::instance()->exit =
        new InstanceFunction<Context, void()>(ctxt_, &Context::exit);

    State1::instance()->clear();

    State0::instance()->transitions[event3] =
        Context::transition_type(State1::instance());

    ctxt_->fire(event3);
    KJSD_CUNIT_ASSERT(ctxt_->i_ == 0);
    KJSD_CUNIT_ASSERT(ctxt_->current() == State1::instance());
    return 0;
}

const char* test_state_machine()
{
    const KJSD_CUNIT_Func f[] = {
        test_current,
        test_change_state,
        test_fire,
        test_active,
        test_passive
    };

    for (int i = 0; i < KJSD_LENGTH(f); i++)
    {
        setUp();
        KJSD_CUNIT_RUN(f[i]);
        tearDown();
    }
    return 0;
}
