/**
 * @file shared_ptr.hpp
 *
 * @version $Id:$
 *
 * @brief A shared pointer
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2015 The KJSD Project. All rights reserved.
 *
 ***********************************************************************/
#ifndef KJSD_SHARED_PTR_HPP
#define KJSD_SHARED_PTR_HPP

#include <cassert>
#include <kjsd/hash_table.hpp>
#include <kjsd/cutil.h>

namespace kjsd
{
    // tbd. thread safe
    template<typename T> class SharedPtr
    {
    public:
        SharedPtr(T* p = 0) : p_(p)
        {
            if (p_ == 0) return;
            ++reference_counter()[p_];
        }
        virtual ~SharedPtr()
        {
            delete_if_noref();
        }
        SharedPtr(const SharedPtr& me) : p_(0)
        { *this = me; }
        SharedPtr& operator=(const SharedPtr& me)
        {
            *this = me.p_;
            return *this;
        }
        SharedPtr& operator=(T* p)
        {
            if (p != 0)
            {
                ++reference_counter()[p];
            }
            delete_if_noref();
            p_ = p;

            return *this;
        }

        T& operator*() const
        {
            return *p_;
        }

        T* operator->() const
        {
            return p_;
        }

        bool operator==(const SharedPtr<T>& p) const
        {
            return p_ == p.p_;
        }
        bool operator!=(const SharedPtr<T>& p) const
        {
            return !(operator==(p));
        }
        bool operator==(const T* p)
        {
            return p_ == p;
        }
        bool operator!=(const T* p)
        {
            return !(operator==(p));
        }

        size_t use_count() const
        {
            return (p_ == 0) ? 0: reference_counter()[p_];
        }

    private:
        T* p_;
        static HashTable<T*, size_t>& reference_counter();

        void delete_if_noref()
        {
            if (p_ == 0) return;
            assert(reference_counter().count(p_) != 0);
            assert(reference_counter()[p_] > 0);

            if (--reference_counter()[p_] == 0)
            {
                reference_counter().erase(p_);
                KJSD_DPRINTF("delete %ld\n", reinterpret_cast<size_t>(p_));
                delete p_;
                p_ = 0;
            }
        }
    };

    template<typename T>
    HashTable<T*, size_t>& SharedPtr<T>::reference_counter()
    {
        static HashTable<T*, size_t> ref_cnt;
        return ref_cnt;
    }
}

#endif // KJSD_SHARED_PTR_HPP
