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
# if !defined(FARM_PP_INDIRECT_SELF)
#    error FARM_PP_ERROR:  no indirect file to include
# endif
#
# define FARM_PP_IS_SELFISH 1
#
# include FARM_PP_INDIRECT_SELF
#
# undef FARM_PP_IS_SELFISH
# undef FARM_PP_INDIRECT_SELF
