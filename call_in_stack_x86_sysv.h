#ifndef _CALLINSTACK_X86SYSV_H_
#define _CALLINSTACK_X86SYSV_H_

#ifdef __i386__

#define WORDSIZE (4)

#define WORDBITSIZE (8*WORDSIZE)

#define MAX_ARGUMENT_SIZE (sizeof(long double))

#define MAX_RETUREN_SIZE (sizeof(long double))

#define STACK_ALIGNMENT_SIZE (16)		//After GCC 4.5 or in Apple MAC, it is required for 16 bytes stack alignment.

typedef unsigned int word_int_t;

#include "template_util.h"

#define args_list_define(i) \
template<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames_end)> \
struct args_list< MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)> \
	: public args_list< MACRO_JOIN(RECURSIVE_FUNC_,i)(define_parent_begin, define_parent, define_parent_end)>{ \
	typedef args_list< MACRO_JOIN(RECURSIVE_FUNC_,i)(define_parent_begin, define_parent, define_parent_end)> parent; \
	typedef MACRO_JOIN(t,i) new_type; \
	\
	const static word_int_t addtional_stack_cost = _COUNT_OF_SIZE(change_ref_to_pointer_size<new_type>::size, WORDSIZE);\
	const static word_int_t stackword_cost = parent::stackword_cost + ((addtional_stack_cost == 0)? 0 : (_ALIGNED_COST(parent::stackword_cost, addtional_stack_cost)));\
	const static word_int_t stack_padding_reporter = (1<<stackword_cost) -1;\
	\
	typedef assert_not_class_not_largesize<new_type, MAX_ARGUMENT_SIZE> assert_instance;\
	template<typename O>\
	static void out_stackword_cost(O& o){\
		o<<"stackword_cost at " <<i<<"th:"<<stackword_cost<<"\r\n";\
		parent::out_stackword_cost(o);\
	}\
	template<typename O>\
	static void out_stack_padding_reporter(O& o){\
		o<<"stack_padding_reporter at " <<i<<"th:"<<stack_padding_reporter<<"\r\n";\
		parent::out_stack_padding_reporter(o);\
	}\
};

BATCH_FUNC1(args_list_define)

//((arg_types::stack_padding_reporter & (((word_int_t)1)<<((word_int_t)(arg_types::stackword_cost-j))%WORDBITSIZE)) != 0) means the space is for padding(and lea is cheaper than push)
// Here "%WORDBITSIZE" is to escape from shifting overflow warning...

#define push_stack_define(j) if(arg_types::stackword_cost >= j ){\
	if((arg_types::stack_padding_reporter & (((word_int_t)1)<<((word_int_t)(arg_types::stackword_cost-j))%WORDBITSIZE)) != 0) \
			__asm__ ("push "MACRO_TOSTRING(j*WORDSIZE+WORDSIZE)"(%ebp);\n\t");\
	else 	__asm__ ("lea -"MACRO_TOSTRING(WORDSIZE)"(%esp), %esp;\n\t");}

//MAX_ARGUMENT_SIZE = 3*WORDSIZE, 10*3=30,
#define func_back1(func)  func(30) func(29) func(28) func(27) func(26) func(25) func(24) func(23) func(22) func(21) func(20) func(19) func(18) func(17) func(16) func(15) func(14) func(13) func(12) func(11) func(10) func(9) func(8) func(7) func(6) func(5) func(4) func(3) func(2) func(1)

#define func_back(func) func_back1(func) func(0)

//We save arguments and previous sp in new stack
#define STACK_COST(T) (function_property<T>::stackword_cost + 1)

#define call_with_stack_define(i) \
template <MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames) bool has_variable_arguments> \
__attribute__ ((noinline)) static RETURN_TYPE call_with_stack(\
	MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types)  \
	void* dest_func, char* stack_base){\
	typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)> arg_types; \
	__asm__ ("mov 	%0, %%esp;		\n\t" 	\
				::"X"(stack_base));			\
	func_back1(push_stack_define)			\
	__asm__ ("call 	*%0;			\n\t" 	\
				::"X"(dest_func));			\
	__asm__ ("mov 	%ebp, %esp;		\n\t");	\
	RETURN_INSTRUCTION(RETURN_TYPE);		\
}

template<typename R>
struct call_with_stack_class{
#define RETURN_TYPE R
//manually pop 	%ebp because we need manually ret!
#define RETURN_INSTRUCTION(r_type) 	__asm__ ("pop 	%ebp;\n\t"); __asm__ ("ret;\n\t");DUMMY_RETURN(r_type);
BATCH_FUNC(call_with_stack_define)
typedef assert_not_class_not_largesize<R, MAX_RETUREN_SIZE> assert_instance;
#undef RETURN_TYPE
#undef RETURN_INSTRUCTION
};

//GET_SP is to avoid uninitialized warning
#define GET_SP(sp_value) __asm__ (	"mov 	%%esp,  %0;	\n\t" 	:"=X"(sp_value))
#define DEF_SP(sp_value) register char *sp_value asm ("esp"); GET_SP(sp_value)
//Maybe your compiler does not support register variable? use DEF_SP_BAK instead!
#define DEF_SP_BAK(sp_value) char *sp_value; GET_SP(sp_value)
#endif
#endif
