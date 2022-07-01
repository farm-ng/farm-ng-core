# /* Copyright (C) 2001
#  * Housemarque Oy
#  * http://www.housemarque.com
#  *
#  * Distributed under the Boost Software License, Version 1.0. (See
#  * accompanying file LICENSE_1_0.txt or copy at
#  * http://www.boost.org/LICENSE_1_0.txt)
#  */
#
# /* Revised by Paul Mensonides (2002) */
# /* Revised by Edward Diener (2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_LIST_FOLD_RIGHT_HPP
# define FARM_PP_PREPROCESSOR_LIST_FOLD_RIGHT_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/debug/error.hpp>
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
#
# if 0
#    define FARM_PP_LIST_FOLD_RIGHT(op, state, list)
# endif
#
# define FARM_PP_LIST_FOLD_RIGHT FARM_PP_CAT(FARM_PP_LIST_FOLD_RIGHT_, FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256))
#
# define FARM_PP_LIST_FOLD_RIGHT_257(o, s, l) FARM_PP_ERROR(0x0004)
#
# define FARM_PP_LIST_FOLD_RIGHT_D(d, o, s, l) FARM_PP_LIST_FOLD_RIGHT_ ## d(o, s, l)
# define FARM_PP_LIST_FOLD_RIGHT_2ND FARM_PP_LIST_FOLD_RIGHT
# define FARM_PP_LIST_FOLD_RIGHT_2ND_D FARM_PP_LIST_FOLD_RIGHT_D
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    include <farm_pp/preprocessor/list/detail/edg/fold_right.hpp>
# else
#    include <farm_pp/preprocessor/list/detail/fold_right.hpp>
# endif
#
# else
#
# include <farm_pp/preprocessor/arithmetic/dec.hpp>
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/control/while.hpp>
# include <farm_pp/preprocessor/debug/error.hpp>
# include <farm_pp/preprocessor/detail/auto_rec.hpp>
#
# if 0
#    define FARM_PP_LIST_FOLD_RIGHT(op, state, list)
# endif
#
# include <farm_pp/preprocessor/config/limits.hpp>
#
# if FARM_PP_LIMIT_WHILE == 256
# define FARM_PP_LIST_FOLD_RIGHT FARM_PP_CAT(FARM_PP_LIST_FOLD_RIGHT_, FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 256)))
# define FARM_PP_LIST_FOLD_RIGHT_257(o, s, l) FARM_PP_ERROR(0x0004)
# elif FARM_PP_LIMIT_WHILE == 512
# define FARM_PP_LIST_FOLD_RIGHT FARM_PP_CAT(FARM_PP_LIST_FOLD_RIGHT_, FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 512)))
# define FARM_PP_LIST_FOLD_RIGHT_513(o, s, l) FARM_PP_ERROR(0x0004)
# elif FARM_PP_LIMIT_WHILE == 1024
# define FARM_PP_LIST_FOLD_RIGHT FARM_PP_CAT(FARM_PP_LIST_FOLD_RIGHT_, FARM_PP_DEC(FARM_PP_AUTO_REC(FARM_PP_WHILE_P, 1024)))
# define FARM_PP_LIST_FOLD_RIGHT_1025(o, s, l) FARM_PP_ERROR(0x0004)
# else
# error Incorrect value for the FARM_PP_LIMIT_WHILE limit
# endif
#
# define FARM_PP_LIST_FOLD_RIGHT_D(d, o, s, l) FARM_PP_LIST_FOLD_RIGHT_ ## d(o, s, l)
# define FARM_PP_LIST_FOLD_RIGHT_2ND FARM_PP_LIST_FOLD_RIGHT
# define FARM_PP_LIST_FOLD_RIGHT_2ND_D FARM_PP_LIST_FOLD_RIGHT_D
#
# if FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_EDG()
#    include <farm_pp/preprocessor/list/detail/edg/fold_right.hpp>
# else
#    include <farm_pp/preprocessor/list/detail/fold_right.hpp>
# endif
#
# endif
#
# endif
