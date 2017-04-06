#ifndef _CALLINSTACK_X64SYSV_H_
#define _CALLINSTACK_X64SYSV_H_

#ifdef __x86_64__

#define WORDSIZE (8)

#define WORDBITSIZE (8*WORDSIZE)

#define MAX_ARGUMENT_SIZE (sizeof(long double))

#define MAX_RETUREN_SIZE (sizeof(long double))

#define STACK_ALIGNMENT_SIZE (16)

typedef unsigned long long word_int_t;

#include "template_util.h"

template <typename T>
struct type_test{
	const static int floatreg_cost = 0;
	const static int intreg_cost = 1;
	const static bool is_float = false;
};

template <typename T>
struct type_test<const T>:public type_test<T>{
};

template <typename T>
struct type_test<volatile T>:public type_test<T>{
};

template <>
struct type_test<float>{
	const static int floatreg_cost = 1;
	const static int intreg_cost = 0;
	const static bool is_float = true;
};

template <>
struct type_test<double>{
	const static int floatreg_cost = 1;
	const static int intreg_cost = 0;
	const static bool is_float = true;
};

template <>
struct type_test<long double>{
	const static int floatreg_cost = 0;		//in x64, stack is used for transmitting the long double parameter
	const static int intreg_cost = 0;
	const static bool is_float = true;
};

#define args_list_define(i) \
template<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames_end)> \
struct args_list< MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)> \
	: public args_list< MACRO_JOIN(RECURSIVE_FUNC_,i)(define_parent_begin, define_parent, define_parent_end)>{ \
	typedef args_list< MACRO_JOIN(RECURSIVE_FUNC_,i)(define_parent_begin, define_parent, define_parent_end)> parent; \
	typedef MACRO_JOIN(t,i) new_type; \
	\
	const static int intreg_cost = parent::intreg_cost + ((parent::intreg_cost != parent::max_int_reg_cost_x64_system_v) && (type_test<new_type>::intreg_cost != 0) ? 1 : 0) ; \
	const static int floatreg_cost = parent::floatreg_cost + ((parent::floatreg_cost != parent::max_float_reg_cost_x64_system_v) && (type_test<new_type>::floatreg_cost != 0) ? 1 : 0); \
	const static int float_count = parent::float_count + type_test<new_type>::is_float ? 1 : 0;\
\
	const static int addtional_stack_cost =  ((parent::intreg_cost != intreg_cost ) || (parent::floatreg_cost != floatreg_cost))\
	? 0:_COUNT_OF_SIZE(change_ref_to_pointer_size<new_type>::size, WORDSIZE);\
	const static int stackword_cost = parent::stackword_cost + ((addtional_stack_cost == 0)? 0 : (_ALIGNED_COST(parent::stackword_cost, addtional_stack_cost)));\
	STATIC_ASSERTER(cost_too_much_stack, stackword_cost < WORDBITSIZE);\
	const static int stack_padding_reporter = (parent::stack_padding_reporter << (stackword_cost-parent::stackword_cost)) + (1<<addtional_stack_cost) - 1;\
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
			__asm__ ("pushq " MACRO_TOSTRING(j*WORDSIZE) "(%r10);\n\t");\
	else 	__asm__ ("leaq -" MACRO_TOSTRING(WORDSIZE) "(%rsp), %rsp;\n\t");}

#define restore_stack_define(j) if(arg_types::stackword_cost == j){\
	__asm__ ("movq " MACRO_TOSTRING(j*WORDSIZE) "(%rsp), %rsp;\n\t");}

//MAX_ARGUMENT_SIZE = 2*WORDSIZE, 10*2=20,
//In x64, the "dest_func" argument may be stored at register so we do not know the parameters' address and we should copy arguments beginning from sp+stackword_cost
#define func_back1(func) func(20) func(19) func(18) func(17) func(16) func(15) func(14) func(13) func(12) func(11) func(10) func(9) func(8) func(7) func(6) func(5) func(4) func(3) func(2) func(1)

#define func_back(func) func_back1(func) func(0)

//We save arguments and previous sp in new stack
#define STACK_COST(T) (function_property<T>::stackword_cost + 1)

#define call_with_stack_define(i) \
template <MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames) bool has_variable_arguments > \
__attribute__ ((noinline)) static RETURN_TYPE call_with_stack(\
	MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types)  \
	void* dest_func, char* stack_base ){\
	typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)> arg_types; \
	INIT_INSTRUCTION(RETURN_TYPE) \
 	if(arg_types::intreg_cost >= 6){\
		__asm__ (	"movq 	%0,  	%%r11;		\n\t" 	\
					::"X"(dest_func)		\
				);\
	}\
\
	if(arg_types::stackword_cost == 0 && arg_types::intreg_cost <= 4){\
		__asm__ (	"movq 	%%rsp,  -" MACRO_TOSTRING(WORDSIZE) "(%0);		\n\t" 	\
					"leaq 	-" MACRO_TOSTRING(WORDSIZE) "(%0),  %%rsp;		\n\t" 	\
					::"X"(stack_base)	\
				);\
	}else{										\
		__asm__ (	"movq 	%%rsp,  %%r10;		\n\t" 	\
					"movq 	%0,  	%%rsp;		\n\t" 	\
					"pushq 	%%r10;				\n\t"   \
					::"X"(stack_base)		\
				);										\
		func_back1(push_stack_define)					\
	}\
\
	if(arg_types::float_count > 0 && has_variable_arguments){\
		__asm__ (	"movq 	%0,  %%rax;			\n\t"	\
				:: "X"(arg_types::float_count));	\
	}\
\
	if(arg_types::intreg_cost >= 6){					\
		__asm__ (	"callq 	*%r11;				\n\t");	\
	}else{												\
		__asm__ (	"callq	*%0					\n\t"	\
				::"X"(dest_func)						\
		);												\
	}													\
\
	func_back(restore_stack_define)						\
	RETURN_INSTRUCTION(RETURN_TYPE);					\
}

#pragma GCC push_options
#pragma GCC optimize ("O2")
//More than O2 or Os is also enabled. You can set O3 or Os.
//We use this because we cannot use "naked" attribute in x86 and x64, we will use forced O2 optimization (function O2 attribute maybe ignored by some compilers) instead.


template<typename R>
struct call_with_stack_class{
#define RETURN_TYPE R
#define INIT_INSTRUCTION(r_type)
#define RETURN_INSTRUCTION(r_type) 	  	__asm__ ("ret;\n\t");DUMMY_RETURN(r_type);
BATCH_FUNC(call_with_stack_define)
typedef assert_not_class_not_largesize<R, MAX_RETUREN_SIZE> assert_instance;
#undef RETURN_TYPE
#undef INIT_INSTRUCTION
#undef RETURN_INSTRUCTION
};


#pragma GCC pop_options

//GET_SP is to avoid uninitialized warning
#define GET_SP(sp_value) __asm__ ("movq 	%%rsp,  %0;	\n\t" : "=X"(sp_value))
#define DEF_SP(sp_value) register  char * volatile sp_value asm ("rsp");GET_SP(sp_value)
//Maybe your compiler does not support register variable? use DEF_SP_BAK instead!
#define DEF_SP_BAK(sp_value) char *sp_value; GET_SP(sp_value)
#endif
#endif
