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
# ifndef FARM_PP_PREPROCESSOR_REPETITION_DEDUCE_Z_HPP
# define FARM_PP_PREPROCESSOR_REPETITION_DEDUCE_Z_HPP
#
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
# include <farm_pp/preprocessor/repetition/repeat.hpp>
#
# /* FARM_PP_DEDUCE_Z */
#
# define FARM_PP_DEDUCE_Z() FARM_PP_AUTO_REC(FARM_PP_REPEAT_P, 4)
#
# endif
