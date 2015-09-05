/**
 * @file command.hpp
 *
 * @version $Id:$
 *
 * @brief コマンド1，2，3
 *
 * Usage:
 * @code
 * class Foo() { public: void func(int i){ std::cout << i << std::endl; } };
 * class Bar() { public: void func(int i){ std::cout << i << std::endl; } };
 * Foo f;
 * Bar b;
 * InstanceFunction<Foo, void(int)> f1(&f, &Foo::func);
 * InstanceFunction<Bar, void(int)> f2(&b, &Bar::func);
 * Command<void(int)> com_f1(&f1, 1);
 * Command<void(int)> com_f2(&f2, 2);
 * com_f1(); // call Foo::func(1)
 * com_f2(); // call Bar::func(2)
 * @endcode
 *
 * @note コマンドとして定義できる関数の引数は3つまで
 *
 * @see Delegate
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 ***********************************************************************/
#ifndef KJSD_COMMAND_HPP
#define KJSD_COMMAND_HPP

#include <kjsd/delegate.hpp>

#define KJSD_COMMAND_FPAR_DEF0
#define KJSD_COMMAND_FPAR_DEF1 A0 a0_
#define KJSD_COMMAND_FPAR_DEF2 KJSD_COMMAND_FPAR_DEF1; A1 a1_
#define KJSD_COMMAND_FPAR_DEF3 KJSD_COMMAND_FPAR_DEF2; A2 a2_

#define KJSD_COMMAND_CPAR0
#define KJSD_COMMAND_CPAR1 const A0& a0,
#define KJSD_COMMAND_CPAR2 KJSD_COMMAND_CPAR1 const A1& a1,
#define KJSD_COMMAND_CPAR3 KJSD_COMMAND_CPAR2 const A2& a2,

#define KJSD_COMMAND_CPAR_BIND0
#define KJSD_COMMAND_CPAR_BIND1 , a0_(a0)
#define KJSD_COMMAND_CPAR_BIND2 KJSD_COMMAND_CPAR_BIND1, a1_(a1)
#define KJSD_COMMAND_CPAR_BIND3 KJSD_COMMAND_CPAR_BIND2, a2_(a2)

#define KJSD_COMMAND_FPAR0
#define KJSD_COMMAND_FPAR1 const A0& a0
#define KJSD_COMMAND_FPAR2 KJSD_COMMAND_FPAR1, const A1& a1
#define KJSD_COMMAND_FPAR3 KJSD_COMMAND_FPAR2, const A2& a2

#define KJSD_COMMAND_FPAR_ENTR0
#define KJSD_COMMAND_FPAR_ENTR1 a0_ = a0
#define KJSD_COMMAND_FPAR_ENTR2 KJSD_COMMAND_FPAR_ENTR1; a1_ = a1
#define KJSD_COMMAND_FPAR_ENTR3 KJSD_COMMAND_FPAR_ENTR2; a2_ = a2

#define KJSD_COMMAND_FARG0
#define KJSD_COMMAND_FARG1 a0_
#define KJSD_COMMAND_FARG2 KJSD_COMMAND_FARG1, a1_
#define KJSD_COMMAND_FARG3 KJSD_COMMAND_FARG2, a2_


namespace kjsd
{
    /**
     * @brief コマンド
     *
     * デリゲートに引数を束縛させたクラス
     */
    template<typename Sig> class Command;

    template<typename R>
    class Command<R()> : public Delegate<R()>
    {
    public:
        Command(Function<R()>* fp = 0) : Delegate<R()>(fp)
        {}
        virtual ~Command() {}

        Command<R()>& operator=(Function<R()>* fp)
        {
            Delegate<R()>::operator=(fp);
            return *this;
        }
        R operator()() const
        {
            return Delegate<R()>::operator()();
        }
    };

#define KJSD_COMMAND_DEF(num)                                           \
    template<KJSD_DELEGATE_TPAR##num>                                   \
    class Command<R(KJSD_DELEGATE_TARG##num)> :                         \
        public Delegate<R(KJSD_DELEGATE_TARG##num)>                     \
    {                                                                   \
    public:                                                             \
        Command(Function<R(KJSD_DELEGATE_TARG##num)>* fp = 0) :         \
            Delegate<R(KJSD_DELEGATE_TARG##num)>(fp)                    \
        {}                                                              \
        Command(KJSD_COMMAND_CPAR##num                                  \
                Function<R(KJSD_DELEGATE_TARG##num)>* fp = 0) :         \
            Delegate<R(KJSD_DELEGATE_TARG##num)>(fp)                    \
            KJSD_COMMAND_CPAR_BIND##num                                 \
        {}                                                              \
        virtual ~Command() {}                                           \
                                                                        \
        Command<R(KJSD_DELEGATE_TARG##num)>&                            \
            operator=(Function<R(KJSD_DELEGATE_TARG##num)>* fp)         \
            {                                                           \
                Delegate<R(KJSD_DELEGATE_TARG##num)>::operator=(fp);    \
                return *this;                                           \
            }                                                           \
        R operator()() const                                            \
        {                                                               \
            return Delegate<R(KJSD_DELEGATE_TARG##num)>::               \
                operator()(KJSD_COMMAND_FARG##num);                     \
        }                                                               \
        void bind(KJSD_COMMAND_FPAR##num)                               \
        {                                                               \
            KJSD_COMMAND_FPAR_ENTR##num;                                \
        }                                                               \
    private:                                                            \
        KJSD_COMMAND_FPAR_DEF##num;                                     \
    }

    KJSD_COMMAND_DEF(1);
    KJSD_COMMAND_DEF(2);
    KJSD_COMMAND_DEF(3);
}

#endif // KJSD_COMMAND_HPP
