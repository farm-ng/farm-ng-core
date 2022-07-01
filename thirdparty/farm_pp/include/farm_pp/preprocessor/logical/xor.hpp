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
# ifndef FARM_PP_PREPROCESSOR_LOGICAL_XOR_HPP
# define FARM_PP_PREPROCESSOR_LOGICAL_XOR_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/logical/bool.hpp>
# include <farm_pp/preprocessor/logical/bitxor.hpp>
#
# /* FARM_PP_XOR */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_XOR(p, q) FARM_PP_BITXOR(FARM_PP_BOOL(p), FARM_PP_BOOL(q))
# else
#    define FARM_PP_XOR(p, q) FARM_PP_XOR_I(p, q)
#    define FARM_PP_XOR_I(p, q) FARM_PP_BITXOR(FARM_PP_BOOL(p), FARM_PP_BOOL(q))
# endif
#
# endif
