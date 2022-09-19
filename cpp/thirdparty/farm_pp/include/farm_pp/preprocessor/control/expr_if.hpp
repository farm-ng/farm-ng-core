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
# ifndef FARM_PP_PREPROCESSOR_CONTROL_EXPR_IF_HPP
# define FARM_PP_PREPROCESSOR_CONTROL_EXPR_IF_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/expr_iif.hpp>
# include <farm_pp/preprocessor/logical/bool.hpp>
#
# /* FARM_PP_EXPR_IF */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_EXPR_IF(cond, expr) FARM_PP_EXPR_IIF(FARM_PP_BOOL(cond), expr)
# else
#    define FARM_PP_EXPR_IF(cond, expr) FARM_PP_EXPR_IF_I(cond, expr)
#    define FARM_PP_EXPR_IF_I(cond, expr) FARM_PP_EXPR_IIF(FARM_PP_BOOL(cond), expr)
# endif
#
# endif
