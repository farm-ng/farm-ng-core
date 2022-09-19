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
# ifndef FARM_PP_PREPROCESSOR_ARITHMETIC_ADD_HPP
# define FARM_PP_PREPROCESSOR_ARITHMETIC_ADD_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_ADD */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ADD(x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# else
#    define FARM_PP_ADD(x, y) FARM_PP_ADD_I(x, y)
#    define FARM_PP_ADD_I(x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# endif
#
# define FARM_PP_ADD_P(d, xy) FARM_PP_TUPLE_ELEM(2, 1, xy)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_ADD_O(d, xy) FARM_PP_ADD_O_I xy
# else
#    define FARM_PP_ADD_O(d, xy) FARM_PP_ADD_O_I(FARM_PP_TUPLE_ELEM(2, 0, xy), FARM_PP_TUPLE_ELEM(2, 1, xy))
# endif
#
# define FARM_PP_ADD_O_I(x, y) (FARM_PP_INC(x), FARM_PP_DEC(y))
#
# /* FARM_PP_ADD_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ADD_D(d, x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# else
#    define FARM_PP_ADD_D(d, x, y) FARM_PP_ADD_D_I(d, x, y)
#    define FARM_PP_ADD_D_I(d, x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# endif
#
# else
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/facilities/identity.hpp>
# include <farm_pp/preprocessor/logical/bitand.hpp>
# include <farm_pp/preprocessor/logical/bitor.hpp>
# include <farm_pp/preprocessor/logical/bool.hpp>
# include <farm_pp/preprocessor/logical/compl.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/arithmetic/detail/is_maximum_number.hpp>
# include <farm_pp/preprocessor/arithmetic/detail/is_minimum_number.hpp>
#
# /* FARM_PP_ADD */
#
#    define FARM_PP_ADD(x, y) FARM_PP_IIF(FARM_PP_BITOR(FARM_PP_DETAIL_IS_MAXIMUM_NUMBER(y),FARM_PP_DETAIL_IS_MINIMUM_NUMBER(x)),FARM_PP_IDENTITY_N(y,2),FARM_PP_ADD_DO)(x,y)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ADD_DO(x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# else
#    define FARM_PP_ADD_DO(x, y) FARM_PP_ADD_I(x, y)
#    define FARM_PP_ADD_I(x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# endif
#
# define FARM_PP_ADD_P(d, xy) FARM_PP_BITAND(FARM_PP_BOOL(FARM_PP_TUPLE_ELEM(2, 1, xy)),FARM_PP_COMPL(FARM_PP_DETAIL_IS_MAXIMUM_NUMBER(FARM_PP_TUPLE_ELEM(2, 0, xy))))
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_ADD_O(d, xy) FARM_PP_ADD_O_I xy
# else
#    define FARM_PP_ADD_O(d, xy) FARM_PP_ADD_O_I(FARM_PP_TUPLE_ELEM(2, 0, xy), FARM_PP_TUPLE_ELEM(2, 1, xy))
# endif
#
# define FARM_PP_ADD_O_I(x, y) (FARM_PP_INC(x), FARM_PP_DEC(y))
#
# /* FARM_PP_ADD_D */
#
#    define FARM_PP_ADD_D(d, x, y) FARM_PP_IIF(FARM_PP_BITOR(FARM_PP_DETAIL_IS_MAXIMUM_NUMBER(y),FARM_PP_DETAIL_IS_MINIMUM_NUMBER(x)),FARM_PP_IDENTITY_N(y,3),FARM_PP_ADD_DO_D)(d,x,y)
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ADD_DO_D(d, x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# else
#    define FARM_PP_ADD_DO_D(d, x, y) FARM_PP_ADD_D_I(d, x, y)
#    define FARM_PP_ADD_D_I(d, x, y) FARM_PP_TUPLE_ELEM(2, 0, FARM_PP_WHILE_ ## d(FARM_PP_ADD_P, FARM_PP_ADD_O, (x, y)))
# endif
#
# endif
#
# endif
