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
# ifndef FARM_PP_PREPROCESSOR_ARRAY_POP_BACK_HPP
# define FARM_PP_PREPROCESSOR_ARRAY_POP_BACK_HPP
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/array/elem.hpp>
# include <farm_pp/preprocessor/array/size.hpp>
# include <farm_pp/preprocessor/repetition/enum.hpp>
# include <farm_pp/preprocessor/repetition/deduce_z.hpp>
#
# /* FARM_PP_ARRAY_POP_BACK */
#
# define FARM_PP_ARRAY_POP_BACK(array) FARM_PP_ARRAY_POP_BACK_Z(FARM_PP_DEDUCE_Z(), array)
#
# /* FARM_PP_ARRAY_POP_BACK_Z */
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    define FARM_PP_ARRAY_POP_BACK_Z(z, array) FARM_PP_ARRAY_POP_BACK_I(z, FARM_PP_ARRAY_SIZE(array), array)
# else
#    define FARM_PP_ARRAY_POP_BACK_Z(z, array) FARM_PP_ARRAY_POP_BACK_Z_D(z, array)
#    define FARM_PP_ARRAY_POP_BACK_Z_D(z, array) FARM_PP_ARRAY_POP_BACK_I(z, FARM_PP_ARRAY_SIZE(array), array)
# endif
#
# define FARM_PP_ARRAY_POP_BACK_I(z, size, array) (FARM_PP_DEC(size), (FARM_PP_ENUM_ ## z(FARM_PP_DEC(size), FARM_PP_ARRAY_POP_BACK_M, array)))
# define FARM_PP_ARRAY_POP_BACK_M(z, n, data) FARM_PP_ARRAY_ELEM(n, data)
#
# endif
