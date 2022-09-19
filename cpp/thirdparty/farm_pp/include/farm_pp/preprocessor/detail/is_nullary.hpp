# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_DETAIL_IS_NULLARY_HPP
# define FARM_PP_PREPROCESSOR_DETAIL_IS_NULLARY_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/detail/check.hpp>
#
# /* FARM_PP_IS_NULLARY */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_IS_NULLARY(x) FARM_PP_CHECK(x, FARM_PP_IS_NULLARY_CHECK)
# else
#    define FARM_PP_IS_NULLARY(x) FARM_PP_IS_NULLARY_I(x)
#    define FARM_PP_IS_NULLARY_I(x) FARM_PP_CHECK(x, FARM_PP_IS_NULLARY_CHECK)
# endif
#
# define FARM_PP_IS_NULLARY_CHECK() 1
# define FARM_PP_CHECK_RESULT_FARM_PP_IS_NULLARY_CHECK 0, FARM_PP_NIL
#
# endif
