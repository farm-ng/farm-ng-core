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
# ifndef FARM_PP_PREPROCESSOR_REPETITION_ENUM_PARAMS_WITH_A_DEFAULT_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_ENUM_PARAMS_WITH_A_DEFAULT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/facilities/intercept.hpp>
# include <farm_pp/preprocessor/repetition/enum_binary_params.hpp>
#
# /* FARM_PP_ENUM_PARAMS_WITH_A_DEFAULT */
#
# define FARM_PP_ENUM_PARAMS_WITH_A_DEFAULT(count, param, def) FARM_PP_ENUM_BINARY_PARAMS(count, param, = def FARM_PP_INTERCEPT)
#
# endif
