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
# ifndef FARM_PP_PREPROCESSOR_SLOT_SLOT_HPP
# define FARM_PP_PREPROCESSOR_SLOT_SLOT_HPP
#
# include <farm_pp/preprocessor/cat.hpp>
# include <farm_pp/preprocessor/slot/detail/def.hpp>
#
# /* FARM_PP_ASSIGN_SLOT */
#
# define FARM_PP_ASSIGN_SLOT(i) FARM_PP_CAT(FARM_PP_ASSIGN_SLOT_, i)
#
# define FARM_PP_ASSIGN_SLOT_1 <farm_pp/preprocessor/slot/detail/slot1.hpp>
# define FARM_PP_ASSIGN_SLOT_2 <farm_pp/preprocessor/slot/detail/slot2.hpp>
# define FARM_PP_ASSIGN_SLOT_3 <farm_pp/preprocessor/slot/detail/slot3.hpp>
# define FARM_PP_ASSIGN_SLOT_4 <farm_pp/preprocessor/slot/detail/slot4.hpp>
# define FARM_PP_ASSIGN_SLOT_5 <farm_pp/preprocessor/slot/detail/slot5.hpp>
#
# /* FARM_PP_SLOT */
#
# define FARM_PP_SLOT(i) FARM_PP_CAT(FARM_PP_SLOT_, i)()
#
# endif
