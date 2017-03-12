#ifndef _TEMPLATE_UTIL_H_
#define _TEMPLATE_UTIL_H_

#define MACRO_TOSTRING_0(x) #x
#define MACRO_TOSTRING(x) MACRO_TOSTRING_0(x)

#define MACRO_JOIN(X,Y) X##Y

#define BATCH_FUNC1(xfunc) xfunc(1) \
xfunc(2) \
xfunc(3) \
xfunc(4) \
xfunc(5) \
xfunc(6) \
xfunc(7) \
xfunc(8) \
xfunc(9) \
xfunc(10)

#define BATCH_FUNC(xfunc) xfunc(0) BATCH_FUNC1(xfunc)

#define RECURSIVE_FUNC_0(funcbegin, func, funcend) funcbegin(0)
#define RECURSIVE_FUNC_1(funcbegin, func, funcend) RECURSIVE_FUNC_0(funcbegin, func, func) funcend(1)
#define RECURSIVE_FUNC_2(funcbegin, func, funcend) RECURSIVE_FUNC_1(funcbegin, func, func) funcend(2)
#define RECURSIVE_FUNC_3(funcbegin, func, funcend) RECURSIVE_FUNC_2(funcbegin, func, func) funcend(3)
#define RECURSIVE_FUNC_4(funcbegin, func, funcend) RECURSIVE_FUNC_3(funcbegin, func, func) funcend(4)
#define RECURSIVE_FUNC_5(funcbegin, func, funcend) RECURSIVE_FUNC_4(funcbegin, func, func) funcend(5)
#define RECURSIVE_FUNC_6(funcbegin, func, funcend) RECURSIVE_FUNC_5(funcbegin, func, func) funcend(6)
#define RECURSIVE_FUNC_7(funcbegin, func, funcend) RECURSIVE_FUNC_6(funcbegin, func, func) funcend(7)
#define RECURSIVE_FUNC_8(funcbegin, func, funcend) RECURSIVE_FUNC_7(funcbegin, func, func) funcend(8)
#define RECURSIVE_FUNC_9(funcbegin, func, funcend) RECURSIVE_FUNC_8(funcbegin, func, func) funcend(9)
#define RECURSIVE_FUNC_10(funcbegin, func, funcend) RECURSIVE_FUNC_9(funcbegin, func, func) funcend(10)
#define RECURSIVE_FUNC_11(funcbegin, func, funcend) RECURSIVE_FUNC_10(funcbegin, func, func) funcend(11)


#define BI_ONE_BATCH_FUNC1(join_func, i, xyfunc) join_func(i, 1,  xyfunc) \
join_func(i, 2,  xyfunc) \
join_func(i, 3,  xyfunc) \
join_func(i, 4,  xyfunc) \
join_func(i, 5,  xyfunc) \
join_func(i, 6,  xyfunc) \
join_func(i, 7,  xyfunc) \
join_func(i, 8,  xyfunc) \
join_func(i, 9,  xyfunc) \
join_func(i, 10,  xyfunc)

#define BI_ONE_BATCH_FUNC(join_func, i, xyfunc) join_func(i, 0, xyfunc) BI_ONE_BATCH_FUNC1(join_func, i, xyfunc)

#define REC_FUNC_0(i, xyfunc) xyfunc(i, 0)
#define REC_FUNC_1(i, xyfunc) REC_FUNC_0(i, xyfunc) xyfunc(i, 1)
#define REC_FUNC_2(i, xyfunc) REC_FUNC_1(i, xyfunc) xyfunc(i, 2)
#define REC_FUNC_3(i, xyfunc) REC_FUNC_2(i, xyfunc) xyfunc(i, 3)
#define REC_FUNC_4(i, xyfunc) REC_FUNC_3(i, xyfunc) xyfunc(i, 4)
#define REC_FUNC_5(i, xyfunc) REC_FUNC_4(i, xyfunc) xyfunc(i, 5)
#define REC_FUNC_6(i, xyfunc) REC_FUNC_5(i, xyfunc) xyfunc(i, 6)
#define REC_FUNC_7(i, xyfunc) REC_FUNC_6(i, xyfunc) xyfunc(i, 7)
#define REC_FUNC_8(i, xyfunc) REC_FUNC_7(i, xyfunc) xyfunc(i, 8)
#define REC_FUNC_9(i, xyfunc) REC_FUNC_8(i, xyfunc) xyfunc(i, 9)
#define REC_FUNC_10(i, xyfunc) REC_FUNC_9(i, xyfunc) xyfunc(i, 10)

#define CONCAT_FUNC(i, j, k) MACRO_JOIN(REC_FUNC_, i)(j, k)

#define BI_TWO_BATCH_FUNC1(i, xyfunc) BI_ONE_BATCH_FUNC1(CONCAT_FUNC, i, xyfunc)
#define BI_TWO_BATCH_FUNC(i, xyfunc) BI_ONE_BATCH_FUNC(CONCAT_FUNC, i, xyfunc)


#define define_args_begin(i)
#define define_args(i)  MACRO_JOIN(a,i),
#define define_args_end(i)  MACRO_JOIN(a,i)

#define define_types_begin(i)
#define define_types(i) MACRO_JOIN(t,i),
#define define_types_end(i) MACRO_JOIN(t,i)

#define define_rtypes_begin(i)
#define define_rtypes(i) typename function_property<T>::MACRO_JOIN(type,i),
#define define_rtypes_end(i) typename function_property<T>::MACRO_JOIN(type,i)

#define define_typenames_begin(i)
#define define_typenames(i) typename MACRO_JOIN(t,i),
#define define_typenames_end(i) typename MACRO_JOIN(t,i)

#define define_typenames_ex_begin(i)
#define define_typenames_ex(i) typename MACRO_JOIN(tt,i),
#define define_typenames_ex_end(i) typename MACRO_JOIN(tt,i)

#define define_rtype_ex_begin(i)
#define define_rtype_ex(i) typename remove_cvref<MACRO_JOIN(tt,i)>::type,
#define define_rtype_ex_end(i) typename remove_cvref<MACRO_JOIN(tt,i)>::type

#define define_type_ex_begin(i)
#define define_type_ex(i) ,MACRO_JOIN(tt,i)
#define define_type_ex_end(i) ,MACRO_JOIN(tt,i)

#define define_type_args_ex_begin(i)
#define define_type_args_ex(i)  , MACRO_JOIN(tt,i) MACRO_JOIN(aa,i)
#define define_type_args_ex_end(i) ,MACRO_JOIN(tt,i) MACRO_JOIN(aa,i)

#define define_args_ex_begin(i)
#define define_args_ex(i)  MACRO_JOIN(aa,i),
#define define_args_ex_end(i) MACRO_JOIN(aa,i)

#define define_args_ex_c_begin(i)
#define define_args_ex_c(i)  ,MACRO_JOIN(aa,i)
#define define_args_ex_c_end(i) ,MACRO_JOIN(aa,i)


#define define_typeargs_begin(i)
#define define_typeargs(i)  , typename function_property<T>::MACRO_JOIN(type,i)  MACRO_JOIN(a,i)
#define define_typeargs_end(i) , typename function_property<T>::MACRO_JOIN(type,i)  MACRO_JOIN(a,i)

#define typedefine_types_begin(i)
#define typedefine_types(i) typedef MACRO_JOIN(t,i) MACRO_JOIN(type,i);
#define typedefine_types_end(i) typedefine_types(i)

#define define_parent_begin(i) define_types_begin(i)
#define define_parent(i) define_types(i)
#define define_parent_end(i) void

template <typename C>
struct remove_cvref{
	typedef C type;
};

template <typename C>
struct remove_cvref<const C>{
	typedef C type;
};

template <typename C>
struct remove_cvref<volatile C>{
	typedef C type;
};

template <typename C>
struct remove_cvref<C&>: public remove_cvref<C>{
};

template <typename C>
struct change_ref_to_pointer_size{
	typedef C type;
	const static int size = sizeof(C);
};
template <typename C>
struct change_ref_to_pointer_size<C&>{
	typedef C type;
	const static int size = sizeof(C*);
};

struct threechar{
	char c[3];
	template<typename T>static char is_class_test(int T::*);
	template<typename T>static threechar is_class_test(...);
};

template<bool t>
struct static_asserter;

template<>
struct static_asserter<true>{typedef int* type;};

#define STATIC_ASSERTER(name, value) typedef static_asserter<value> name;

template <typename T>
struct assert_not_class{
	STATIC_ASSERTER(not_class, sizeof(threechar::is_class_test<T>(0)) != sizeof(char));
};

template <typename T, unsigned int max_type_size>
struct assert_not_large_type{
	STATIC_ASSERTER(not_large_type, max_type_size >= sizeof(T));
};

//struct parameter, or parameter size beyond expection is not enabled.
template <typename T, unsigned int max_type_size>
struct assert_not_class_not_largesize :
	public assert_not_class<T>, public assert_not_large_type<T, max_type_size>{
};

template <typename Ptr>
struct predicate_attr;

template <typename R, typename O>
struct predicate_attr<R (*)(O)>{
	typedef R return_type;
	typedef O object_type;
};

template <typename R, typename O, typename T>
struct predicate_attr<R (T::*)(O)>{
	typedef T class_type;
	typedef R return_type;
	typedef O object_type;
};

template <
	typename t1=void, typename t2=void, typename t3=void, typename t4=void, typename t5=void, typename t6=void, typename t7=void, typename t8=void, typename t9=void, typename t10=void,
	typename t11=void/*, typename t12=void, typename t13=void, typename t14=void, typename t15=void, typename t16=void, typename t17=void, typename t18=void, typename t19=void, typename t20=void,
	typename t21=void, typename t22=void, typename t23=void, typename t24=void, typename t25=void, typename t26=void, typename t27=void, typename t28=void, typename t29=void, typename t30=void*/>
struct args_list;



template <>
struct args_list<
	void, void, void, void, void, void, void, void, void, void,
	void/*, void, void, void, void, void, void, void, void, void,
	void, void, void, void, void, void, void, void, void, void*/>
{
	const static int max_int_reg_cost_x64_system_v = 6;
	const static int max_float_reg_cost_x64_system_v = 8;

	const static int intreg_cost = 0;
	const static int floatreg_cost = 0;
	const static int stackword_cost = 0;

	const static int float_count = 0;

	//it reports in arguments stack style: the last bit(stack_padding_reporter%2) will be one if the last word in stack cost is not wasted for padding.
	const static int stack_padding_reporter = 0;

	//following function are used for development debug. It does not generate codes if you do not use them.
	template<typename O>
	static void out_stackword_cost(O&  ){}
	template<typename O>
	static void out_stack_padding_reporter(O&  ){}
};

template<typename C>
struct function_property;

#define class_define(i) template<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames) typename R> \
struct function_property<R (*)(MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)) > \
{\
	typedef args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)> arg_list_type;\
	MACRO_JOIN(RECURSIVE_FUNC_,i)(typedefine_types_begin, typedefine_types, typedefine_types_end)\
	typedef R return_type;\
	static const int arguments_count = i;\
	static const int stackword_cost = args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)>::stackword_cost;\
	static const int floatreg_cost = args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)>::floatreg_cost;\
	static const int intreg_cost = args_list<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)>::intreg_cost;\
	static const bool has_variable_arguments = false;\
}; \
template<MACRO_JOIN(RECURSIVE_FUNC_,i)(define_typenames_begin, define_typenames, define_typenames) typename R> \
struct function_property<R (*)(MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types)...) > \
	:public function_property<R (*)(MACRO_JOIN(RECURSIVE_FUNC_,i)(define_types_begin, define_types, define_types_end)) >\
{\
	static const bool has_variable_arguments = true;\
};

BATCH_FUNC(class_define)

//let z = _ALIGNED_BY(x,y), then z % y == 0  && z >= x
#define _ALIGNED_BY(x,y) (((x)+(y)-1)&~((y)-1))

#define _COUNT_OF_SIZE(new_type_size, wordsize) (_ALIGNED_BY(new_type_size, wordsize) / (wordsize))

//prev_cost % new_cost != 0 ? No!!!!
//So _ALIGNED_BY(prev, (new_cost)) - (prev) is ready for padding.
#define _ALIGNED_COST(prev, new_cost) _ALIGNED_BY((prev), (new_cost)) - (prev) + (new_cost)

#define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)
#if GCC_VERSION >= 40500
#define dummy_return(r_type)  __builtin_unreachable();
#else
#define dummy_return(r_type) return (r_type)(0);
#endif

#endif
