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
# ifndef FARM_PP_PREPROCESSOR_LOGICAL_NOT_HPP
# define FARM_PP_PREPROCESSOR_LOGICAL_NOT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/logical/bool.hpp>
# include <farm_pp/preprocessor/logical/compl.hpp>
#
# /* FARM_PP_NOT */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_NOT(x) FARM_PP_COMPL(FARM_PP_BOOL(x))
# else
#    define FARM_PP_NOT(x) FARM_PP_NOT_I(x)
#    define FARM_PP_NOT_I(x) FARM_PP_COMPL(FARM_PP_BOOL(x))
# endif
#
# endif
