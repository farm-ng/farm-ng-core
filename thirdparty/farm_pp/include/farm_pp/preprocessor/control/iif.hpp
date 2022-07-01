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
# ifndef FARM_PP_PREPROCESSOR_CONTROL_IIF_HPP
# define FARM_PP_PREPROCESSOR_CONTROL_IIF_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MWCC()
#    define FARM_PP_IIF(bit, t, f) FARM_PP_IIF_I(bit, t, f)
# else
#    define FARM_PP_IIF(bit, t, f) FARM_PP_IIF_OO((bit, t, f))
#    define FARM_PP_IIF_OO(par) FARM_PP_IIF_I ## par
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_IIF_I(bit, t, f) FARM_PP_IIF_ ## bit(t, f)
# else
#    define FARM_PP_IIF_I(bit, t, f) FARM_PP_IIF_II(FARM_PP_IIF_ ## bit(t, f))
#    define FARM_PP_IIF_II(id) id
# endif
#
# define FARM_PP_IIF_0(t, f) f
# define FARM_PP_IIF_1(t, f) t
#
# endif
