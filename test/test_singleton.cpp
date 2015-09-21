/**
 * @file test_singleton.cpp
 *
 * @brief A unit test suite of Singleton
 *
 * @author Kenji MINOURA / kenji@kandj.org
 *
 * Copyright (c) 2012 K&J Software Design, Ltd. All rights reserved.
 *
 * @see <related_items>
 ***********************************************************************/
#include <kjsd/cunit.h>
#include <kjsd/singleton.hpp>

using namespace kjsd;

namespace test_singleton_ns
{
    class SingletonImpl : public Singleton<SingletonImpl>
    {
    public:
        int aaa;
    private:
        friend class Singleton<SingletonImpl>;
        SingletonImpl() : aaa(10) {}
        ~SingletonImpl() {}
    };
}

using namespace test_singleton_ns;

static const char* test_new();


static const char* test_new()
{
    // new SingletonImpl();
    SingletonImpl* ins = SingletonImpl::instance();

    KJSD_CUNIT_ASSERT(ins != 0);
    KJSD_CUNIT_ASSERT(ins == SingletonImpl::instance());
    KJSD_CUNIT_ASSERT(ins->aaa == 10);

    // delete ins;

    return 0;
}

const char* test_singleton()
{
    KJSD_CUNIT_RUN(test_new);
    return 0;
}
