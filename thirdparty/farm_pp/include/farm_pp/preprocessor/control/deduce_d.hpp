# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2002.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* Revised by Edward Diener (2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_CONTROL_DEDUCE_D_HPP
# define FARM_PP_PREPROCESSOR_CONTROL_DEDUCE_D_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
#
# /* FARM_PP_DEDUCE_D */
#
# define FARM_PP_DEDUCE_D() FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256)
#
# else
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
# include <farm_pp/preprocessor/config/limits.hpp>
#
# /* FARM_PP_DEDUCE_D */
#
# if FARM_PP_LIMIT_WHILE == 256
# define FARM_PP_DEDUCE_D() FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256))
# elif FARM_PP_LIMIT_WHILE == 512
# define FARM_PP_DEDUCE_D() FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 512))
# elif FARM_PP_LIMIT_WHILE == 1024
# define FARM_PP_DEDUCE_D() FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 1024))
# else
# error Incorrect value for the FARM_PP_LIMIT_WHILE limit
# endif
#
# endif
#
# endif
