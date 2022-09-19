# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
# /* Revised by Edward Diener (2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_ARITHMETIC_DIV_HPP
# define FARM_PP_PREPROCESSOR_ARITHMETIC_DIV_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/arithmetic/detail/div_base.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_DIV */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_DIV(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE(x, y))
# else
#    define FARM_PP_DIV(x, y) FARM_PP_DIV_I(x, y)
#    define FARM_PP_DIV_I(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE(x, y))
# endif
#
# /* FARM_PP_DIV_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_DIV_D(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE_D(d, x, y))
# else
#    define FARM_PP_DIV_D(d, x, y) FARM_PP_DIV_D_I(d, x, y)
#    define FARM_PP_DIV_D_I(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE_D(d, x, y))
# endif
#
# else
#
# include <farm_pp/preprocessor/arithmetic/detail/div_base.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/identity.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/arithmetic/detail/is_1_number.hpp>
#
# /* FARM_PP_DIV */
#
#    define FARM_PP_DIV(x, y) FARM_PP_IIF(FARM_PP_DETAIL_IS_1_NUMBER(y),FARM_PP_IDENTITY_N(x,2),FARM_PP_DIV_DO)(x,y)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_DIV_DO(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE(x, y))
# else
#    define FARM_PP_DIV_DO(x, y) FARM_PP_DIV_I(x, y)
#    define FARM_PP_DIV_I(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE(x, y))
# endif
#
# /* FARM_PP_DIV_D */
#
#    define FARM_PP_DIV_D(d, x, y) FARM_PP_IIF(FARM_PP_DETAIL_IS_1_NUMBER(y),FARM_PP_IDENTITY_N(x,3),FARM_PP_DIV_DO_D)(d,x,y)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_DIV_DO_D(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE_D(d, x, y))
# else
#    define FARM_PP_DIV_DO_D(d, x, y) FARM_PP_DIV_D_I(d, x, y)
#    define FARM_PP_DIV_D_I(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_DIV_BASE_D(d, x, y))
# endif
#
# endif
#
# endif
