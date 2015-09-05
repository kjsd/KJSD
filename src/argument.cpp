/**
 * @file argument.cpp
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
#include <cassert>
#include "../include/kjsd/argument.hpp"

using namespace kjsd;

Argument::iterator&
Argument::iterator::operator=(const Argument::iterator &it)
{
    arg_ = it.arg_;
    index_ = it.index_;
    optp_ = it.optp_;
    stat_ = it.stat_;

    return *this;
}

char Argument::iterator::operator*() const
{
    return (stat_ == SHORT) ? *optp_: '\0';
}

Argument::iterator& Argument::iterator::operator++()
{
    enum step
    {
        start,
        next_arg,
        next_opt,
        arg_opt,
        end
    };

    const char *p;
    step st = start;

    while (st != end)
    {
        switch (st)
        {
        case start:
            st = (*this == arg_->end()) ? arg_opt: ((stat_ == SHORT) ?
                                                    next_opt: next_arg);
            break;

        case next_arg:
            ++index_;
            p = arg_->argv(index_);
            if (p == 0)
            {
                st = arg_opt;
            }
            else if ((*p == '-') && (*(p + 1) != '-') && (*(p + 1) != '\0'))
            {
                optp_ = p + 1;
                stat_ = SHORT;
                st = end;
            }
            else if ((*p == '-') && (*(p + 1) == '-') && (*(p + 2) != '\0'))
            {
                optp_ = p + 2;
                stat_ = LONG;
                st = end;
            }
            else
            {
                st = arg_opt;
            }
            break;

        case next_opt:
            ++optp_;
            st = (*optp_ == '\0') ? next_arg: end;
            break;

        case arg_opt:
            optp_ = 0;
            stat_ = ARG;
            st = end;
            break;

        default:
            assert(false);
            break;
        }
    }

    return *this;
}

Argument::iterator Argument::iterator::operator++(int)
{
    Argument::iterator it = *this;
    ++(*this);

    return it;
}

bool Argument::iterator::operator==(const Argument::iterator& it) const
{
    return (stat_ == it.stat_) && (index_ == it.index_)
		&& (optp_ == it.optp_);
}

bool Argument::iterator::operator!=(const Argument::iterator& it) const
{
    return (stat_ != it.stat_) || (index_ != it.index_)
		|| (optp_ != it.optp_);
}

const char *Argument::iterator::argument() const
{
    return (stat_ == LONG) ? optp_: arg_->argv(index_);
}
