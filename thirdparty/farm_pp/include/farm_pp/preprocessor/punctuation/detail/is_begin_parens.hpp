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
#ifndef FARM_PP_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP
#define FARM_PP_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP

#if FARM_PP_VARIADICS_MSVC

#include <farm_pp/preprocessor/facilities/empty.hpp>

#define FARM_PP_DETAIL_VD_IBP_CAT(a, b) FARM_PP_DETAIL_VD_IBP_CAT_I(a, b)
#define FARM_PP_DETAIL_VD_IBP_CAT_I(a, b) FARM_PP_DETAIL_VD_IBP_CAT_II(a ## b)
#define FARM_PP_DETAIL_VD_IBP_CAT_II(res) res

#define FARM_PP_DETAIL_IBP_SPLIT(i, ...) \
    FARM_PP_DETAIL_VD_IBP_CAT(FARM_PP_DETAIL_IBP_PRIMITIVE_CAT(FARM_PP_DETAIL_IBP_SPLIT_,i)(__VA_ARGS__),FARM_PP_EMPTY()) \
/**/

#define FARM_PP_DETAIL_IBP_IS_VARIADIC_C(...) 1 1

#else

#define FARM_PP_DETAIL_IBP_SPLIT(i, ...) \
    FARM_PP_DETAIL_IBP_PRIMITIVE_CAT(FARM_PP_DETAIL_IBP_SPLIT_,i)(__VA_ARGS__) \
/**/

#define FARM_PP_DETAIL_IBP_IS_VARIADIC_C(...) 1

#endif /* FARM_PP_VARIADICS_MSVC */

#define FARM_PP_DETAIL_IBP_SPLIT_0(a, ...) a
#define FARM_PP_DETAIL_IBP_SPLIT_1(a, ...) __VA_ARGS__

#define FARM_PP_DETAIL_IBP_CAT(a, ...) FARM_PP_DETAIL_IBP_PRIMITIVE_CAT(a,__VA_ARGS__)
#define FARM_PP_DETAIL_IBP_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define FARM_PP_DETAIL_IBP_IS_VARIADIC_R_1 1,
#define FARM_PP_DETAIL_IBP_IS_VARIADIC_R_FARM_PP_DETAIL_IBP_IS_VARIADIC_C 0,

#endif /* FARM_PP_PREPROCESSOR_DETAIL_IS_BEGIN_PARENS_HPP */
