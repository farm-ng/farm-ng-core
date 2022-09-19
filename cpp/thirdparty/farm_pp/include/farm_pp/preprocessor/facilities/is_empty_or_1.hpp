# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Paul Mensonides 2003.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_FACILITIES_IS_EMPTY_OR_1_HPP
# define FARM_PP_PREPROCESSOR_FACILITIES_IS_EMPTY_OR_1_HPP
#
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/empty.hpp>
# include <farm_pp/preprocessor/facilities/identity.hpp>
# include <farm_pp/preprocessor/facilities/is_1.hpp>
# include <farm_pp/preprocessor/facilities/is_empty.hpp>
#
# /* FARM_PP_IS_EMPTY_OR_1 */
#
# define FARM_PP_IS_EMPTY_OR_1(x) \
    FARM_PP_IIF( \
        FARM_PP_IS_EMPTY(x FARM_PP_EMPTY()), \
        FARM_PP_IDENTITY(1), \
        FARM_PP_IS_1 \
    )(x) \
    /**/
#
# endif
