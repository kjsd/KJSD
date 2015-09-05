/**
 * @file utf8_codecvt_facet.hpp
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
#ifndef KJSD_UTF8_CODECVT_FACET_HPP
#define KJSD_UTF8_CODECVT_FACET_HPP

#include <boost/filesystem/config.hpp>

#define BOOST_UTF8_BEGIN_NAMESPACE                              \
    namespace boost { namespace filesystem { namespace detail {

#define BOOST_UTF8_END_NAMESPACE }}}
#define BOOST_UTF8_DECL BOOST_FILESYSTEM_DECL

#include <boost/detail/utf8_codecvt_facet.hpp>

#undef BOOST_UTF8_BEGIN_NAMESPACE
#undef BOOST_UTF8_END_NAMESPACE
#undef BOOST_UTF8_DECL

#endif // KJSD_UTF8_CODECVT_FACET_HPP
