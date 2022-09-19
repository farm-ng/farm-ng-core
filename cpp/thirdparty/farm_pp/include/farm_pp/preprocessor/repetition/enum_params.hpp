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
# ifndef FARM_PP_PREPROCESSOR_REPETITION_ENUM_PARAMS_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_ENUM_PARAMS_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/punctuation/comma_if.hpp>
# include <farm_pp/preprocessor/repetition/repeat.hpp>
#
# /* FARM_PP_ENUM_PARAMS */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ENUM_PARAMS(count, param) FARM_PP_REPEAT(count, FARM_PP_ENUM_PARAMS_M, param)
# else
#    define FARM_PP_ENUM_PARAMS(count, param) FARM_PP_ENUM_PARAMS_I(count, param)
#    define FARM_PP_ENUM_PARAMS_I(count, param) FARM_PP_REPEAT(count, FARM_PP_ENUM_PARAMS_M, param)
# endif
#
# define FARM_PP_ENUM_PARAMS_M(z, n, param) FARM_PP_COMMA_IF(n) param ## n
#
# /* FARM_PP_ENUM_PARAMS_Z */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ENUM_PARAMS_Z(z, count, param) FARM_PP_REPEAT_ ## z(count, FARM_PP_ENUM_PARAMS_M, param)
# else
#    define FARM_PP_ENUM_PARAMS_Z(z, count, param) FARM_PP_ENUM_PARAMS_Z_I(z, count, param)
#    define FARM_PP_ENUM_PARAMS_Z_I(z, count, param) FARM_PP_REPEAT_ ## z(count, FARM_PP_ENUM_PARAMS_M, param)
# endif
#
# endif
