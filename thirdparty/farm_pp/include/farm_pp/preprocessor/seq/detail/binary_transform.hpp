# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2011.                                  *
#  *     Distributed under the Boost Software License, Version 1.0. (See      *
#  *     accompanying file LICENSE_1_0.txt or copy at                         *
#  *     http://www.boost.org/LICENSE_1_0.txt)                                *
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_SEQ_DETAIL_BINARY_TRANSFORM_HPP
# define FARM_PP_PREPROCESSOR_SEQ_DETAIL_BINARY_TRANSFORM_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
# include <farm_pp/preprocessor/tuple/eat.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# include <farm_pp/preprocessor/variadic/detail/is_single_return.hpp>
#
# /* FARM_PP_SEQ_BINARY_TRANSFORM */
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_MSVC()
#    define FARM_PP_SEQ_BINARY_TRANSFORM(seq) FARM_PP_SEQ_BINARY_TRANSFORM_I(, seq)
#    define FARM_PP_SEQ_BINARY_TRANSFORM_I(p, seq) FARM_PP_SEQ_BINARY_TRANSFORM_II(p ## seq)
#    define FARM_PP_SEQ_BINARY_TRANSFORM_II(seq) FARM_PP_SEQ_BINARY_TRANSFORM_III(seq)
#    define FARM_PP_SEQ_BINARY_TRANSFORM_III(seq) FARM_PP_CAT(FARM_PP_SEQ_BINARY_TRANSFORM_A seq, 0)
# else
#    define FARM_PP_SEQ_BINARY_TRANSFORM(seq) FARM_PP_CAT(FARM_PP_SEQ_BINARY_TRANSFORM_A seq, 0)
# endif
# if FARM_PP_VARIADICS_MSVC
#    define FARM_PP_SEQ_BINARY_TRANSFORM_REM(data) data
#    define FARM_PP_SEQ_BINARY_TRANSFORM_A(...) (FARM_PP_SEQ_BINARY_TRANSFORM_REM, __VA_ARGS__)() FARM_PP_SEQ_BINARY_TRANSFORM_B
#    define FARM_PP_SEQ_BINARY_TRANSFORM_B(...) (FARM_PP_SEQ_BINARY_TRANSFORM_REM, __VA_ARGS__)() FARM_PP_SEQ_BINARY_TRANSFORM_A
# else
#    define FARM_PP_SEQ_BINARY_TRANSFORM_A(...) (FARM_PP_REM, __VA_ARGS__)() FARM_PP_SEQ_BINARY_TRANSFORM_B
#    define FARM_PP_SEQ_BINARY_TRANSFORM_B(...) (FARM_PP_REM, __VA_ARGS__)() FARM_PP_SEQ_BINARY_TRANSFORM_A
# endif
# define FARM_PP_SEQ_BINARY_TRANSFORM_A0 (FARM_PP_EAT, ?)
# define FARM_PP_SEQ_BINARY_TRANSFORM_B0 (FARM_PP_EAT, ?)
#
# endif
