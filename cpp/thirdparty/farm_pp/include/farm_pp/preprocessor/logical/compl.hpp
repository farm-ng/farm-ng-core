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
# ifndef FARM_PP_PREPROCESSOR_LOGICAL_COMPL_HPP
# define FARM_PP_PREPROCESSOR_LOGICAL_COMPL_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_COMPL */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_COMPL(x) FARM_PP_COMPL_I(x)
# else
#    define FARM_PP_COMPL(x) FARM_PP_COMPL_OO((x))
#    define FARM_PP_COMPL_OO(par) FARM_PP_COMPL_I ## par
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_COMPL_I(x) FARM_PP_COMPL_ ## x
# else
#    define FARM_PP_COMPL_I(x) FARM_PP_COMPL_ID(FARM_PP_COMPL_ ## x)
#    define FARM_PP_COMPL_ID(id) id
# endif
#
# define FARM_PP_COMPL_0 1
# define FARM_PP_COMPL_1 0
#
# endif
