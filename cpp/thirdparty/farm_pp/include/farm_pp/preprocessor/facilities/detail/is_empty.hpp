# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
#ifndef FARM_PP_PREPROCESSOR_DETAIL_IS_EMPTY_HPP
#define FARM_PP_PREPROCESSOR_DETAIL_IS_EMPTY_HPP

#include <farm_pp/preprocessor/punctuation/is_begin_parens.hpp>

#if FARM_PP_VARIADICS_MSVC

# pragma warning(once:4002)

#define FARM_PP_DETAIL_IS_EMPTY_IIF_0(t, b) b
#define FARM_PP_DETAIL_IS_EMPTY_IIF_1(t, b) t

#else

#define FARM_PP_DETAIL_IS_EMPTY_IIF_0(t, ...) __VA_ARGS__
#define FARM_PP_DETAIL_IS_EMPTY_IIF_1(t, ...) t

#endif

#if FARM_PP_VARIADICS_MSVC && _MSC_VER <= 1400

#define FARM_PP_DETAIL_IS_EMPTY_PROCESS(param) \
    FARM_PP_IS_BEGIN_PARENS \
        ( \
        FARM_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C param () \
        ) \
/**/

#else

#define FARM_PP_DETAIL_IS_EMPTY_PROCESS(...) \
    FARM_PP_IS_BEGIN_PARENS \
        ( \
        FARM_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C __VA_ARGS__ () \
        ) \
/**/

#endif

#define FARM_PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(a, b) a ## b
#define FARM_PP_DETAIL_IS_EMPTY_IIF(bit) FARM_PP_DETAIL_IS_EMPTY_PRIMITIVE_CAT(FARM_PP_DETAIL_IS_EMPTY_IIF_,bit)
#define FARM_PP_DETAIL_IS_EMPTY_NON_FUNCTION_C(...) ()

#endif /* FARM_PP_PREPROCESSOR_DETAIL_IS_EMPTY_HPP */
