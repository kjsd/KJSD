/**
 * @file test_state_machine.cpp
 *
 * @version $Id: 1dfac53cdf1068d28f0ab483695c256624b91bf3 $
 *
 * @brief A unit test suite of StateMachine
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
/// このヘッダは必ずincludeする
#include <cppunit/extensions/HelperMacros.h>

#include <cassert>
#include <iostream>
#include <cstring>
#include <kjsd/state_machine.hpp>

using namespace std;
using namespace kjsd;

/**
 * @class TEST_StateMachine
 *
 * @brief StateMachineクラスのユニットテストスイート
 *
 * @see StateMachine
 */
// CppUnit::TestFixtureを継承させる
class TEST_StateMachine : public CppUnit::TestFixture
{
    //---> ここからフレームワークのお約束事

    /// テストスイート宣言開始マクロ．このクラス名を指定する．
    CPPUNIT_TEST_SUITE(TEST_StateMachine);

    /// 以下テストケース宣言マクロ．テスト実行時にはこの順番で呼ばれる．
    /// あるテストケースで別機能を使う必要がある場合が考えられるから，
    /// 単体で完結する機能からテストされる順番になるよう注意する．
    CPPUNIT_TEST(test_start);
    CPPUNIT_TEST(test_change_state);
    CPPUNIT_TEST(test_fire);
    CPPUNIT_TEST(test_active);
    CPPUNIT_TEST(test_passive);

    /// テストスイート宣言終了マクロ
    CPPUNIT_TEST_SUITE_END();

public:
    /**
     * @brief 各テストケース実行前に呼ばれるコールバック．
     * 通常テストケースに共通して必要なリソースを全て確保する．
     */
    void setUp();

    /**
     * @brief 各テストケース終了後に呼ばれるコールバック．
     * テストケースが互いに影響し合わないようにするため，setUp()で確保した
     * テスト用リソースを全て開放しておく．
     */
    void tearDown();

protected:
    //<--- ここまでフレームワークのお約束事．
    // あとはクラス宣言後にもうひとつ(CPPUNIT_TEST_SUITE_REGISTRATION)すれば
    // お約束事は終了．

    // 以下は実際のテストケース宣言．テスト対象によって大きく変わる部分．
    // 上のCPPUNIT_TEST()で登録したメソッドは全てここで宣言する．

    enum Event
    {
        event0,
        event1,
        event2,
        event3
    };
        
    class Context : public StateMachine<Event>
    {
    public:
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

    Context* ctxt_;
    void test_start();
    void test_change_state();
    void test_fire();
    void test_active();
    void test_passive();
};

/// このテストスイートをCppUnitへ登録
CPPUNIT_TEST_SUITE_REGISTRATION(TEST_StateMachine);

void TEST_StateMachine::setUp()
{
    cout << endl;

    ctxt_ = new Context();
}

void TEST_StateMachine::tearDown()
{
    Context::State0::instance()->clear();
    Context::State1::instance()->clear();
    delete ctxt_;
    ctxt_ = 0;
}

void TEST_StateMachine::test_start()
{
    CPPUNIT_ASSERT(ctxt_->current() == Context::State0::instance());
}

void TEST_StateMachine::test_change_state()
{
    Context::State0::instance()->transitions[event0] =
        Context::transition_type(Context::End::instance());

    ctxt_->fire(event0);
    CPPUNIT_ASSERT(ctxt_->current() == Context::End::instance());
}

void TEST_StateMachine::test_fire()
{
    struct lambda
    {
        static bool true_() { return true; }
        static bool false_() { return false; }
    };

    Context::State0::instance()->transitions[event1] =
        Context::transition_type(
            0,
            new InstanceFunction<Context, void()>(ctxt_, &Context::action),
            new StaticFunction<bool()>(lambda::false_));

    ctxt_->fire(event1);
    CPPUNIT_ASSERT(ctxt_->i_ == 0);

    Context::State0::instance()->transitions[event2] =
        Context::transition_type(
            0,
            new InstanceFunction<Context, void()>(ctxt_, &Context::action),
            new StaticFunction<bool()>(lambda::true_));

    ctxt_->fire(event2);
    CPPUNIT_ASSERT(ctxt_->i_ == 100);
}

void TEST_StateMachine::test_active()
{
    Context::State1::instance()->entry =
        new InstanceFunction<Context, void()>(ctxt_, &Context::entry);
    Context::State1::instance()->exit =
        new InstanceFunction<Context, void()>(ctxt_, &Context::exit);

    Context::State0::instance()->transitions[event1] =
        Context::transition_type(Context::State1::instance());
    Context::State1::instance()->transitions[event2] =
        Context::transition_type(Context::End::instance());

    ctxt_->fire(event1);
    ctxt_->fire(event2);
    CPPUNIT_ASSERT(ctxt_->i_ == 2);
    CPPUNIT_ASSERT(ctxt_->current() == Context::End::instance());
}

void TEST_StateMachine::test_passive()
{
    Context::State1::instance()->entry =
        new InstanceFunction<Context, void()>(ctxt_, &Context::entry);
    Context::State1::instance()->exit =
        new InstanceFunction<Context, void()>(ctxt_, &Context::exit);

    Context::State1::instance()->clear();

    Context::State0::instance()->transitions[event3] =
        Context::transition_type(Context::State1::instance());

    ctxt_->fire(event3);
    CPPUNIT_ASSERT(ctxt_->i_ == 0);
    CPPUNIT_ASSERT(ctxt_->current() == Context::State1::instance());
}
