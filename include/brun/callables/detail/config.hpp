/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : config
 * @created     : Tuesday Aug 22, 2023 23:59:29 CEST
 * @description : 
 * */

#ifndef BRUN_CALLABLES_DETAIL_CONFIG_HPP
#define BRUN_CALLABLES_DETAIL_CONFIG_HPP

#if defined(__cpp_static_call_operator) && __cpp_static_call_operator >= 202207L
#define CB_STATIC static
#define CB_CONST
#else
#define CB_STATIC
#define CB_CONST const
#endif



#endif /* BRUN_CALLABLES_DETAIL_CONFIG_HPP */

