farm_pp is merely a trivial fork of the boost preprocessor macro library from:

https://github.com/boostorg/preprocessor


Motivation is to have farm-ng-core more self-contained an not to pull in boost
at this level.


Macros are renamed as follows:

 BOOST_*    -> FARM_PP_*
 BOOST_PP_* -> FARM_PP_*

so there should not be any conflicts if a boost dependencies is included
somewhere downstream.

This fork is based on commit: 667e87b3392db338a919cbe0213979713aca52e3