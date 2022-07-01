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
# ifndef FARM_PP_PREPROCESSOR_FACILITIES_EXPAND_HPP
# define FARM_PP_PREPROCESSOR_FACILITIES_EXPAND_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC() && ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_DMC()
#    define FARM_PP_EXPAND(x) FARM_PP_EXPAND_I(x)
# else
#    define FARM_PP_EXPAND(x) FARM_PP_EXPAND_OO((x))
#    define FARM_PP_EXPAND_OO(par) FARM_PP_EXPAND_I ## par
# endif
#
# define FARM_PP_EXPAND_I(x) x
#
# endif
