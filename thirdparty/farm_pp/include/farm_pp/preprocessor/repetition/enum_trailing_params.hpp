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
# ifndef FARM_PP_PREPROCESSOR_REPETITION_ENUM_TRAILING_PARAMS_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_ENUM_TRAILING_PARAMS_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/repetition/repeat.hpp>
#
# /* FARM_PP_ENUM_TRAILING_PARAMS */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ENUM_TRAILING_PARAMS(count, param) FARM_PP_REPEAT(count, FARM_PP_ENUM_TRAILING_PARAMS_M, param)
# else
#    define FARM_PP_ENUM_TRAILING_PARAMS(count, param) FARM_PP_ENUM_TRAILING_PARAMS_I(count, param)
#    define FARM_PP_ENUM_TRAILING_PARAMS_I(count, param) FARM_PP_REPEAT(count, FARM_PP_ENUM_TRAILING_PARAMS_M, param)
# endif
#
# define FARM_PP_ENUM_TRAILING_PARAMS_M(z, n, param) , param ## n
#
# /* FARM_PP_ENUM_TRAILING_PARAMS_Z */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ENUM_TRAILING_PARAMS_Z(z, count, param) FARM_PP_REPEAT_ ## z(count, FARM_PP_ENUM_TRAILING_PARAMS_M, param)
# else
#    define FARM_PP_ENUM_TRAILING_PARAMS_Z(z, count, param) FARM_PP_ENUM_TRAILING_PARAMS_Z_I(z, count, param)
#    define FARM_PP_ENUM_TRAILING_PARAMS_Z_I(z, count, param) FARM_PP_REPEAT_ ## z(count, FARM_PP_ENUM_TRAILING_PARAMS_M, param)
# endif
#
# endif
