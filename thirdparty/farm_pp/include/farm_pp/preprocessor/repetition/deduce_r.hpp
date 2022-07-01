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
# ifndef FARM_PP_PREPROCESSOR_REPETITION_DEDUCE_R_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_DEDUCE_R_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
# include <farm_pp/preprocessor/repetition/for.hpp>
#
# /* FARM_PP_DEDUCE_R */
#
# define FARM_PP_DEDUCE_R() FARM_PP_AUTO_REC(FARM_PP_FOR_P, 256)
#
# else
#
# /* FARM_PP_DEDUCE_R */
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
# include <farm_pp/preprocessor/repetition/for.hpp>
# include <farm_pp/preprocessor/config/limits.hpp>
#
# if FARM_PP_LIMIT_FOR == 256
# define FARM_PP_DEDUCE_R() FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_FOR_P, 256))
# elif FARM_PP_LIMIT_FOR == 512
# define FARM_PP_DEDUCE_R() FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_FOR_P, 512))
# elif FARM_PP_LIMIT_FOR == 1024
# define FARM_PP_DEDUCE_R() FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_FOR_P, 1024))
# else
# error Incorrect value for the FARM_PP_LIMIT_FOR limit
# endif
#
# endif
#
# endif
