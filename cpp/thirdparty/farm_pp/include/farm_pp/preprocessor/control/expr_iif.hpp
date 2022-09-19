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
# ifndef FARM_PP_PREPROCESSOR_CONTROL_EXPR_IIF_HPP
# define FARM_PP_PREPROCESSOR_CONTROL_EXPR_IIF_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_EXPR_IIF */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_EXPR_IIF(bit, expr) FARM_PP_EXPR_IIF_I(bit, expr)
# else
#    define FARM_PP_EXPR_IIF(bit, expr) FARM_PP_EXPR_IIF_OO((bit, expr))
#    define FARM_PP_EXPR_IIF_OO(par) FARM_PP_EXPR_IIF_I ## par
# endif
#
# define FARM_PP_EXPR_IIF_I(bit, expr) FARM_PP_EXPR_IIF_ ## bit(expr)
#
# define FARM_PP_EXPR_IIF_0(expr)
# define FARM_PP_EXPR_IIF_1(expr) expr
#
# endif
