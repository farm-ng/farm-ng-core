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
# ifndef FARM_PP_PREPROCESSOR_LOGICAL_BITNOR_HPP
# define FARM_PP_PREPROCESSOR_LOGICAL_BITNOR_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_BITNOR */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_BITNOR(x, y) FARM_PP_BITNOR_I(x, y)
# else
#    define FARM_PP_BITNOR(x, y) FARM_PP_BITNOR_OO((x, y))
#    define FARM_PP_BITNOR_OO(par) FARM_PP_BITNOR_I ## par
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_BITNOR_I(x, y) FARM_PP_BITNOR_ ## x ## y
# else
#    define FARM_PP_BITNOR_I(x, y) FARM_PP_BITNOR_ID(FARM_PP_BITNOR_ ## x ## y)
#    define FARM_PP_BITNOR_ID(id) id
# endif
#
# define FARM_PP_BITNOR_00 1
# define FARM_PP_BITNOR_01 0
# define FARM_PP_BITNOR_10 0
# define FARM_PP_BITNOR_11 0
#
# endif
