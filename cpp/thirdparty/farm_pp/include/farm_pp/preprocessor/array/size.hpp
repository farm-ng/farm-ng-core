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
# ifndef FARM_PP_PREPROCESSOR_ARRAY_SIZE_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_SIZE_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/tuple/elem.hpp>
#
# /* FARM_PP_ARRAY_SIZE */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ARRAY_SIZE(array) FARM_PP_TUPLE_ELEM(2, 0, array)
# else
#    define FARM_PP_ARRAY_SIZE(array) FARM_PP_ARRAY_SIZE_I(array)
#    define FARM_PP_ARRAY_SIZE_I(array) FARM_PP_ARRAY_SIZE_II array
#    define FARM_PP_ARRAY_SIZE_II(size, data) size
# endif
#
# endif
