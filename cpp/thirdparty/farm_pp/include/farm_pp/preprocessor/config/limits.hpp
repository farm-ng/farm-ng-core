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
# /* Revised by Edward Diener (2011,2020) */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_CONFIG_LIMITS_HPP
# define FARM_PP_PREPROCESSOR_CONFIG_LIMITS_HPP
#
# include <farm_pp/preprocessor/config/config.hpp>
#
# if defined(FARM_PP_LIMIT_DIM)
# undef FARM_PP_LIMIT_DIM
# endif
# if defined(FARM_PP_LIMIT_ITERATION_DIM)
# undef FARM_PP_LIMIT_ITERATION_DIM
# endif
# if defined(FARM_PP_LIMIT_SLOT_SIG)
# undef FARM_PP_LIMIT_SLOT_SIG
# endif
# if defined(FARM_PP_LIMIT_SLOT_COUNT)
# undef FARM_PP_LIMIT_SLOT_COUNT
# endif
# if defined(FARM_PP_LIMIT_WHILE)
# undef FARM_PP_LIMIT_WHILE
# endif
#
# if ~FARM_PP_CONFIG_FLAGS() & FARM_PP_CONFIG_STRICT()
#
# if defined(FARM_PP_LIMIT_MAG)
# undef FARM_PP_LIMIT_MAG
# endif
# if defined(FARM_PP_LIMIT_VARIADIC)
# undef FARM_PP_LIMIT_VARIADIC
# endif
# if defined(FARM_PP_LIMIT_TUPLE)
# undef FARM_PP_LIMIT_TUPLE
# endif
# if defined(FARM_PP_LIMIT_FOR)
# undef FARM_PP_LIMIT_FOR
# endif
# if defined(FARM_PP_LIMIT_REPEAT)
# undef FARM_PP_LIMIT_REPEAT
# endif
# if defined(FARM_PP_LIMIT_SEQ)
# undef FARM_PP_LIMIT_SEQ
# endif
# if defined(FARM_PP_LIMIT_ITERATION)
# undef FARM_PP_LIMIT_ITERATION
# endif
#
# define FARM_PP_LIMIT_MAG 256
# define FARM_PP_LIMIT_WHILE 256
# define FARM_PP_LIMIT_VARIADIC 64
# define FARM_PP_LIMIT_TUPLE 64
# define FARM_PP_LIMIT_FOR 256
# define FARM_PP_LIMIT_SEQ 256
# define FARM_PP_LIMIT_REPEAT 256
# define FARM_PP_LIMIT_ITERATION 256
#
#else
#
# if defined(FARM_PP_LIMIT_MAG)
# if !(FARM_PP_LIMIT_MAG == 256 || FARM_PP_LIMIT_MAG == 512 || FARM_PP_LIMIT_MAG == 1024)
# undef FARM_PP_LIMIT_MAG
# define FARM_PP_LIMIT_MAG 256
# define FARM_PP_LIMIT_WHILE 256
# else
# define FARM_PP_LIMIT_WHILE FARM_PP_LIMIT_MAG
# if !defined(FARM_PP_LIMIT_SEQ)
# define FARM_PP_LIMIT_SEQ FARM_PP_LIMIT_MAG
# endif
# endif
# else
# define FARM_PP_LIMIT_MAG 256
# define FARM_PP_LIMIT_WHILE 256
# endif
#
# if defined(FARM_PP_LIMIT_VARIADIC)
# if !(FARM_PP_LIMIT_VARIADIC == 64 || FARM_PP_LIMIT_VARIADIC == 128 || FARM_PP_LIMIT_VARIADIC == 256)
# undef FARM_PP_LIMIT_VARIADIC
# define FARM_PP_LIMIT_VARIADIC 64
# endif
# else
# define FARM_PP_LIMIT_VARIADIC 64
# endif
#
# if defined(FARM_PP_LIMIT_TUPLE)
# if !(FARM_PP_LIMIT_TUPLE == 64 || FARM_PP_LIMIT_TUPLE == 128 || FARM_PP_LIMIT_TUPLE == 256)
# undef FARM_PP_LIMIT_TUPLE
# define FARM_PP_LIMIT_TUPLE 64
# elif FARM_PP_LIMIT_TUPLE > FARM_PP_LIMIT_VARIADIC
# undef FARM_PP_LIMIT_VARIADIC
# define FARM_PP_LIMIT_VARIADIC FARM_PP_LIMIT_TUPLE
# endif
# else
# define FARM_PP_LIMIT_TUPLE 64
# endif
#
# if defined(FARM_PP_LIMIT_FOR)
# if !(FARM_PP_LIMIT_FOR == 256 || FARM_PP_LIMIT_FOR == 512 || FARM_PP_LIMIT_FOR == 1024)
# undef FARM_PP_LIMIT_FOR
# define FARM_PP_LIMIT_FOR 256
# elif FARM_PP_LIMIT_FOR > FARM_PP_LIMIT_MAG
# undef FARM_PP_LIMIT_FOR
# define FARM_PP_LIMIT_FOR FARM_PP_LIMIT_MAG
# endif
# else
# define FARM_PP_LIMIT_FOR 256
# endif
#
# if defined(FARM_PP_LIMIT_REPEAT)
# if !(FARM_PP_LIMIT_REPEAT == 256 || FARM_PP_LIMIT_REPEAT == 512 || FARM_PP_LIMIT_REPEAT == 1024)
# undef FARM_PP_LIMIT_REPEAT
# define FARM_PP_LIMIT_REPEAT 256
# elif FARM_PP_LIMIT_REPEAT > FARM_PP_LIMIT_MAG
# undef FARM_PP_LIMIT_REPEAT
# define FARM_PP_LIMIT_REPEAT FARM_PP_LIMIT_MAG
# endif
# else
# define FARM_PP_LIMIT_REPEAT 256
# endif
#
# if defined(FARM_PP_LIMIT_SEQ)
# if !(FARM_PP_LIMIT_SEQ == 256 || FARM_PP_LIMIT_SEQ == 512 || FARM_PP_LIMIT_SEQ == 1024)
# undef FARM_PP_LIMIT_SEQ
# define FARM_PP_LIMIT_SEQ 256
# elif FARM_PP_LIMIT_SEQ > FARM_PP_LIMIT_MAG
# undef FARM_PP_LIMIT_SEQ
# define FARM_PP_LIMIT_SEQ FARM_PP_LIMIT_MAG
# endif
# else
# define FARM_PP_LIMIT_SEQ 256
# endif
#
# if defined(FARM_PP_LIMIT_ITERATION)
# if !(FARM_PP_LIMIT_ITERATION == 256 || FARM_PP_LIMIT_ITERATION == 512 || FARM_PP_LIMIT_ITERATION == 1024)
# undef FARM_PP_LIMIT_ITERATION
# define FARM_PP_LIMIT_ITERATION 256
# elif FARM_PP_LIMIT_ITERATION > FARM_PP_LIMIT_MAG
# undef FARM_PP_LIMIT_ITERATION
# define FARM_PP_LIMIT_ITERATION FARM_PP_LIMIT_MAG
# endif
# else
# define FARM_PP_LIMIT_ITERATION 256
# endif
#
# endif
#
# define FARM_PP_LIMIT_DIM 3
# define FARM_PP_LIMIT_ITERATION_DIM 3
# define FARM_PP_LIMIT_SLOT_SIG 10
# define FARM_PP_LIMIT_SLOT_COUNT 5
#
# endif
