# /* **************************************************************************
#  *                                                                          *
#  *     (C) Copyright Edward Diener 2014.
#  *     Distributed under the Boost Software License, Version 1.0. (See
#  *     accompanying file LICENSE_1_0.txt or copy at
#  *     http://www.boost.org/LICENSE_1_0.txt)
#  *                                                                          *
#  ************************************************************************** */
#
# /* See http://www.boost.org for most recent version. */
#
#ifndef FARM_PP_PREPROCESSOR_REMOVE_PARENS_HPP
#define FARM_PP_PREPROCESSOR_REMOVE_PARENS_HPP

#include <farm_pp/preprocessor/config/config.hpp>
#include <farm_pp/preprocessor/control/iif.hpp>
#include <farm_pp/preprocessor/facilities/identity.hpp>
#include <farm_pp/preprocessor/punctuation/is_begin_parens.hpp>
#include <farm_pp/preprocessor/tuple/enum.hpp>

#define FARM_PP_REMOVE_PARENS(param) \
    FARM_PP_IIF \
      ( \
      FARM_PP_IS_BEGIN_PARENS(param), \
      FARM_PP_REMOVE_PARENS_DO, \
      FARM_PP_IDENTITY \
      ) \
    (param)() \
/**/

#define FARM_PP_REMOVE_PARENS_DO(param) \
  FARM_PP_IDENTITY(FARM_PP_TUPLE_ENUM(param)) \
/**/

#endif /* FARM_PP_PREPROCESSOR_REMOVE_PARENS_HPP */
