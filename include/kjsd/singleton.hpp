/**
 * @file singleton.hpp
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
#ifndef KJSD_SINGLETON_HPP
#define KJSD_SINGLETON_HPP

namespace kjsd
{
    template<typename T> class Singleton
    {
    public:
        static T* instance()
        {
            static T ins;
            return &ins;
        }
    protected:
        Singleton() {}
        ~Singleton() {}
    private:
        Singleton(const Singleton&);
        Singleton& operator=(const Singleton&);
    };
};

#endif // KJSD_SINGLETON_HPP
