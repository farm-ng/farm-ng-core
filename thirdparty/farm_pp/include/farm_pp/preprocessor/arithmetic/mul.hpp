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
# ifndef FARM_PP_PREPROCESSOR_ARITHMETIC_MUL_HPP
# define FARM_PP_PREPROCESSOR_ARITHMETIC_MUL_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/arithmetic/add.hpp>
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_MUL */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_MUL(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# else
#    define FARM_PP_MUL(x, y) FARM_PP_MUL_I(x, y)
#    define FARM_PP_MUL_I(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# endif
#
# define FARM_PP_MUL_P(d, rxy) FARM_PP_TUPLE_ELEM(3, 2, rxy)
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_MUL_O(d, rxy) FARM_PP_MUL_O_IM(d, FARM_PP_TUPLE_REM_3 rxy)
#    define FARM_PP_MUL_O_IM(d, im) FARM_PP_MUL_O_I(d, im)
# else
#    define FARM_PP_MUL_O(d, rxy) FARM_PP_MUL_O_I(d, FARM_PP_TUPLE_ELEM(3, 0, rxy), FARM_PP_TUPLE_ELEM(3, 1, rxy), FARM_PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define FARM_PP_MUL_O_I(d, r, x, y) (FARM_PP_ADD_D(d, r, x), x, FARM_PP_DEC(y))
#
# /* FARM_PP_MUL_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_MUL_D(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE_ ## d(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# else
#    define FARM_PP_MUL_D(d, x, y) FARM_PP_MUL_D_I(d, x, y)
#    define FARM_PP_MUL_D_I(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE_ ## d(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# endif
#
# else
#
# include <farm_pp/preprocessor/arithmetic/add.hpp>
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/facilities/identity.hpp>
# include <farm_pp/preprocessor/logical/bitand.hpp>
# include <farm_pp/preprocessor/logical/bool.hpp>
# include <farm_pp/preprocessor/logical/compl.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# include <farm_pp/preprocessor/arithmetic/detail/is_minimum_number.hpp>
# include <farm_pp/preprocessor/arithmetic/detail/is_maximum_number.hpp>
# include <farm_pp/preprocessor/arithmetic/detail/is_1_number.hpp>
#
# /* FARM_PP_MUL */
#
#    define FARM_PP_MUL(x, y) FARM_PP_IIF(FARM_PP_DETAIL_IS_MINIMUM_NUMBER(x),FARM_PP_IDENTITY_N(x,2),FARM_PP_MUL_CHECK_1X)(x,y)
#
#    define FARM_PP_MUL_CHECK_1X(x, y) FARM_PP_IIF(FARM_PP_DETAIL_IS_1_NUMBER(x),FARM_PP_IDENTITY_N(y,2),FARM_PP_MUL_DO)(x,y)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_MUL_DO(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# else
#    define FARM_PP_MUL_DO(x, y) FARM_PP_MUL_I(x, y)
#    define FARM_PP_MUL_I(x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# endif
#
# define FARM_PP_MUL_P(d, rxy) FARM_PP_BITAND(FARM_PP_BOOL(FARM_PP_TUPLE_ELEM(3, 2, rxy)),FARM_PP_COMPL(FARM_PP_DETAIL_IS_MAXIMUM_NUMBER(FARM_PP_TUPLE_ELEM(3, 0, rxy))))
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_MUL_O(d, rxy) FARM_PP_MUL_O_IM(d, FARM_PP_TUPLE_REM_3 rxy)
#    define FARM_PP_MUL_O_IM(d, im) FARM_PP_MUL_O_I(d, im)
# else
#    define FARM_PP_MUL_O(d, rxy) FARM_PP_MUL_O_I(d, FARM_PP_TUPLE_ELEM(3, 0, rxy), FARM_PP_TUPLE_ELEM(3, 1, rxy), FARM_PP_TUPLE_ELEM(3, 2, rxy))
# endif
#
# define FARM_PP_MUL_O_I(d, r, x, y) (FARM_PP_ADD_D(d, r, x), x, FARM_PP_DEC(y))
#
# /* FARM_PP_MUL_D */
#
#    define FARM_PP_MUL_D(d, x, y) FARM_PP_IIF(FARM_PP_DETAIL_IS_MINIMUM_NUMBER(x),FARM_PP_IDENTITY_N(x,3),FARM_PP_MUL_CHECK_1X_D)(d,x,y)
#
#    define FARM_PP_MUL_CHECK_1X_D(d, x, y) FARM_PP_IIF(FARM_PP_DETAIL_IS_1_NUMBER(x),FARM_PP_IDENTITY_N(y,3),FARM_PP_MUL_DO_D)(d,x,y)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_MUL_DO_D(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE_ ## d(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# else
#    define FARM_PP_MUL_DO_D(d, x, y) FARM_PP_MUL_D_I(d, x, y)
#    define FARM_PP_MUL_D_I(d, x, y) FARM_PP_TUPLE_ELEM(3, 0, FARM_PP_WHILE_ ## d(FARM_PP_MUL_P, FARM_PP_MUL_O, (0, x, y)))
# endif
#
# endif
#
# endif
