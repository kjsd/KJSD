/**
 * @file state_machine.hpp
 *
 * @version $Id:$
 *
 * @brief
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2015 The KJSD Project. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#ifndef KJSD_STATE_MACHINE_HPP
#define KJSD_STATE_MACHINE_HPP

#include <kjsd/singleton.hpp>
#include <kjsd/command.hpp>
#include <kjsd/hash_table.hpp>
#include <kjsd/cutil.h>

namespace kjsd
{
    template<typename E, typename HT = void(), typename AT = HT>
    class State
    {
    public:
        class Transition
        {
        public:
            State<E,HT,AT>* next;
            Command<AT> action;
            Delegate<bool()> guard;
            Transition(State<E,HT,AT>* n = 0,
                       Command<AT> act = 0,
                       Delegate<bool()> gd = new StaticFunction<bool()>(
                           &Transition::always_true)) :
                next(n), action(act), guard(gd)
            {}

        private:
            static bool always_true() { return true; }
        };

        virtual const char* name() const = 0;
        virtual State<E>* parent() const = 0;

        Command<HT> entry;
        Command<HT> exit;
        HashTable<E, Transition> transitions;

        State(Command<HT> ent = 0, Command<HT> ext = 0) :
            entry(ent), exit(ext)
        {}

        bool acceptable(const E& e) const
        {
            return transitions.count(e) != 0;
        }
        void clear()
        {
            entry = 0;
            exit = 0;
            transitions.clear();
        }
    };

    template<typename E, typename HT = void(), typename AT = HT>
    class StateMachine
    {
    public:
        typedef State<E,HT,AT> state_type;
        typedef typename state_type::Transition transition_type;

        class Start: public state_type,
                     public Singleton<Start>
        {
            friend class Singleton<Start>;
        public:
            const char* name() const { return "Start"; }
            state_type* parent() const { return 0; }
        private:
            Start() {}
            ~Start() {}
        };
        class End: public state_type,
                   public Singleton<End>
        {
            friend class Singleton<End>;
        public:
            const char* name() const { return "End"; }
            state_type* parent() const { return 0; }
        private:
            End() {}
            ~End() {}
        };

        StateMachine() : current_(Start::instance()) {}
        virtual ~StateMachine() {}

        void fire(const E& e)
        {
            state_type* s = current_;
            for (; (s != 0) && !s->acceptable(e); s = s->parent());
            if (s == 0) return;

            if (s->transitions[e].guard())
            {
                s->transitions[e].action();
                change_state(s->transitions[e].next);
            }
        }
        state_type* current() const { return current_; }
    protected:
        void change_state(state_type* next)
        {
            state_type* src = current_;
            state_type* dst = next;

            if ((dst == 0) || (dst == src)) return;

            typedef typename std::vector<state_type*> state_list;

            state_list src_list;
            for (state_type* s = src; s != 0; s = s->parent())
            {
                src_list.push_back(s);
            }
            state_list dst_list;
            for (state_type* s = dst; s != 0; s = s->parent())
            {
                dst_list.push_back(s);
            }

            while (src_list.back() == dst_list.back())
            {
                src_list.pop_back();
                dst_list.pop_back();
            }

            for (typename state_list::iterator it = src_list.begin();
                 it != src_list.end();
                 ++it)
            {
                (*it)->exit();
            }
            while (!dst_list.empty())
            {
                dst_list.back()->entry();
                dst_list.pop_back();
            }

            KJSD_DPRINTF("%s -> %s\n", current_->name(), next->name());
            current_ = next;
        }
    private:
        state_type* current_;
    };
}

#endif // KJSD_STATE_MACHINE_HPP
