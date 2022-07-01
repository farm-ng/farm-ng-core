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
# ifndef FARM_PP_PREPROCESSOR_DEBUG_ERROR_HPP
# define FARM_PP_PREPROCESSOR_DEBUG_ERROR_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/config/config.hpp>
#
# /* FARM_PP_ERROR */
#
# if FARM_PP_CONFIG_ERRORS
#    define FARM_PP_ERROR(code) FARM_PP_CAT(FARM_PP_ERROR_, code)
# endif
#
# define FARM_PP_ERROR_0x0000 FARM_PP_ERROR(0x0000, FARM_PP_INDEX_OUT_OF_BOUNDS)
# define FARM_PP_ERROR_0x0001 FARM_PP_ERROR(0x0001, FARM_PP_WHILE_OVERFLOW)
# define FARM_PP_ERROR_0x0002 FARM_PP_ERROR(0x0002, FARM_PP_FOR_OVERFLOW)
# define FARM_PP_ERROR_0x0003 FARM_PP_ERROR(0x0003, FARM_PP_REPEAT_OVERFLOW)
# define FARM_PP_ERROR_0x0004 FARM_PP_ERROR(0x0004, FARM_PP_LIST_FOLD_OVERFLOW)
# define FARM_PP_ERROR_0x0005 FARM_PP_ERROR(0x0005, FARM_PP_SEQ_FOLD_OVERFLOW)
# define FARM_PP_ERROR_0x0006 FARM_PP_ERROR(0x0006, FARM_PP_ARITHMETIC_OVERFLOW)
# define FARM_PP_ERROR_0x0007 FARM_PP_ERROR(0x0007, FARM_PP_DIVISION_BY_ZERO)
#
# endif
