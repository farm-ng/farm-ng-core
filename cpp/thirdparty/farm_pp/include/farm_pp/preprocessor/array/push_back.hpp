# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_ARRAY_PUSH_BACK_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_PUSH_BACK_HPP
#
# include <farm_pp/preprocessor/arithmetic/inc.hpp>
# include <farm_pp/preprocessor/array/data.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/punctuation/comma_if.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# include <farm_pp/preprocessor/array/detail/get_data.hpp>
#
# /* FARM_PP_ARRAY_PUSH_BACK */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ARRAY_PUSH_BACK(array, elem) FARM_PP_ARRAY_PUSH_BACK_I(FARM_PP_ARRAY_SIZE(array), FARM_PP_ARRAY_DATA(array), elem)
# else
#    define FARM_PP_ARRAY_PUSH_BACK(array, elem) FARM_PP_ARRAY_PUSH_BACK_D(array, elem)
#    define FARM_PP_ARRAY_PUSH_BACK_D(array, elem) FARM_PP_ARRAY_PUSH_BACK_I(FARM_PP_ARRAY_SIZE(array), FARM_PP_ARRAY_DATA(array), elem)
# endif
#
# define FARM_PP_ARRAY_PUSH_BACK_I(size, data, elem) (FARM_PP_INC(size), (FARM_PP_ARRAY_DETAIL_GET_DATA(size,data) FARM_PP_COMMA_IF(size) elem))
#
# endif
