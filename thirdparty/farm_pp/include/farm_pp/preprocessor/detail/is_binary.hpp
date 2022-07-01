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
# ifndef FARM_PP_PREPROCESSOR_DETAIL_IS_BINARY_HPP
# define FARM_PP_PREPROCESSOR_DETAIL_IS_BINARY_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/detail/check.hpp>
#
# /* FARM_PP_IS_BINARY */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_IS_BINARY(x) FARM_PP_CHECK(x, FARM_PP_IS_BINARY_CHECK)
# else
#    define FARM_PP_IS_BINARY(x) FARM_PP_IS_BINARY_I(x)
#    define FARM_PP_IS_BINARY_I(x) FARM_PP_CHECK(x, FARM_PP_IS_BINARY_CHECK)
# endif
#
# define FARM_PP_IS_BINARY_CHECK(a, b) 1
# define FARM_PP_CHECK_RESULT_FARM_PP_IS_BINARY_CHECK 0, FARM_PP_NIL
#
# endif
