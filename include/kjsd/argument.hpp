/**
 * @file argument.hpp
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
#ifndef KJSD_ARGUMENT_HPP
#define KJSD_ARGUMENT_HPP

namespace kjsd
{
    class Argument
    {
    public:
        enum Status
        {
            SHORT,
            LONG,
            ARG
        };

        class iterator
        {
        public:
            iterator(const Argument &arg) :
                arg_(&arg), index_(0), optp_(0), stat_(ARG) {}
            iterator(const Argument &arg, int idx, const char *op) :
                arg_(&arg), index_(idx), optp_(op), stat_(ARG) {}
            iterator(const iterator &it)
            {
                *this = it;
            }

            iterator& operator=(const iterator &it);
            char operator*() const;
            iterator& operator++();
            iterator operator++(int);
            bool operator==(const iterator &it) const;
            bool operator!=(const iterator &it) const;
            const char *argument() const;
            Status status() const
            {
                return stat_;
            }
                
        private:
            const Argument* arg_;
            int index_;
            const char *optp_;
            Status stat_;
        };

        Argument(int argc, const char * const *argv) :
            argc_(argc), argv_(argv) {}

        virtual ~Argument() {}

        const char *argv(int idx) const;
        const iterator begin() const;
        const iterator end() const;

    private:
        int argc_;
        const char * const *argv_;
    };

    inline const char *Argument::argv(int idx) const
    {
        return (idx < argc_) ? argv_[idx]: 0;
    }

    inline const Argument::iterator Argument::begin() const
    {
        Argument::iterator it(*this);

        return ++it;
    }

    inline const Argument::iterator Argument::end() const
    {
        return Argument::iterator(*this, argc_, 0);
    }
}

#endif // KJSD_ARGUMENT_HPP
