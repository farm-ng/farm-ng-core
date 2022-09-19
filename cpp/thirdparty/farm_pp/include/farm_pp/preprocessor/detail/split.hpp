# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# ifndef FARM_PP_PREPROCESSOR_DETAIL_SPLIT_HPP
# define FARM_PP_PREPROCESSOR_DETAIL_SPLIT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_SPLIT */
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_SPLIT(n, im) FARM_PP_SPLIT_I((n, im))
#    define FARM_PP_SPLIT_I(par) FARM_PP_SPLIT_II ## par
#    define FARM_PP_SPLIT_II(n, a, b) FARM_PP_SPLIT_ ## n(a, b)
# elif FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_SPLIT(n, im) FARM_PP_SPLIT_I(n((im)))
#    define FARM_PP_SPLIT_I(n) FARM_PP_SPLIT_ID(FARM_PP_SPLIT_II_ ## n)
#    define FARM_PP_SPLIT_II_0(s) FARM_PP_SPLIT_ID(FARM_PP_SPLIT_0 s)
#    define FARM_PP_SPLIT_II_1(s) FARM_PP_SPLIT_ID(FARM_PP_SPLIT_1 s)
#    define FARM_PP_SPLIT_ID(id) id
# else
#    define FARM_PP_SPLIT(n, im) FARM_PP_SPLIT_I(n)(im)
#    define FARM_PP_SPLIT_I(n) FARM_PP_SPLIT_ ## n
# endif
#
# define FARM_PP_SPLIT_0(a, b) a
# define FARM_PP_SPLIT_1(a, b) b
#
# endif
