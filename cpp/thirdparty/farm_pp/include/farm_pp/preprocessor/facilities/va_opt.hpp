# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2019.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
# ifndef FARM_PP_PREPROCESSOR_FACILITIES_VA_OPT_HPP
# define FARM_PP_PREPROCESSOR_FACILITIES_VA_OPT_HPP
# include <farm_pp/preprocessor/variadic/has_opt.hpp>
# if FARM_PP_VARIADIC_HAS_OPT()
# include <farm_pp/preprocessor/control/iif.hpp>
# include <farm_pp/preprocessor/facilities/check_empty.hpp>
# include <farm_pp/preprocessor/tuple/rem.hpp>
# define FARM_PP_VA_OPT_IMPL(atuple) \
    FARM_PP_TUPLE_REM() atuple       \
/**/
# define FARM_PP_VA_OPT(rdata,rempty,...)     \
    FARM_PP_VA_OPT_IMPL                       \
        (                                      \
        FARM_PP_IIF                           \
            (                                  \
            FARM_PP_CHECK_EMPTY(__VA_ARGS__), \
            rempty,                            \
            rdata                              \
            )                                  \
        )                                      \
/**/
# endif /* FARM_PP_VARIADIC_HAS_OPT() */
# endif /* FARM_PP_PREPROCESSOR_FACILITIES_VA_OPT_HPP */
