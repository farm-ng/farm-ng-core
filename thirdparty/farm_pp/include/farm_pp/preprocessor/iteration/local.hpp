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
# ifndef FARM_PP_PREPROCESSOR_ITERATION_LOCAL_HPP
# define FARM_PP_PREPROCESSOR_ITERATION_LOCAL_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/slot/slot.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_LOCAL_ITERATE */
#
# define FARM_PP_LOCAL_ITERATE() <farm_pp/preprocessor/iteration/detail/local.hpp>
#
# define FARM_PP_LOCAL_C(n) (FARM_PP_LOCAL_S) <= n && (FARM_PP_LOCAL_F) >= n
# define FARM_PP_LOCAL_R(n) (FARM_PP_LOCAL_F) <= n && (FARM_PP_LOCAL_S) >= n
#
# endif
