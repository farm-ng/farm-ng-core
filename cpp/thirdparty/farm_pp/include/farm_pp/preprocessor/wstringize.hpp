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
# ifndef FARM_PP_PREPROCESSOR_WSTRINGIZE_HPP
# define FARM_PP_PREPROCESSOR_WSTRINGIZE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_WSTRINGIZE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_WSTRINGIZE(text) FARM_PP_WSTRINGIZE_I(text)
# else
#    define FARM_PP_WSTRINGIZE(text) FARM_PP_WSTRINGIZE_OO((text))
#    define FARM_PP_WSTRINGIZE_OO(par) FARM_PP_WSTRINGIZE_I ## par
# endif
#
# define FARM_PP_WSTRINGIZE_I(...) FARM_PP_WSTRINGIZE_II(#__VA_ARGS__)
# define FARM_PP_WSTRINGIZE_II(str) L ## str
#
# endif
