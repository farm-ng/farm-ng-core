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
# ifndef FARM_PP_PREPROCESSOR_IS_BEGIN_PARENS_HPP
# define FARM_PP_PREPROCESSOR_IS_BEGIN_PARENS_HPP

#include <farm_pp/preprocessor/config/config.hpp>
#include <farm_pp/preprocessor/punctuation/detail/is_begin_parens.hpp>

#if FARM_PP_VARIADICS_MSVC && _MSC_VER <= 1400

#define FARM_PP_IS_BEGIN_PARENS(param) \
    FARM_PP_DETAIL_IBP_SPLIT \
      ( \
      0, \
      FARM_PP_DETAIL_IBP_CAT \
        ( \
        FARM_PP_DETAIL_IBP_IS_VARIADIC_R_, \
        FARM_PP_DETAIL_IBP_IS_VARIADIC_C param \
        ) \
      ) \
/**/

#else

#define FARM_PP_IS_BEGIN_PARENS(...) \
    FARM_PP_DETAIL_IBP_SPLIT \
      ( \
      0, \
      FARM_PP_DETAIL_IBP_CAT \
        ( \
        FARM_PP_DETAIL_IBP_IS_VARIADIC_R_, \
        FARM_PP_DETAIL_IBP_IS_VARIADIC_C __VA_ARGS__ \
        ) \
      ) \
/**/

#endif /* FARM_PP_VARIADICS_MSVC && _MSC_VER <= 1400 */
#endif /* FARM_PP_PREPROCESSOR_IS_BEGIN_PARENS_HPP */
