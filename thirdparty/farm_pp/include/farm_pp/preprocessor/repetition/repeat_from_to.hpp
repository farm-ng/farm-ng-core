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
# ifndef FARM_PP_PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_REPEAT_FROM_TO_HPP
#
# include <farm_pp/preprocessor/arithmetic/add.hpp>
# include <farm_pp/preprocessor/arithmetic/sub.hpp>
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/debug/error.hpp>
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
# include <farm_pp/preprocessor/repetition/repeat.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_REPEAT_FROM_TO */
#
# if 0
#    define FARM_PP_REPEAT_FROM_TO(first, last, macro, data)
# endif
#
# define FARM_PP_REPEAT_FROM_TO FARM_PP_CAT(FARM_PP_REPEAT_FROM_TO_, FARM_PP_AUTO_REC(FARM_PP_REPEAT_P, 4))
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# define FARM_PP_REPEAT_FROM_TO_1(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_1(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_2(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_2(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_3(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_3(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256), f, l, m, dt)
#
# else
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/config/limits.hpp>
#
# if FARM_PP_LIMIT_REPEAT == 256
# define FARM_PP_REPEAT_FROM_TO_1(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_1(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256)), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_2(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_2(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256)), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_3(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_3(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256)), f, l, m, dt)
# elif FARM_PP_LIMIT_REPEAT == 512
# define FARM_PP_REPEAT_FROM_TO_1(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_1(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 512)), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_2(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_2(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 512)), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_3(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_3(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 512)), f, l, m, dt)
# elif FARM_PP_LIMIT_REPEAT == 1024
# define FARM_PP_REPEAT_FROM_TO_1(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_1(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 1024)), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_2(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_2(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 1024)), f, l, m, dt)
# define FARM_PP_REPEAT_FROM_TO_3(f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_3(FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 1024)), f, l, m, dt)
# else
# error Incorrect value for the FARM_PP_LIMIT_REPEAT limit
# endif
#
# endif
#
# define FARM_PP_REPEAT_FROM_TO_4(f, l, m, dt) FARM_PP_ERROR(0x0003)
#
# define FARM_PP_REPEAT_FROM_TO_1ST FARM_PP_REPEAT_FROM_TO_1
# define FARM_PP_REPEAT_FROM_TO_2ND FARM_PP_REPEAT_FROM_TO_2
# define FARM_PP_REPEAT_FROM_TO_3RD FARM_PP_REPEAT_FROM_TO_3
#
# /* FARM_PP_REPEAT_FROM_TO_D */
#
# if 0
#    define FARM_PP_REPEAT_FROM_TO_D(d, first, last, macro, data)
# endif
#
# define FARM_PP_REPEAT_FROM_TO_D FARM_PP_CAT(FARM_PP_REPEAT_FROM_TO_D_, FARM_PP_AUTO_REC(FARM_PP_REPEAT_P, 4))
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) FARM_PP_REPEAT_1(FARM_PP_SUB_D(d, l, f), FARM_PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define FARM_PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) FARM_PP_REPEAT_2(FARM_PP_SUB_D(d, l, f), FARM_PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define FARM_PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) FARM_PP_REPEAT_3(FARM_PP_SUB_D(d, l, f), FARM_PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# else
#    define FARM_PP_REPEAT_FROM_TO_D_1(d, f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt)
#    define FARM_PP_REPEAT_FROM_TO_D_2(d, f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt)
#    define FARM_PP_REPEAT_FROM_TO_D_3(d, f, l, m, dt) FARM_PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt)
#    define FARM_PP_REPEAT_FROM_TO_D_1_I(d, f, l, m, dt) FARM_PP_REPEAT_1(FARM_PP_SUB_D(d, l, f), FARM_PP_REPEAT_FROM_TO_M_1, (d, f, m, dt))
#    define FARM_PP_REPEAT_FROM_TO_D_2_I(d, f, l, m, dt) FARM_PP_REPEAT_2(FARM_PP_SUB_D(d, l, f), FARM_PP_REPEAT_FROM_TO_M_2, (d, f, m, dt))
#    define FARM_PP_REPEAT_FROM_TO_D_3_I(d, f, l, m, dt) FARM_PP_REPEAT_3(FARM_PP_SUB_D(d, l, f), FARM_PP_REPEAT_FROM_TO_M_3, (d, f, m, dt))
# endif
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_REPEAT_FROM_TO_M_1(z, n, dfmd) FARM_PP_REPEAT_FROM_TO_M_1_IM(z, n, FARM_PP_TUPLE_REM_4 dfmd)
#    define FARM_PP_REPEAT_FROM_TO_M_2(z, n, dfmd) FARM_PP_REPEAT_FROM_TO_M_2_IM(z, n, FARM_PP_TUPLE_REM_4 dfmd)
#    define FARM_PP_REPEAT_FROM_TO_M_3(z, n, dfmd) FARM_PP_REPEAT_FROM_TO_M_3_IM(z, n, FARM_PP_TUPLE_REM_4 dfmd)
#    define FARM_PP_REPEAT_FROM_TO_M_1_IM(z, n, im) FARM_PP_REPEAT_FROM_TO_M_1_I(z, n, im)
#    define FARM_PP_REPEAT_FROM_TO_M_2_IM(z, n, im) FARM_PP_REPEAT_FROM_TO_M_2_I(z, n, im)
#    define FARM_PP_REPEAT_FROM_TO_M_3_IM(z, n, im) FARM_PP_REPEAT_FROM_TO_M_3_I(z, n, im)
# else
#    define FARM_PP_REPEAT_FROM_TO_M_1(z, n, dfmd) FARM_PP_REPEAT_FROM_TO_M_1_I(z, n, FARM_PP_TUPLE_ELEM(4, 0, dfmd), FARM_PP_TUPLE_ELEM(4, 1, dfmd), FARM_PP_TUPLE_ELEM(4, 2, dfmd), FARM_PP_TUPLE_ELEM(4, 3, dfmd))
#    define FARM_PP_REPEAT_FROM_TO_M_2(z, n, dfmd) FARM_PP_REPEAT_FROM_TO_M_2_I(z, n, FARM_PP_TUPLE_ELEM(4, 0, dfmd), FARM_PP_TUPLE_ELEM(4, 1, dfmd), FARM_PP_TUPLE_ELEM(4, 2, dfmd), FARM_PP_TUPLE_ELEM(4, 3, dfmd))
#    define FARM_PP_REPEAT_FROM_TO_M_3(z, n, dfmd) FARM_PP_REPEAT_FROM_TO_M_3_I(z, n, FARM_PP_TUPLE_ELEM(4, 0, dfmd), FARM_PP_TUPLE_ELEM(4, 1, dfmd), FARM_PP_TUPLE_ELEM(4, 2, dfmd), FARM_PP_TUPLE_ELEM(4, 3, dfmd))
# endif
#
# define FARM_PP_REPEAT_FROM_TO_M_1_I(z, n, d, f, m, dt) FARM_PP_REPEAT_FROM_TO_M_1_II(z, FARM_PP_ADD_D(d, n, f), m, dt)
# define FARM_PP_REPEAT_FROM_TO_M_2_I(z, n, d, f, m, dt) FARM_PP_REPEAT_FROM_TO_M_2_II(z, FARM_PP_ADD_D(d, n, f), m, dt)
# define FARM_PP_REPEAT_FROM_TO_M_3_I(z, n, d, f, m, dt) FARM_PP_REPEAT_FROM_TO_M_3_II(z, FARM_PP_ADD_D(d, n, f), m, dt)
#
# define FARM_PP_REPEAT_FROM_TO_M_1_II(z, n, m, dt) m(z, n, dt)
# define FARM_PP_REPEAT_FROM_TO_M_2_II(z, n, m, dt) m(z, n, dt)
# define FARM_PP_REPEAT_FROM_TO_M_3_II(z, n, m, dt) m(z, n, dt)
#
# endif
