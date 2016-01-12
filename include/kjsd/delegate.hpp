/**
 * @file delegate.hpp
 *
 * @version $Id:$
 *
 * @brief デリゲート1，2，3
 *
 * Usage:
 * @code
 * class Foo() { public: void func(int i){ std::cout << i << std::endl; } };
 * class Bar() { public: void func(int i){ std::cout << i << std::endl; } };
 * Delegate<void(int)> funcp;
 * Foo f;
 * Bar b;
 * funcp = new InstanceFunction<Foo, void(int)>(&f, &Foo::func);
 * funcp(1); // call Foo::func
 * funcp = new InstanceFunction<Bar, void(int)>(&b, &Bar::func);
 * funcp(2); // call Bar::func
 * @endcode
 *
 * @note デリゲートとして定義できる関数の引数は3つまで
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 The KJSD Project. All rights reserved.
 *
 ***********************************************************************/
#ifndef KJSD_DELEGATE_HPP
#define KJSD_DELEGATE_HPP

#include <kjsd/shared_ptr.hpp>

#define KJSD_DELEGATE_TPAR0 typename R
#define KJSD_DELEGATE_TPAR1 KJSD_DELEGATE_TPAR0, typename A0
#define KJSD_DELEGATE_TPAR2 KJSD_DELEGATE_TPAR1, typename A1
#define KJSD_DELEGATE_TPAR3 KJSD_DELEGATE_TPAR2, typename A2

#define KJSD_DELEGATE_TARG0 
#define KJSD_DELEGATE_TARG1 A0
#define KJSD_DELEGATE_TARG2 KJSD_DELEGATE_TARG1, A1
#define KJSD_DELEGATE_TARG3 KJSD_DELEGATE_TARG2, A2

#define KJSD_DELEGATE_FPAR0
#define KJSD_DELEGATE_FPAR1 A0 a0
#define KJSD_DELEGATE_FPAR2 KJSD_DELEGATE_FPAR1, A1 a1
#define KJSD_DELEGATE_FPAR3 KJSD_DELEGATE_FPAR2, A2 a2

#define KJSD_DELEGATE_FARG0
#define KJSD_DELEGATE_FARG1 a0
#define KJSD_DELEGATE_FARG2 KJSD_DELEGATE_FARG1, a1
#define KJSD_DELEGATE_FARG3 KJSD_DELEGATE_FARG2, a2

namespace kjsd
{
    template<typename R>
    struct Nop
    {
        static R f() { return R(); }
    };

    /**
     * @brief 関数オブジェクトインタフェース
     *
     * 関数として引数と戻り値のみを定義したインタフェース．
     */
    template<typename Sig> class Function;
#define KJSD_FUNCTION_DEF(num)                                          \
    template<KJSD_DELEGATE_TPAR##num>                                   \
    class Function<R(KJSD_DELEGATE_TARG##num)>                          \
    {                                                                   \
    public:                                                             \
        virtual ~Function() {}                                          \
        virtual R operator()(KJSD_DELEGATE_FPAR##num) const = 0;        \
        virtual bool operator==                                         \
            (const Function<R(KJSD_DELEGATE_TARG##num)>&) const = 0;    \
        virtual bool operator!=                                         \
            (const Function<R(KJSD_DELEGATE_TARG##num)>&) const = 0;    \
    }

    /**
     * @brief インスタンス関数オブジェクト
     *
     * 実行に必要なインスタンスの実体を保持した関数オブジェクト．
     * Functionを実装することで共通のインタフェースを提供する．
     */
    template<typename T, typename Sig> class InstanceFunction;
#define KJSD_INSTANCE_FUNCTION_DEF(num)                                 \
    template<typename T, KJSD_DELEGATE_TPAR##num>                       \
    class InstanceFunction<T, R(KJSD_DELEGATE_TARG##num)> :             \
        public Function<R(KJSD_DELEGATE_TARG##num)>                     \
    {                                                                   \
    public:                                                             \
        typedef R (T::*function_t)(KJSD_DELEGATE_FPAR##num);            \
                                                                        \
        InstanceFunction(T* i = 0, function_t f = 0) :                  \
            inst_(i), f_(f) {}                                          \
        InstanceFunction(const InstanceFunction& me)                    \
        { *this = me; }                                                 \
        virtual ~InstanceFunction() {}                                  \
                                                                        \
        virtual InstanceFunction& operator=(const InstanceFunction &me) \
        {                                                               \
            inst_ = me.inst_;                                           \
            f_ = me.f_;                                                 \
            return *this;                                               \
        }                                                               \
        virtual R operator()(KJSD_DELEGATE_FPAR##num) const             \
        {                                                               \
            return ((inst_ == 0) || (f_ == 0)) ?                        \
                Nop<R>::f():                                            \
                (inst_->*f_)(KJSD_DELEGATE_FARG##num);                  \
        }                                                               \
        virtual bool operator==                                         \
            (const Function<R(KJSD_DELEGATE_TARG##num)>& fi) const      \
        {                                                               \
            const InstanceFunction<T, R(KJSD_DELEGATE_TARG##num)>*      \
                func = dynamic_cast<const InstanceFunction              \
                <T, R(KJSD_DELEGATE_TARG##num)>*>(&fi);                 \
            return (func != 0) &&                                       \
                ((inst_ == func->inst_) && (f_ == func->f_));           \
        }                                                               \
        virtual bool operator!=                                         \
            (const Function<R(KJSD_DELEGATE_TARG##num)>& fi) const      \
        {                                                               \
            return !(operator==(fi));                                   \
        }                                                               \
    private:                                                            \
        T* inst_;                                                       \
        function_t f_;                                                  \
    }

    /**
     * @brief スタティック関数オブジェクト
     *
     * 静的関数オブジェクト．
     * Functionを実装することで共通のインタフェースを提供する．
     */
    template<typename Sig> class StaticFunction;
#define KJSD_STATIC_FUNCTION_DEF(num)                               \
    template<KJSD_DELEGATE_TPAR##num>                               \
    class StaticFunction<R(KJSD_DELEGATE_TARG##num)> :              \
        public Function<R(KJSD_DELEGATE_TARG##num)>                 \
    {                                                               \
    public:                                                         \
        typedef R (*function_t)(KJSD_DELEGATE_FPAR##num);           \
                                                                    \
        StaticFunction(function_t f = 0) : f_(f) {}                 \
        StaticFunction(const StaticFunction& me)                    \
        { *this = me; }                                             \
        virtual ~StaticFunction() {}                                \
                                                                    \
        virtual StaticFunction& operator=(const StaticFunction &me) \
            {                                                       \
                f_ = me.f_;                                         \
                return *this;                                       \
            }                                                       \
        virtual R operator()(KJSD_DELEGATE_FPAR##num) const         \
        {                                                           \
            return (f_ == 0) ?                                      \
                Nop<R>::f():                                        \
                (*f_)(KJSD_DELEGATE_FARG##num);                     \
        }                                                           \
        virtual bool operator==                                     \
            (const Function<R(KJSD_DELEGATE_TARG##num)>& fi) const  \
        {                                                           \
            const StaticFunction<R(KJSD_DELEGATE_TARG##num)>*       \
                func = dynamic_cast<const StaticFunction            \
                <R(KJSD_DELEGATE_TARG##num)>*>(&fi);                \
            return (func != 0) && (f_ == func->f_);                 \
        }                                                           \
        virtual bool operator!=                                     \
            (const Function<R(KJSD_DELEGATE_TARG##num)>& fi) const  \
        {                                                           \
            return !(operator==(fi));                               \
        }                                                           \
    private:                                                        \
        function_t f_;                                              \
    }

    /**
     * @brief デリゲート(汎用関数オブジェクト)
     */
    template<typename Sig> class Delegate;
#define KJSD_DELEGATE_DEF(num)                                          \
    template<KJSD_DELEGATE_TPAR##num>                                   \
    class Delegate<R(KJSD_DELEGATE_TARG##num)>                          \
    {                                                                   \
    public:                                                             \
        Delegate(Function<R(KJSD_DELEGATE_TARG##num)>* fp = 0) :        \
            fp_(fp)                                                     \
        {}                                                              \
        virtual ~Delegate() {}                                          \
                                                                        \
        Delegate(const Delegate<R(KJSD_DELEGATE_TARG##num)>& d)         \
        { *this = d; }                                                  \
        Delegate<R(KJSD_DELEGATE_TARG##num)>&                           \
            operator=(const Delegate<R(KJSD_DELEGATE_TARG##num)>& d)    \
            {                                                           \
                fp_ = d.fp_;                                            \
                return *this;                                           \
            }                                                           \
                                                                        \
        virtual Delegate<R(KJSD_DELEGATE_TARG##num)>&                   \
            operator=(Function<R(KJSD_DELEGATE_TARG##num)>* fp)         \
            {                                                           \
                fp_ = fp;                                               \
                return *this;                                           \
            }                                                           \
        virtual R operator()(KJSD_DELEGATE_FPAR##num) const             \
        {                                                               \
            return (fp_ == 0) ?                                         \
                Nop<R>::f():                                            \
                (*fp_)(KJSD_DELEGATE_FARG##num);                        \
        }                                                               \
        virtual bool operator==                                         \
            (const Delegate<R(KJSD_DELEGATE_TARG##num)>& d) const       \
        {                                                               \
            if ((fp_ == 0) || (d.fp_ == 0))                             \
                return (fp_ == 0) && (d.fp_ == 0);                      \
            else                                                        \
                return *fp_ == *(d.fp_);                                \
        }                                                               \
        virtual bool operator!=                                         \
            (const Delegate<R(KJSD_DELEGATE_TARG##num)>& d) const       \
        {                                                               \
            return !(operator==(d));                                    \
        }                                                               \
    private:                                                            \
        SharedPtr<Function<R(KJSD_DELEGATE_TARG##num)> > fp_;           \
    }

    KJSD_FUNCTION_DEF(0);
    KJSD_FUNCTION_DEF(1);
    KJSD_FUNCTION_DEF(2);
    KJSD_FUNCTION_DEF(3);
    KJSD_INSTANCE_FUNCTION_DEF(0);
    KJSD_INSTANCE_FUNCTION_DEF(1);
    KJSD_INSTANCE_FUNCTION_DEF(2);
    KJSD_INSTANCE_FUNCTION_DEF(3);
    KJSD_STATIC_FUNCTION_DEF(0);
    KJSD_STATIC_FUNCTION_DEF(1);
    KJSD_STATIC_FUNCTION_DEF(2);
    KJSD_STATIC_FUNCTION_DEF(3);
    KJSD_DELEGATE_DEF(0);
    KJSD_DELEGATE_DEF(1);
    KJSD_DELEGATE_DEF(2);
    KJSD_DELEGATE_DEF(3);
}

#endif // KJSD_DELEGATE_HPP
