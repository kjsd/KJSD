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
            ++ref_cnt_[p_];
        }
        virtual ~SharedPtr()
        {
            delete_if_nocnt(p_);
        }
        SharedPtr(const SharedPtr& me)
        { *this = me; }
        SharedPtr& operator=(const SharedPtr& me)
        {
            *this = me.p_;
            return *this;
        }
        SharedPtr& operator=(T* p)
        {
            T* tmp = p_;

            p_ = p;
            if (p_ != 0)
            {
                ++ref_cnt_[p_];
            }
            delete_if_nocnt(tmp);

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
            return (p_ == 0) ? 0: ref_cnt_[p_];
        }

    private:
        T* p_;
        static HashTable<T*, size_t> ref_cnt_;

        void delete_if_nocnt(T* p)
        {
            if (p_ == 0) return;

            if (--ref_cnt_[p] == 0)
            {
                ref_cnt_.erase(p);
                // KJSD_CUTIL_DPRINTF("delete %ld\n", reinterpret_cast<size_t>(p));
                delete p;
            }
        }
    };

    template<typename T>
    HashTable<T*, size_t> SharedPtr<T>::ref_cnt_;
}

#endif // KJSD_SHARED_PTR_HPP
