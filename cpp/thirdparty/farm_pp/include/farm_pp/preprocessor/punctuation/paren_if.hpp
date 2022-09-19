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
# ifndef FARM_PP_PREPROCESSOR_PUNCTUATION_PAREN_IF_HPP
# define FARM_PP_PREPROCESSOR_PUNCTUATION_PAREN_IF_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/control/if.hpp>
# include <farm_pp/preprocessor/facilities/empty.hpp>
# include <farm_pp/preprocessor/punctuation/paren.hpp>
#
# /* FARM_PP_LPAREN_IF */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_LPAREN_IF(cond) FARM_PP_IF(cond, FARM_PP_LPAREN, FARM_PP_EMPTY)()
# else
#    define FARM_PP_LPAREN_IF(cond) FARM_PP_LPAREN_IF_I(cond)
#    define FARM_PP_LPAREN_IF_I(cond) FARM_PP_IF(cond, FARM_PP_LPAREN, FARM_PP_EMPTY)()
# endif
#
# /* FARM_PP_RPAREN_IF */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_RPAREN_IF(cond) FARM_PP_IF(cond, FARM_PP_RPAREN, FARM_PP_EMPTY)()
# else
#    define FARM_PP_RPAREN_IF(cond) FARM_PP_RPAREN_IF_I(cond)
#    define FARM_PP_RPAREN_IF_I(cond) FARM_PP_IF(cond, FARM_PP_RPAREN, FARM_PP_EMPTY)()
# endif
#
# endif
