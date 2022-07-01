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
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_ENUM_SHIFTED_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/debug/error.hpp>
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
# include <farm_pp/preprocessor/punctuation/comma_if.hpp>
# include <farm_pp/preprocessor/repetition/repeat.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
#
# /* FARM_PP_ENUM_SHIFTED */
#
# if 0
#    define FARM_PP_ENUM_SHIFTED(count, macro, data)
# endif
#
# define FARM_PP_ENUM_SHIFTED FARM_PP_CAT(FARM_PP_ENUM_SHIFTED_, FARM_PP_AUTO_REC(FARM_PP_REPEAT_P, 4))
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ENUM_SHIFTED_1(c, m, d) FARM_PP_REPEAT_1(FARM_PP_DEC(c), FARM_PP_ENUM_SHIFTED_M_1, (m, d))
#    define FARM_PP_ENUM_SHIFTED_2(c, m, d) FARM_PP_REPEAT_2(FARM_PP_DEC(c), FARM_PP_ENUM_SHIFTED_M_2, (m, d))
#    define FARM_PP_ENUM_SHIFTED_3(c, m, d) FARM_PP_REPEAT_3(FARM_PP_DEC(c), FARM_PP_ENUM_SHIFTED_M_3, (m, d))
# else
#    define FARM_PP_ENUM_SHIFTED_1(c, m, d) FARM_PP_ENUM_SHIFTED_1_I(c, m, d)
#    define FARM_PP_ENUM_SHIFTED_2(c, m, d) FARM_PP_ENUM_SHIFTED_1_2(c, m, d)
#    define FARM_PP_ENUM_SHIFTED_3(c, m, d) FARM_PP_ENUM_SHIFTED_1_3(c, m, d)
#    define FARM_PP_ENUM_SHIFTED_1_I(c, m, d) FARM_PP_REPEAT_1(FARM_PP_DEC(c), FARM_PP_ENUM_SHIFTED_M_1, (m, d))
#    define FARM_PP_ENUM_SHIFTED_2_I(c, m, d) FARM_PP_REPEAT_2(FARM_PP_DEC(c), FARM_PP_ENUM_SHIFTED_M_2, (m, d))
#    define FARM_PP_ENUM_SHIFTED_3_I(c, m, d) FARM_PP_REPEAT_3(FARM_PP_DEC(c), FARM_PP_ENUM_SHIFTED_M_3, (m, d))
# endif
#
# define FARM_PP_ENUM_SHIFTED_4(c, m, d) FARM_PP_ERROR(0x0003)
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#    define FARM_PP_ENUM_SHIFTED_M_1(z, n, md) FARM_PP_ENUM_SHIFTED_M_1_IM(z, n, FARM_PP_TUPLE_REM_2 md)
#    define FARM_PP_ENUM_SHIFTED_M_2(z, n, md) FARM_PP_ENUM_SHIFTED_M_2_IM(z, n, FARM_PP_TUPLE_REM_2 md)
#    define FARM_PP_ENUM_SHIFTED_M_3(z, n, md) FARM_PP_ENUM_SHIFTED_M_3_IM(z, n, FARM_PP_TUPLE_REM_2 md)
#    define FARM_PP_ENUM_SHIFTED_M_1_IM(z, n, im) FARM_PP_ENUM_SHIFTED_M_1_I(z, n, im)
#    define FARM_PP_ENUM_SHIFTED_M_2_IM(z, n, im) FARM_PP_ENUM_SHIFTED_M_2_I(z, n, im)
#    define FARM_PP_ENUM_SHIFTED_M_3_IM(z, n, im) FARM_PP_ENUM_SHIFTED_M_3_I(z, n, im)
# else
#    define FARM_PP_ENUM_SHIFTED_M_1(z, n, md) FARM_PP_ENUM_SHIFTED_M_1_I(z, n, FARM_PP_TUPLE_ELEM(2, 0, md), FARM_PP_TUPLE_ELEM(2, 1, md))
#    define FARM_PP_ENUM_SHIFTED_M_2(z, n, md) FARM_PP_ENUM_SHIFTED_M_2_I(z, n, FARM_PP_TUPLE_ELEM(2, 0, md), FARM_PP_TUPLE_ELEM(2, 1, md))
#    define FARM_PP_ENUM_SHIFTED_M_3(z, n, md) FARM_PP_ENUM_SHIFTED_M_3_I(z, n, FARM_PP_TUPLE_ELEM(2, 0, md), FARM_PP_TUPLE_ELEM(2, 1, md))
# endif
#
# define FARM_PP_ENUM_SHIFTED_M_1_I(z, n, m, d) FARM_PP_COMMA_IF(n) m(z, FARM_PP_INC(n), d)
# define FARM_PP_ENUM_SHIFTED_M_2_I(z, n, m, d) FARM_PP_COMMA_IF(n) m(z, FARM_PP_INC(n), d)
# define FARM_PP_ENUM_SHIFTED_M_3_I(z, n, m, d) FARM_PP_COMMA_IF(n) m(z, FARM_PP_INC(n), d)
#
# endif
