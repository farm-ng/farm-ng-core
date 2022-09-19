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
# ifndef FARM_PP_PREPROCESSOR_LOGICAL_BITOR_HPP
# define FARM_PP_PREPROCESSOR_LOGICAL_BITOR_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_BITOR */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_BITOR(x, y) FARM_PP_BITOR_I(x, y)
# else
#    define FARM_PP_BITOR(x, y) FARM_PP_BITOR_OO((x, y))
#    define FARM_PP_BITOR_OO(par) FARM_PP_BITOR_I ## par
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_BITOR_I(x, y) FARM_PP_BITOR_ ## x ## y
# else
#    define FARM_PP_BITOR_I(x, y) FARM_PP_BITOR_ID(FARM_PP_BITOR_ ## x ## y)
#    define FARM_PP_BITOR_ID(id) id
# endif
#
# define FARM_PP_BITOR_00 0
# define FARM_PP_BITOR_01 1
# define FARM_PP_BITOR_10 1
# define FARM_PP_BITOR_11 1
#
# endif
