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
# ifndef FARM_PP_PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
# define FARM_PP_PREPROCESSOR_PUNCTUATION_COMMA_IF_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/facilities/empty.hpp>
# include <farm_pp/preprocessor/punctuation/comma.hpp>
#
# /* FARM_PP_COMMA_IF */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_COMMA_IF(cond) FARM_PP_IF(cond, FARM_PP_COMMA, FARM_PP_EMPTY)()
# else
#    define FARM_PP_COMMA_IF(cond) FARM_PP_COMMA_IF_I(cond)
#    define FARM_PP_COMMA_IF_I(cond) FARM_PP_IF(cond, FARM_PP_COMMA, FARM_PP_EMPTY)()
# endif
#
# endif
