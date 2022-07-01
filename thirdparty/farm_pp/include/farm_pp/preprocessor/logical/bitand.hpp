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
# ifndef FARM_PP_PREPROCESSOR_LOGICAL_BITAND_HPP
# define FARM_PP_PREPROCESSOR_LOGICAL_BITAND_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_BITAND */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_BITAND(x, y) FARM_PP_BITAND_I(x, y)
# else
#    define FARM_PP_BITAND(x, y) FARM_PP_BITAND_OO((x, y))
#    define FARM_PP_BITAND_OO(par) FARM_PP_BITAND_I ## par
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_BITAND_I(x, y) FARM_PP_BITAND_ ## x ## y
# else
#    define FARM_PP_BITAND_I(x, y) FARM_PP_BITAND_ID(FARM_PP_BITAND_ ## x ## y)
#    define FARM_PP_BITAND_ID(res) res
# endif
#
# define FARM_PP_BITAND_00 0
# define FARM_PP_BITAND_01 0
# define FARM_PP_BITAND_10 0
# define FARM_PP_BITAND_11 1
#
# endif
