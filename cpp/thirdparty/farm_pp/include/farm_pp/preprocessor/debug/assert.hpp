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
# ifndef FARM_PP_PREPROCESSOR_DEBUG_ASSERT_HPP
# define FARM_PP_PREPROCESSOR_DEBUG_ASSERT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/expr_iif.hpp>
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/logical/not.hpp>
# include <farm_pp/preprocessor/tuple/eat.hpp>
#
# /* FARM_PP_ASSERT */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ASSERT FARM_PP_ASSERT_D
# else
#    define FARM_PP_ASSERT(cond) FARM_PP_ASSERT_D(cond)
# endif
#
# define FARM_PP_ASSERT_D(cond) FARM_PP_IIF(FARM_PP_NOT(cond), FARM_PP_ASSERT_ERROR, FARM_PP_TUPLE_EAT_1)(...)
# define FARM_PP_ASSERT_ERROR(x, y, z)
#
# /* FARM_PP_ASSERT_MSG */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ASSERT_MSG FARM_PP_ASSERT_MSG_D
# else
#    define FARM_PP_ASSERT_MSG(cond, msg) FARM_PP_ASSERT_MSG_D(cond, msg)
# endif
#
# define FARM_PP_ASSERT_MSG_D(cond, msg) FARM_PP_EXPR_IIF(FARM_PP_NOT(cond), msg)
#
# endif
