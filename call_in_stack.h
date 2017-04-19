#ifndef _CALLINSTACK_H_
#define _CALLINSTACK_H_

#include "call_in_stack_config.h"

//1.We have arguments passing stack cost and previous sp pointer saving cost in stack before calling.
//  So we use get_stack_base<cost + 1>(stack_buffer + length) as stack_base.
//2.Some versions of GCC can not support "call_in_stack_impl::static_asserter< i == call_in_stack_impl::function_property<T>::arguments_count && (typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0) > *p = 0" 
//  to break overload ambiguous(ambiguous error has higher priority than direct SFINAE?). So we use inner type of call_in_stack_impl::static_asserter instead.
//3.The assert "i == call_in_stack_impl::function_property<T>::arguments_count && (typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0)" 
//  is to help the overload resolution between "i fixed arguments, j variable arguments" and "i+1 fixed arguments, j-1 variable arguments" 

#define call_in_stack_define(i, j) \
template <MACRO_JOIN(RECURSIVE_FUNC_, j)(define_typenames_ex_begin, define_typenames_ex, define_typenames_ex) typename T > \
DLL_LOCAL inline typename call_in_stack_impl::function_property<T>::return_type call_in_stack(T dest_func  \
MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typeargs_begin, define_typeargs, define_typeargs) \
MACRO_JOIN(RECURSIVE_FUNC_,j)(define_type_args_ex_begin, define_type_args_ex, define_type_args_ex_end) \
, typename call_in_stack_impl::static_asserter< i == call_in_stack_impl::function_property<T>::arguments_count && (call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0)>::type *p = 0){ \
	using namespace call_in_stack_impl; \
	const int default_stack_length = 16*1024; \
	struct auto_stack_buffer{ \
		char* stack_buffer; \
		auto_stack_buffer(unsigned int the_size):stack_buffer(new char[the_size]){} \
		~auto_stack_buffer(){delete []stack_buffer;} \
	}z(default_stack_length); \
	typedef typename function_property<T>::return_type return_type; \
	typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) void> arg_types;\
    return return_type_adapter<return_type>::forward(do_call< \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) \
		typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments>( \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin, define_args, define_args) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_begin, define_args_ex, define_args_ex) \
		(void*)dest_func, get_stack_base< STACK_COST(arg_types) >(z.stack_buffer + default_stack_length) )); \
} \
template <MACRO_JOIN(RECURSIVE_FUNC_, j)(define_typenames_ex_begin, define_typenames_ex, define_typenames_ex) typename T > \
DLL_LOCAL inline typename call_in_stack_impl::function_property<T>::return_type call_in_stack(unsigned int stack_length, T dest_func  \
MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typeargs_begin, define_typeargs, define_typeargs) \
MACRO_JOIN(RECURSIVE_FUNC_,j)(define_type_args_ex_begin, define_type_args_ex, define_type_args_ex_end) \
, typename call_in_stack_impl::static_asserter< i == call_in_stack_impl::function_property<T>::arguments_count && (call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0)>::type *p = 0){ \
	struct auto_stack_buffer{ \
		char* stack_buffer; \
		auto_stack_buffer(unsigned int the_size):stack_buffer(new char[the_size]){} \
		~auto_stack_buffer(){delete []stack_buffer;} \
	}z(stack_length); \
    using namespace call_in_stack_impl; \
	typedef typename function_property<T>::return_type return_type; \
	typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) void> arg_types;\
    return return_type_adapter<return_type>::forward(do_call< \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) \
		typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments>( \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin, define_args, define_args) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_begin, define_args_ex, define_args_ex) \
		(void*)dest_func, get_stack_base< STACK_COST(arg_types) >(z.stack_buffer + stack_length) )); \
} \
template <MACRO_JOIN(RECURSIVE_FUNC_, j)(define_typenames_ex_begin, define_typenames_ex, define_typenames_ex) typename T > \
DLL_LOCAL inline typename call_in_stack_impl::function_property<T>::return_type call_in_stack(char* stack_buffer, unsigned int stack_length, T dest_func \
MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typeargs_begin, define_typeargs, define_typeargs) \
MACRO_JOIN(RECURSIVE_FUNC_,j)(define_type_args_ex_begin, define_type_args_ex, define_type_args_ex_end) \
, typename call_in_stack_impl::static_asserter< i == call_in_stack_impl::function_property<T>::arguments_count && (call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0)>::type *p = 0){ \
    using namespace call_in_stack_impl; \
	typedef typename function_property<T>::return_type return_type; \
	typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) void> arg_types;\
    return return_type_adapter<return_type>::forward(do_call< \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) \
		typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments>( \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin, define_args, define_args) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_begin, define_args_ex, define_args_ex) \
		(void*)dest_func, get_stack_base< STACK_COST(arg_types) >(stack_buffer + stack_length) )); \
} \
template <MACRO_JOIN(RECURSIVE_FUNC_, j)(define_typenames_ex_begin, define_typenames_ex, define_typenames_ex) typename T, typename B, int N > \
DLL_LOCAL inline typename call_in_stack_impl::function_property<T>::return_type call_in_stack(B (&stack_buffer)[N], T dest_func  \
MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typeargs_begin, define_typeargs, define_typeargs) \
MACRO_JOIN(RECURSIVE_FUNC_,j)(define_type_args_ex_begin, define_type_args_ex, define_type_args_ex_end) \
, typename call_in_stack_impl::static_asserter< i == call_in_stack_impl::function_property<T>::arguments_count && (call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0)>::type *p = 0){ \
    using namespace call_in_stack_impl; \
	typedef typename function_property<T>::return_type return_type; \
	typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) void> arg_types;\
    return return_type_adapter<return_type>::forward(do_call< \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) \
		typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments>( \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin, define_args, define_args) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_begin, define_args_ex, define_args_ex) \
		(void*)dest_func, get_stack_base< STACK_COST(arg_types) >((char*)(&(stack_buffer[N])) ))); \
}
BI_TWO_BATCH_FUNC1(10, call_in_stack_define)
//BI_TWO_BATCH_FUNC1 will render 11*10 times and we do not support pure variable argument lists without any fixed argument(comparing printf(...) and printf(char*,...); the previous function does not make sense)
call_in_stack_define(0,0)

//call_in_stack_safe is safe for recursively call_in_stack with same stack_buffer(maybe it is a global variable?) as stack.
#define call_in_stack_safe_define(i, j) \
template <MACRO_JOIN(RECURSIVE_FUNC_, j)(define_typenames_ex_begin, define_typenames_ex, define_typenames_ex) typename T > \
DLL_LOCAL inline typename call_in_stack_impl::function_property<T>::return_type call_in_stack_safe( char* stack_buffer, unsigned int stack_length, T dest_func \
MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typeargs_begin, define_typeargs, define_typeargs) \
MACRO_JOIN(RECURSIVE_FUNC_,j)(define_type_args_ex_begin, define_type_args_ex, define_type_args_ex_end) \
, typename call_in_stack_impl::static_asserter< i == call_in_stack_impl::function_property<T>::arguments_count && (call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0)>::type *p = 0){ \
	using namespace call_in_stack_impl; \
	word_int_t sp_value; sp_value = (word_int_t)&sp_value;\
	if((sp_value > (call_in_stack_impl::word_int_t)stack_buffer) && ((word_int_t)sp_value < (word_int_t)stack_buffer + stack_length)){ \
		return dest_func( \
			MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin_org, define_args_org, define_args_end_org) \
			MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_c_begin, define_args_ex_c, define_args_ex_c_end) \
		); \
	} \
	else{ \
        typedef typename function_property<T>::return_type return_type; \
		typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
			MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) void> arg_types;\
		return return_type_adapter<return_type>::forward(do_call< \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) \
		typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments>( \
			MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin, define_args, define_args) \
			MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_begin, define_args_ex, define_args_ex) \
			(void*)dest_func, get_stack_base< STACK_COST(arg_types) >(stack_buffer + stack_length) )); \
	} \
} \
template <MACRO_JOIN(RECURSIVE_FUNC_, j)(define_typenames_ex_begin, define_typenames_ex, define_typenames_ex) typename T, typename B, int N > \
DLL_LOCAL inline typename call_in_stack_impl::function_property<T>::return_type call_in_stack_safe(B (&stack_buffer)[N], T dest_func \
MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typeargs_begin, define_typeargs, define_typeargs) \
MACRO_JOIN(RECURSIVE_FUNC_,j)(define_type_args_ex_begin, define_type_args_ex, define_type_args_ex_end) \
, typename call_in_stack_impl::static_asserter< i == call_in_stack_impl::function_property<T>::arguments_count && (call_in_stack_impl::function_property<T>::has_variable_arguments || j == 0)>::type *p = 0){ \
	using namespace call_in_stack_impl; \
	word_int_t sp_value; sp_value = (word_int_t)&sp_value;\
	if((sp_value > (word_int_t)(&stack_buffer)) && ((word_int_t)sp_value < (word_int_t)(&(stack_buffer[N])))){ \
		return dest_func( \
			MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin_org, define_args_org, define_args_end_org) \
			MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_c_begin, define_args_ex_c, define_args_ex_c_end) \
		); \
	} \
	else{ \
        typedef typename function_property<T>::return_type return_type; \
		typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
			MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) void> arg_types;\
		return return_type_adapter<return_type>::forward(do_call< \
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_rtypes_begin, define_rtypes, define_rtypes) \
		MACRO_JOIN(RECURSIVE_FUNC_,j)(define_rtype_ex_begin, define_rtype_ex, define_rtype_ex) \
		typename return_type_adapter<return_type>::forward_type, call_in_stack_impl::function_property<T>::has_variable_arguments>( \
			MACRO_JOIN(RECURSIVE_FUNC_,i)(define_args_begin, define_args, define_args) \
			MACRO_JOIN(RECURSIVE_FUNC_,j)(define_args_ex_begin, define_args_ex, define_args_ex) \
			(void*)dest_func, get_stack_base< STACK_COST(arg_types) >((char*)(&(stack_buffer[N])) ))); \
	} \
}

BI_TWO_BATCH_FUNC1(10, call_in_stack_safe_define)
//BI_TWO_BATCH_FUNC1 will render 10*11 times and we do not support pure variable argument lists without any fixed argument(comparing printf(...) and printf(char*,...); the previous function does not make sense)
call_in_stack_safe_define(0,0)

// adapter of member function
#define from_member_fun(class_obj, member_name,...) &call_in_stack_impl::member_function_wrapper<__typeof__(&call_in_stack_impl::change_ref_to_pointer_size<__typeof__(class_obj)>::content_type::member_name)>::exec, \
call_in_stack_impl::member_function_wrapper<__typeof__(&call_in_stack_impl::change_ref_to_pointer_size<__typeof__(class_obj)>::content_type::member_name)>(class_obj, &call_in_stack_impl::change_ref_to_pointer_size<__typeof__(class_obj)>::content_type::member_name),##__VA_ARGS__

// adapter of functor or lambda
#define from_functor(class_obj,...) from_member_fun(class_obj, operator(),##__VA_ARGS__)

#endif
