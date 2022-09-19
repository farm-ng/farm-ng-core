# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.
#  *     (C) Copyright Edward Diener 2011,2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_LIST_TO_ARRAY_HPP
# define FARM_PP_PREPROCESSOR_LIST_TO_ARRAY_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/list/adt.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# if FARM_PP_VARIADICS_MSVC && (_MSC_VER <= 1400)
# include <farm_pp/preprocessor/control/iif.hpp>
# endif
#
# /* FARM_PP_LIST_TO_ARRAY */
#
# if FARM_PP_VARIADICS_MSVC && (_MSC_VER <= 1400)
# define FARM_PP_LIST_TO_ARRAY(list) \
    FARM_PP_IIF \
        ( \
        FARM_PP_LIST_IS_NIL(list), \
        FARM_PP_LIST_TO_ARRAY_VC8ORLESS_EMPTY, \
        FARM_PP_LIST_TO_ARRAY_VC8ORLESS_DO \
        ) \
    (list) \
/**/
# define FARM_PP_LIST_TO_ARRAY_VC8ORLESS_EMPTY(list) (0,())
# define FARM_PP_LIST_TO_ARRAY_VC8ORLESS_DO(list) FARM_PP_LIST_TO_ARRAY_I(FARM_PP_WHILE, list)
# else
# define FARM_PP_LIST_TO_ARRAY(list) FARM_PP_LIST_TO_ARRAY_I(FARM_PP_WHILE, list)
# endif

# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_LIST_TO_ARRAY_I(w, list) \
        FARM_PP_LIST_TO_ARRAY_II(((FARM_PP_TUPLE_REM_CTOR( \
            3, \
            w(FARM_PP_LIST_TO_ARRAY_P, FARM_PP_LIST_TO_ARRAY_O, (list, 1, (~))) \
        )))) \
        /**/
#    define FARM_PP_LIST_TO_ARRAY_II(p) FARM_PP_LIST_TO_ARRAY_II_B(p)
#    define FARM_PP_LIST_TO_ARRAY_II_B(p) FARM_PP_LIST_TO_ARRAY_II_C ## p
#    define FARM_PP_LIST_TO_ARRAY_II_C(p) FARM_PP_LIST_TO_ARRAY_III p
# else
#    define FARM_PP_LIST_TO_ARRAY_I(w, list) \
        FARM_PP_LIST_TO_ARRAY_II(FARM_PP_TUPLE_REM_CTOR( \
            3, \
            w(FARM_PP_LIST_TO_ARRAY_P, FARM_PP_LIST_TO_ARRAY_O, (list, 1, (~))) \
        )) \
        /**/
#    define FARM_PP_LIST_TO_ARRAY_II(im) FARM_PP_LIST_TO_ARRAY_III(im)
# endif
# define FARM_PP_LIST_TO_ARRAY_III(list, size, tuple) (FARM_PP_DEC(size), FARM_PP_LIST_TO_ARRAY_IV tuple)
# define FARM_PP_LIST_TO_ARRAY_IV(_, ...) (__VA_ARGS__)
# define FARM_PP_LIST_TO_ARRAY_P(d, state) FARM_PP_LIST_IS_CONS(FARM_PP_TUPLE_ELEM(3, 0, state))
# define FARM_PP_LIST_TO_ARRAY_O(d, state) FARM_PP_LIST_TO_ARRAY_O_I state
# define FARM_PP_LIST_TO_ARRAY_O_I(list, size, tuple) (FARM_PP_LIST_REST(list), FARM_PP_INC(size), (FARM_PP_TUPLE_REM(size) tuple, FARM_PP_LIST_FIRST(list)))
#
# /* FARM_PP_LIST_TO_ARRAY_D */
#
# if FARM_PP_VARIADICS_MSVC && (_MSC_VER <= 1400)
# define FARM_PP_LIST_TO_ARRAY_D(d, list) \
    FARM_PP_IIF \
        ( \
        FARM_PP_LIST_IS_NIL(list), \
        FARM_PP_LIST_TO_ARRAY_D_VC8ORLESS_EMPTY, \
        FARM_PP_LIST_TO_ARRAY_D_VC8ORLESS_DO \
        ) \
    (d, list) \
/**/
# define FARM_PP_LIST_TO_ARRAY_D_VC8ORLESS_EMPTY(d, list) (0,())
# define FARM_PP_LIST_TO_ARRAY_D_VC8ORLESS_DO(d, list) FARM_PP_LIST_TO_ARRAY_I(FARM_PP_WHILE_ ## d, list)
# else
# define FARM_PP_LIST_TO_ARRAY_D(d, list) FARM_PP_LIST_TO_ARRAY_I(FARM_PP_WHILE_ ## d, list)
# endif
#
# endif /* FARM_PP_PREPROCESSOR_LIST_TO_ARRAY_HPP */
