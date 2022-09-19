# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Edward Diener (2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_ARRAY_REMOVE_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_REMOVE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/array/elem.hpp>
# include <farm_pp/preprocessor/array/push_back.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/comparison/not_equal.hpp>
# include <farm_pp/preprocessor/control/deduce_d.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/tuple/eat.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_ARRAY_REMOVE */
#
# define FARM_PP_ARRAY_REMOVE(array, i) FARM_PP_ARRAY_REMOVE_I(FARM_PP_DEDUCE_D(), array, i)
# define FARM_PP_ARRAY_REMOVE_I(d, array, i) FARM_PP_ARRAY_REMOVE_D(d, array, i)
#
# /* FARM_PP_ARRAY_REMOVE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ARRAY_REMOVE_D(d, array, i) FARM_PP_TUPLE_ELEM(4, 2, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REMOVE_P, FARM_PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# else
#    define FARM_PP_ARRAY_REMOVE_D(d, array, i) FARM_PP_ARRAY_REMOVE_D_I(d, array, i)
#    define FARM_PP_ARRAY_REMOVE_D_I(d, array, i) FARM_PP_TUPLE_ELEM(4, 2, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REMOVE_P, FARM_PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# endif
#
# define FARM_PP_ARRAY_REMOVE_P(d, st) FARM_PP_NOT_EQUAL(FARM_PP_TUPLE_ELEM(4, 0, st), FARM_PP_ARRAY_SIZE(FARM_PP_TUPLE_ELEM(4, 3, st)))
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_ARRAY_REMOVE_O(d, st) FARM_PP_ARRAY_REMOVE_O_I st
# else
#    define FARM_PP_ARRAY_REMOVE_O(d, st) FARM_PP_ARRAY_REMOVE_O_I(FARM_PP_TUPLE_ELEM(4, 0, st), FARM_PP_TUPLE_ELEM(4, 1, st), FARM_PP_TUPLE_ELEM(4, 2, st), FARM_PP_TUPLE_ELEM(4, 3, st))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_DMC()
#    define FARM_PP_ARRAY_REMOVE_O_I(n, i, res, arr) (FARM_PP_INC(n), i, FARM_PP_IIF(FARM_PP_NOT_EQUAL(n, i), FARM_PP_ARRAY_PUSH_BACK, res FARM_PP_TUPLE_EAT_2)(res, FARM_PP_ARRAY_ELEM(n, arr)), arr)
# else
#    define FARM_PP_ARRAY_REMOVE_O_I(n, i, res, arr) (FARM_PP_INC(n), i, FARM_PP_IIF(FARM_PP_NOT_EQUAL(n, i), FARM_PP_ARRAY_PUSH_BACK, FARM_PP_TUPLE_ELEM_2_0)(res, FARM_PP_ARRAY_ELEM(n, arr)), arr)
# endif
#
# else
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/array/elem.hpp>
# include <farm_pp/preprocessor/array/push_back.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/comparison/not_equal.hpp>
# include <farm_pp/preprocessor/control/deduce_d.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# if FARM_PP_LIMIT_TUPLE == 256
# include <farm_pp/preprocessor/logical/not.hpp>
# endif
# include <farm_pp/preprocessor/tuple/eat.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_ARRAY_REMOVE */
#
# define FARM_PP_ARRAY_REMOVE(array, i) FARM_PP_ARRAY_REMOVE_I(FARM_PP_DEDUCE_D(), array, i)
# define FARM_PP_ARRAY_REMOVE_I(d, array, i) FARM_PP_ARRAY_REMOVE_D(d, array, i)
#
# /* FARM_PP_ARRAY_REMOVE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
# if FARM_PP_LIMIT_TUPLE == 256
#    define FARM_PP_ARRAY_REMOVE_D(d, array, i) FARM_PP_ARRAY_REMOVE_ZERO_D(d, array, i, FARM_PP_NOT(i))
# else
#    define FARM_PP_ARRAY_REMOVE_D(d, array, i) FARM_PP_TUPLE_ELEM(4, 2, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REMOVE_P, FARM_PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# endif
# else
#    define FARM_PP_ARRAY_REMOVE_D(d, array, i) FARM_PP_ARRAY_REMOVE_D_I(d, array, i)
# if FARM_PP_LIMIT_TUPLE == 256
#    define FARM_PP_ARRAY_REMOVE_D_I(d, array, i) FARM_PP_ARRAY_REMOVE_ZERO_D(d, array, i, FARM_PP_NOT(i))
# else
#    define FARM_PP_ARRAY_REMOVE_D_I(d, array, i) FARM_PP_TUPLE_ELEM(4, 2, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REMOVE_P, FARM_PP_ARRAY_REMOVE_O, (0, i, (0, ()), array)))
# endif
# endif
#
# if FARM_PP_LIMIT_TUPLE == 256
# define FARM_PP_ARRAY_REMOVE_ZERO_D(d, array, i, zero) \
         FARM_PP_TUPLE_ELEM(4, 2, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REMOVE_P, FARM_PP_ARRAY_REMOVE_O, \
         (1, i, FARM_PP_IIF(zero,(0, ()),(1, (FARM_PP_ARRAY_ELEM(0,array)))), array)))
# endif
#
# define FARM_PP_ARRAY_REMOVE_P(d, st) FARM_PP_NOT_EQUAL(FARM_PP_TUPLE_ELEM(4, 0, st), FARM_PP_ARRAY_SIZE(FARM_PP_TUPLE_ELEM(4, 3, st)))
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_ARRAY_REMOVE_O(d, st) FARM_PP_ARRAY_REMOVE_O_I st
# else
#    define FARM_PP_ARRAY_REMOVE_O(d, st) FARM_PP_ARRAY_REMOVE_O_I(FARM_PP_TUPLE_ELEM(4, 0, st), FARM_PP_TUPLE_ELEM(4, 1, st), FARM_PP_TUPLE_ELEM(4, 2, st), FARM_PP_TUPLE_ELEM(4, 3, st))
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_DMC()
#    define FARM_PP_ARRAY_REMOVE_O_I(n, i, res, arr) (FARM_PP_INC(n), i, FARM_PP_IIF(FARM_PP_NOT_EQUAL(n, i), FARM_PP_ARRAY_PUSH_BACK, res FARM_PP_TUPLE_EAT_2)(res, FARM_PP_ARRAY_ELEM(n, arr)), arr)
# else
#    define FARM_PP_ARRAY_REMOVE_O_I(n, i, res, arr) (FARM_PP_INC(n), i, FARM_PP_IIF(FARM_PP_NOT_EQUAL(n, i), FARM_PP_ARRAY_PUSH_BACK, FARM_PP_TUPLE_ELEM_2_0)(res, FARM_PP_ARRAY_ELEM(n, arr)), arr)
# endif
#
# endif
#
# endif
