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
# ifndef FARM_PP_PREPROCESSOR_ARRAY_REPLACE_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_REPLACE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/array/elem.hpp>
# include <farm_pp/preprocessor/array/push_back.hpp>
# include <farm_pp/preprocessor/comparison/not_equal.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/deduce_d.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_ARRAY_REPLACE */
#
# define FARM_PP_ARRAY_REPLACE(array, i, elem) FARM_PP_ARRAY_REPLACE_I(FARM_PP_DEDUCE_D(), array, i, elem)
# define FARM_PP_ARRAY_REPLACE_I(d, array, i, elem) FARM_PP_ARRAY_REPLACE_D(d, array, i, elem)
#
# /* FARM_PP_ARRAY_REPLACE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ARRAY_REPLACE_D(d, array, i, elem) FARM_PP_TUPLE_ELEM(5, 3, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REPLACE_P, FARM_PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# else
#    define FARM_PP_ARRAY_REPLACE_D(d, array, i, elem) FARM_PP_ARRAY_REPLACE_D_I(d, array, i, elem)
#    define FARM_PP_ARRAY_REPLACE_D_I(d, array, i, elem) FARM_PP_TUPLE_ELEM(5, 3, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REPLACE_P, FARM_PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# endif
#
# define FARM_PP_ARRAY_REPLACE_P(d, state) FARM_PP_NOT_EQUAL(FARM_PP_TUPLE_ELEM(5, 0, state), FARM_PP_ARRAY_SIZE(FARM_PP_TUPLE_ELEM(5, 4, state)))
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_ARRAY_REPLACE_O(d, state) FARM_PP_ARRAY_REPLACE_O_I state
# else
#    define FARM_PP_ARRAY_REPLACE_O(d, state) FARM_PP_ARRAY_REPLACE_O_I(FARM_PP_TUPLE_ELEM(5, 0, state), FARM_PP_TUPLE_ELEM(5, 1, state), FARM_PP_TUPLE_ELEM(5, 2, state), FARM_PP_TUPLE_ELEM(5, 3, state), FARM_PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define FARM_PP_ARRAY_REPLACE_O_I(n, i, elem, res, arr) (FARM_PP_INC(n), i, elem, FARM_PP_ARRAY_PUSH_BACK(res, FARM_PP_IIF(FARM_PP_NOT_EQUAL(n, i), FARM_PP_ARRAY_ELEM(n, arr), elem)), arr)
#
# else
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/array/elem.hpp>
# include <farm_pp/preprocessor/array/push_back.hpp>
# include <farm_pp/preprocessor/comparison/not_equal.hpp>
# include <farm_pp/preprocessor/control/deduce_d.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# if FARM_PP_LIMIT_TUPLE == 256
# include <farm_pp/preprocessor/logical/not.hpp>
# endif
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_ARRAY_REPLACE */
#
# define FARM_PP_ARRAY_REPLACE(array, i, elem) FARM_PP_ARRAY_REPLACE_I(FARM_PP_DEDUCE_D(), array, i, elem)
# define FARM_PP_ARRAY_REPLACE_I(d, array, i, elem) FARM_PP_ARRAY_REPLACE_D(d, array, i, elem)
#
# /* FARM_PP_ARRAY_REPLACE_D */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
# if FARM_PP_LIMIT_TUPLE == 256
#    define FARM_PP_ARRAY_REPLACE_D(d, array, i, elem) FARM_PP_ARRAY_REPLACE_ZERO_D(d, array, i, elem, FARM_PP_NOT(i))
# else
#    define FARM_PP_ARRAY_REPLACE_D(d, array, i, elem) FARM_PP_TUPLE_ELEM(5, 3, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REPLACE_P, FARM_PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# endif
# else
#    define FARM_PP_ARRAY_REPLACE_D(d, array, i, elem) FARM_PP_ARRAY_REPLACE_D_I(d, array, i, elem)
# if FARM_PP_LIMIT_TUPLE == 256
#    define FARM_PP_ARRAY_REPLACE_D_I(d, array, i, elem) FARM_PP_ARRAY_REPLACE_ZERO_D(d, array, i, elem, FARM_PP_NOT(i))
# else
#    define FARM_PP_ARRAY_REPLACE_D_I(d, array, i, elem) FARM_PP_TUPLE_ELEM(5, 3, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REPLACE_P, FARM_PP_ARRAY_REPLACE_O, (0, i, elem, (0, ()), array)))
# endif
# endif
#
# if FARM_PP_LIMIT_TUPLE == 256
# define FARM_PP_ARRAY_REPLACE_ZERO_D(d, array, i, elem, zero) \
         FARM_PP_TUPLE_ELEM(5, 3, FARM_PP_WHILE_ ## d(FARM_PP_ARRAY_REPLACE_P, FARM_PP_ARRAY_REPLACE_O, \
         (1, i, elem, FARM_PP_IIF(zero,(1, (elem)),(1, (FARM_PP_ARRAY_ELEM(0,array)))), array)))
# endif
#
# define FARM_PP_ARRAY_REPLACE_P(d, state) FARM_PP_NOT_EQUAL(FARM_PP_TUPLE_ELEM(5, 0, state), FARM_PP_ARRAY_SIZE(FARM_PP_TUPLE_ELEM(5, 4, state)))
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_ARRAY_REPLACE_O(d, state) FARM_PP_ARRAY_REPLACE_O_I state
# else
#    define FARM_PP_ARRAY_REPLACE_O(d, state) FARM_PP_ARRAY_REPLACE_O_I(FARM_PP_TUPLE_ELEM(5, 0, state), FARM_PP_TUPLE_ELEM(5, 1, state), FARM_PP_TUPLE_ELEM(5, 2, state), FARM_PP_TUPLE_ELEM(5, 3, state), FARM_PP_TUPLE_ELEM(5, 4, state))
# endif
#
# define FARM_PP_ARRAY_REPLACE_O_I(n, i, elem, res, arr) (FARM_PP_INC(n), i, elem, FARM_PP_ARRAY_PUSH_BACK(res, FARM_PP_IIF(FARM_PP_NOT_EQUAL(n, i), FARM_PP_ARRAY_ELEM(n, arr), elem)), arr)
#
# endif
#
# endif
