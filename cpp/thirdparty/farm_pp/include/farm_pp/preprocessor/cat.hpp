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
# ifndef FARM_PP_PREPROCESSOR_CAT_HPP
# define FARM_PP_PREPROCESSOR_CAT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_CAT */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_CAT(a, b) FARM_PP_CAT_I(a, b)
# else
#    define FARM_PP_CAT(a, b) FARM_PP_CAT_OO((a, b))
#    define FARM_PP_CAT_OO(par) FARM_PP_CAT_I ## par
# endif
#
# if (~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()) || (defined(__INTEL_COMPILER) && __INTEL_COMPILER >= 1700)
#    define FARM_PP_CAT_I(a, b) a ## b
# else
#    define FARM_PP_CAT_I(a, b) FARM_PP_CAT_II(~, a ## b)
#    define FARM_PP_CAT_II(p, res) res
# endif
#
# endif
