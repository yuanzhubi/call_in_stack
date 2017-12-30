#ifndef _CALLINSTACK_X64SYSV_H_
#define _CALLINSTACK_X64SYSV_H_

#ifdef __x86_64__

#define WORDSIZE (8)

#define WORDBITSIZE (8*WORDSIZE)

#define MAX_ARGUMENT_SIZE (sizeof(long double))

#define MAX_RETUREN_SIZE (sizeof(long double))

#define STACK_ALIGNMENT_SIZE (16)

#include "call_in_stack_config.h"
#include "template_util.h"

namespace call_in_stack_impl{
	
	template <typename T>
	struct type_test{
		const static int floatreg_cost = 0;
		const static int intreg_cost = 1;
		const static bool is_float = false;
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

	template <typename T>
	struct type_test<const T>:public type_test<T>{
	};

	template <typename T>
	struct type_test<volatile T>:public type_test<T>{
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
		const static int float_count = parent::float_count + (type_test<new_type>::is_float ? 1 : 0);\
	\
		const static int addtional_stack_cost =  ((parent::intreg_cost != intreg_cost ) || (parent::floatreg_cost != floatreg_cost))\
            ? 0:_COUNT_OF_SIZE(change_ref_to_pointer<new_type>::size, WORDSIZE);\
		const static int stackword_cost = parent::stackword_cost + ((addtional_stack_cost == 0)? 0 : (_ALIGNED_COST(parent::stackword_cost, addtional_stack_cost)));\
		STATIC_ASSERTER(cost_too_much_stack, stackword_cost < WORDBITSIZE);\
		const static bool use_stack_ahead = parent::use_stack_ahead || (intreg_cost < parent::max_int_reg_cost_x64_system_v && addtional_stack_cost != 0); \
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
	#undef args_list_define




	//MAX_ARGUMENT_SIZE = 2*WORDSIZE, 10*2=20,
	//In x64, the "dest_func" argument may be stored at register so we do not know the parameters' address and we should copy arguments beginning from sp+stackword_cost
	#define func_back1(func) func(20) func(19) func(18) func(17) func(16) func(15) func(14) func(13) func(12) func(11) func(10) func(9) func(8) func(7) func(6) func(5) func(4) func(3) func(2) func(1)

	#define func_back(func) func_back1(func) func(0)


#if !(defined ALWAYS_HAS_FRAME_POINTER)

	//We save arguments and previous sp in new stack
	#define STACK_COST(T) (T::stackword_cost + 1)

    //((arg_types::stack_padding_reporter & (((size_t)1)<<((size_t)(arg_types::stackword_cost-j))%WORDBITSIZE)) != 0) means the space is for padding(and lea is cheaper than push)
	// Here "%WORDBITSIZE" is to escape from shifting overflow warning...
    #define push_stack_define(j) if(arg_types::stackword_cost >= j ){\
		if((arg_types::stack_padding_reporter & (((size_t)1)<<((size_t)(arg_types::stackword_cost-j))%WORDBITSIZE)) != 0) \
				__asm__ __volatile__ ("pushq " MACRO_TOSTRING(j*WORDSIZE) "(%r10);\n\t");\
		else 	__asm__ __volatile__ ("leaq -" MACRO_TOSTRING(WORDSIZE) "(%rsp), %rsp;\n\t");}

	#define restore_stack_define(j) if(arg_types::stackword_cost == j){\
		__asm__ __volatile__ ("movq " MACRO_TOSTRING(j*WORDSIZE) "(%rsp), %rsp;\n\t");}

	#define call_with_stack_define(i) \
	template <MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames) typename R, bool has_variable_arguments > \
    FORCE_OPTIMIZATION(O1) FORCE_NOINLINE DLL_LOCAL R do_call (\
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types)  \
		void* dest_func, char* stack_base ){\
		typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)> arg_types; \
		if(arg_types::intreg_cost >= 6){\
			__asm__ __volatile__ (	"movq 	%0,  	%%r11;		\n\t" 	\
						::"m"(dest_func)		\
					);\
		}\
	\
		if(arg_types::stackword_cost == 0 && arg_types::intreg_cost <= 4){\
			__asm__ __volatile__ (	"movq 	%%rsp,  -" MACRO_TOSTRING(WORDSIZE) "(%0);		\n\t" 	\
						"leaq 	-" MACRO_TOSTRING(WORDSIZE) "(%0),  %%rsp;		\n\t" 	\
						::"X"(stack_base)	\
					);\
		}else{										\
			__asm__ __volatile__ (	"movq 	%%rsp,  %%r10;		\n\t" 	\
						"movq 	%0,  	%%rsp;		\n\t" 	\
						"pushq 	%%r10;				\n\t"   \
						::"X"(stack_base)		\
					);										\
			func_back1(push_stack_define)					\
		}\
	\
		if(arg_types::float_count > 0 && has_variable_arguments){\
			__asm__ __volatile__ (	"movq 	%0,  %%rax;			\n\t"	\
					:: "X"(arg_types::float_count));	\
		}\
	\
		if(arg_types::intreg_cost >= 6){					\
			__asm__ __volatile__ (	"callq 	*%r11;				\n\t");	\
		}else{												\
			__asm__ __volatile__ (	"callq	*%0					\n\t"	\
					::"r"(dest_func):"rax"						\
			);												\
		}													\
	\
		func_back(restore_stack_define)						\
		__asm__ __volatile__ ("retq;\n\t");                   \
		DUMMY_RETURN(R)					\
	}

#if defined ENABLE_PUSH_PRAGMA
	#pragma GCC push_options
	//disable -fipa-sra
	#pragma GCC optimize ("O1")
	//disable push rbp
	#pragma GCC optimize ("omit-frame-pointer")
	//We use this because we cannot use "naked" attribute in x86 and x64, we will use forced O2 optimization (function O2 attribute maybe ignored by some compilers) instead.
#endif // ENABLE_PUSH_PRAGMA

#else
	//We save arguments and previous sp in new stack
	#define STACK_COST(T) (T::stackword_cost )

    //((arg_types::stack_padding_reporter & (((size_t)1)<<((size_t)(arg_types::stackword_cost-j))%WORDBITSIZE)) != 0) means the space is for padding(and lea is cheaper than push)
	// Here "%WORDBITSIZE" is to escape from shifting overflow warning...
    #define push_stack_define(j) if(arg_types::stackword_cost >= j ){\
		if((arg_types::stack_padding_reporter & (((size_t)1)<<((size_t)(arg_types::stackword_cost-j))%WORDBITSIZE)) != 0) \
				__asm__ __volatile__ ("pushq " MACRO_TOSTRING(j*WORDSIZE+WORDSIZE) "(%rbp);\n\t");\
		else 	__asm__ __volatile__ ("leaq -" MACRO_TOSTRING(WORDSIZE) "(%rsp), %rsp;\n\t");}

	#define restore_stack_define(j) if(arg_types::stackword_cost == j){\
		__asm__ __volatile__ ("movq %rbp, %rsp;\n\t");  __asm__ __volatile__ ("pop %rbp;\n\t");}

	#define call_with_stack_define(i) \
	template <MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames)typename R, bool has_variable_arguments > \
	 FORCE_NOINLINE DLL_LOCAL R do_call (\
		MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types)  \
		void* dest_func, char* stack_base ){\
		typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)> arg_types; \
		__asm__ __volatile__ ("":::"rax","rdi","rsi","rdx","rcx","r8","r9"); \
        __asm__ __volatile__ ("movq 	%0, %%rsp;		\n\t" 	\
                ::"X"(stack_base));			\
        func_back1(push_stack_define)			\
		if(arg_types::float_count > 0 && has_variable_arguments){\
			__asm__ __volatile__ (	"movq 	%0,  %%rax;			\n\t"	\
					:: "X"(arg_types::float_count));	\
		}\
		__asm__ __volatile__ ("":::"rax","rdi","rsi","rdx","rcx","r8","r9"); \
		__asm__ __volatile__ ("call 	*%0;			\n\t" 	\
					::"X"(dest_func));			\
		func_back(restore_stack_define)			\
		__asm__ __volatile__ ("retq;\n\t");                   \
		DUMMY_RETURN(R)					\
	}

#if defined ENABLE_PUSH_PRAGMA
	#pragma GCC push_options

	#pragma GCC optimize ("O0")
	//force push rbp
	#pragma GCC optimize ("no-omit-frame-pointer")

#endif // ENABLE_PUSH_PRAGMA

#endif // ALWAYS_HAS_FRAME_POINTER

	BATCH_FUNC(call_with_stack_define)

#if defined ENABLE_PUSH_PRAGMA
	#pragma GCC pop_options
#endif // ENABLE_PUSH_PRAGMA

	#undef call_with_stack_define
	//#undef push_stack_define
	//#undef restore_stack_define
	//#undef func_back
	//#undef func_back1

	#define DEF_SP(sp_value) DECL_REG_VAR(size_t, sp_value, rsp)
	//Maybe your compiler does not support register variable? use DEF_SP_BAK instead!
	#define GET_SP(sp_value) __asm__ __volatile__ ("movq 	%%rsp,  %0;	\n\t" : "=X"(sp_value))
	#define DEF_SP_BAK(sp_value) size_t sp_value; GET_SP(sp_value)
}

#endif


#endif
