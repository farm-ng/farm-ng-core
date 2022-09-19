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
# ifndef FARM_PP_PREPROCESSOR_FACILITIES_APPLY_HPP
# define FARM_PP_PREPROCESSOR_FACILITIES_APPLY_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/expr_iif.hpp>
# include <farm_pp/preprocessor/detail/is_unary.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_APPLY */
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_APPLY(x) FARM_PP_APPLY_I(x)
#    define FARM_PP_APPLY_I(x) FARM_PP_EXPR_IIF(FARM_PP_IS_UNARY(x), FARM_PP_TUPLE_REM_1 x)
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_BCC()
#    define FARM_PP_APPLY(x) FARM_PP_APPLY_I(x)
#    define FARM_PP_APPLY_I(x) FARM_PP_APPLY_ ## x
#    define FARM_PP_APPLY_(x) x
#    define FARM_PP_APPLY_FARM_PP_NIL
# else
#    define FARM_PP_APPLY(x) FARM_PP_EXPR_IIF(FARM_PP_IS_UNARY(x), FARM_PP_TUPLE_REM_1 x)
# endif
#
# endif
