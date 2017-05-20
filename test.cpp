#include <iostream>
#include "stdlib.h"
#include "stdio.h"

using namespace std;
#include "call_in_stack.h"
#ifdef __x86_64__
typedef long double doublewordtype;
#else
typedef long double doublewordtype;
#endif
void xxz(int a, float b, short c, long d, double e, doublewordtype f,int& g, const int& h,int* i, const int* j){

	/*cout << a << b << c << d << e << f << g << h << *i << *j << endl;*/
}
int z = 3;
//#define void int
void bb(){
	++z;
	cout << z << endl;
	cout << endl;
}
void bb1(int& c){
	++z;
	cout << z << endl;
	cout << c << endl;
}
void bb2(int &c, int d){
	++z;
	cout << z << endl;
	cout << c << endl;
	cout << d << endl;
}

#ifdef ENABLE_RIGHT_VALUE_REFERENCE
#include <utility>
int&& b3(int &&c, const int&& d){
    cout << c << d << endl;
    cout << "right_value_test" << endl;
    return std::move(c);
}

 struct global_struct1{
    global_struct1(){
        int x = 3, y = 6;
        cout << call_in_stack(b3, 3, 6);
		//C++ 0x support left value reference binding to right value reference
		//But C++ 11 does not.
       // cout << call_in_stack(b3, 3, y);
        cout << call_in_stack(b3, std::move(x), std::move(y)) << endl;
    }
}_test1;

#endif

__attribute__ ((noinline))  void tb(int a, int b, int c, int d, int e, int f,int g, short h,int i, short j, int k = 0, char l = 0){
 	cout  << &a  << endl;
	cout  << &g  << endl;
	cout << &h  << endl;
	cout << &i << endl;
	cout << &j << endl;
	cout << &k << endl;
	cout<< &l << endl;
}

short t0(int a, int b, double c, const float d, const volatile double e, const double f,int &g, const int& h,int* i, const int& j){
    doublewordtype xx = f;
	cout << xx << a << b << c << d << e << f << g << h << *i << j << f << endl;
	cout <<"typical0"  << endl;
	cout << endl;
    return 0;
	// about 4kB stack cost
}
double t8(int a, float b, double c, const float d, const volatile double e, const double f,int &g, const int& h,int* i, const int& j){
    doublewordtype xx = f;
	cout << xx << a << b << c << d << e << f << g << h << *i << j << f << endl;
	cout <<"typical8"  << endl;
	cout << endl;
	return -8;
	// about 4kB stack cost
}
void t1(doublewordtype a, float b, double c, const float d, const volatile double e, const double f,int &g, const int& h,int* i, const int& j){
    doublewordtype xx = f;
	cout <<"typical1 begin"  << endl;
	cout <<  a << endl;
	cout << b << c << d << xx << endl;
	cout << e << endl;
	cout<< f << g << h << *i << j << f << endl;
	cout <<"typical1 end"  << endl;
	#ifdef __x86_64__
	printf("%d%Lf\n",*i,a);
	#else
	printf("%d%Lf\n",*i,a);
	//printf("%d%lld\n",*i,a);
	#endif

	cout << endl;
	// about 4kB stack cost
}
typedef int(*new_type)(const char*);
new_type t2(int a, float b, doublewordtype  c, long d, volatile int e, const double f,int &g, const int& h,int* i, const int& j){
    doublewordtype xx = f;
	//cout << &xx << &a << &b << &c << &d << &e << &f << &g << &h << &i << &j << &xx << endl;
	cout << xx << a << b << c << d << e << f << g << h << *i << j << xx << endl;
	cout <<"typical2"  << endl;
	#ifdef __x86_64__
	printf("%d%d%Lf\n",*i,j,c);
	#else
	printf("%d%d%Lf\n",*i,j,c);
	//printf("%d%d%lld\n",*i,j,c);
	#endif

	cout << endl;
	return &puts;
	// about 4kB stack cost
}

int t3(int a, float b, short c, long d, int e, const double f,int &g, const int& h,int* i, doublewordtype j){
    doublewordtype xx = f;
	cout << xx << a << b << c << d << e << f << g << h << *i << endl;
	cout <<"typical3"  << endl;
	cout << j << xx << endl;
	#ifdef __x86_64__
	printf("%d%Lf\n",*i,j);
	#else
	printf("%d%Lf\n",*i,j);
	//printf("%d%lld\n",*i,j);
	#endif
	cout << g << endl;
	return g;
	// about 4kB stack cost
}

char t4(int a, long long b, short c, long d, volatile int e, long long f,int &g, const int& h,int* i, doublewordtype j){
    doublewordtype xx = f;
	cout << &i << endl;
	cout << &j << endl;
	cout << xx << a << b << c << d << e << f << g << h << *i << endl;
	cout <<"typical4"  << endl;
	cout << j << xx << endl;
	#ifdef __x86_64__
	printf("%d%Lf\n",*i,j);
	#else
	printf("%d%Lf\n",*i,j);
	//printf("%d%lld\n",*i,j);
	#endif
	cout << endl;
	return 'a';
	// about 4kB stack cost
}
void long_double_test(){
	//cout << (long double)123456789.1234567 << endl;
	printf("%Lf\n",(long double)123456789.1234567);
}
char buf[24*1024];
int ibuf[2048];
void double_jump(int a, float b, doublewordtype c, long d, double e, const double f,int &g, const int& h,int* i, const int& j){
	call_in_stack_safe(buf, sizeof(buf), t1, 1,2,3,4,5,6,++z, z, &z, z);
	cout << "OK" << endl;
	call_in_stack_safe(buf, sizeof(buf), t3, a,b,c,d,e,f,g,h,i,j);
	cout << "OK" << endl;
	call_in_stack_safe(ibuf,t2, a,b,c,d,e,f,g,h,i,j );
	cout << "OK" << endl;
	call_in_stack_safe(ibuf, t1, 1,2,3,4,5,6,++z, z, &z, z );
	cout << "OK" << endl;
	call_in_stack_safe(buf, bb );
	cout<< "End" << endl;
}

void double_jump1(int a, float b, doublewordtype c, long d, double e, const double f,int &g, const int& h,int* i, const int& j){
	call_in_stack_safe(buf, sizeof(buf), t1, 1,2,3,4,5,6,++z, z, &z, z);
}
struct ag{ag(int){};};
void ff(ag);
#include "cmath"
#include <string>
#include <stdarg.h>

struct class_test_v{
    int p_src_v;
    class_test_v():p_src_v(1234567890){}
    virtual int vdummy(){
        if(p_src_v == 1234567890){
            cout << "functor_test: " << p_src_v  << endl;
        }
        else{
            throw p_src_v;
        }
		return 4444;
    }
    int dummy(){
        if(p_src_v == 1234567890){
            cout << "functor_test: " << p_src_v  << endl;
        }
        else{
            throw p_src_v;
        }
		return 4321;
    }
};

struct class_test_a : public virtual class_test_v{
    int p_src_a;
    class_test_a():p_src_a(123456789){}
    int printa(const char * format, const char* str, int d){
        printf(format, str, d);
        if(p_src_a == 123456789){
            cout << "functor_test: " << p_src_a  << endl;
        }
        else{
            throw p_src_a;
        }
		return 8844;
    }

    virtual int vdummya(){
        if(p_src_a == 123456789){
            cout << "functor_test: " << p_src_a  << endl;
        }
        else{
            throw p_src_a;
        }
		return 8844;
    }

    virtual int vprinta(const char * format, const char* str, int d){
        printf(format, str, d);
        if(p_src_a == 123456789){
            cout << "functor_test: " << p_src_a  << endl;
        }
        else{
            throw p_src_a;
        }
		return 8844;
    }
};

struct class_test_b: public virtual class_test_v{
    int p_src_b;
    class_test_b():p_src_b(987654321){}
    int printb(const char * format, const char* str, int d){
        printf(format, str, d);
        if(p_src_b == 987654321){
           cout << "functor_test: " << p_src_b  << endl;
        }
        else{
            throw p_src_b;
        }
		return 4488;
    }

    virtual int vprintb(const char * format, const char* str, int d){
        printf(format, str, d);
        if(p_src_b == 987654321){
           cout << "functor_test: " << p_src_b  << endl;
        }
        else{
            throw p_src_b;
        }
		return 4488;
    }
};

struct empty_test{
    char test[13];
};
struct class_test: public empty_test, public class_test_b, public class_test_a{
	int src;
	int operator()(const char * format, const char* str, int d){
        printf(format, str, d);
		cout << "functor_test: " << src  << endl;
		return 8866;
	}

    virtual int vprinta(const char * format, const char* str, int d){
        printf(format, str, d);
        cout << "functor_test: " << p_src_a  << endl;
		return 1234;
    }
}functor_test;

FORCE_NOINLINE void float_output(double testnum){
	//double x = testnum1;
	printf("%f%f%f%f%f%f%f%f%f%f\n"
	,testnum,testnum,testnum,testnum,testnum,testnum,testnum,testnum,testnum,testnum);
}

int main(){
	float_output(1.2);
	cout << "You should see \"TEST PASSED\" in the end if your compiling environment test passed! "  << endl;

	/* cout <<change_ref_to_pointer_size<doublewordtype>::size<< _COUNT_OF_SIZE(change_ref_to_pointer_size<doublewordtype>::size, WORDSIZE)<<endl;
	cout << _ALIGNED_COST(_COUNT_OF_SIZE(change_ref_to_pointer_size<doublewordtype>::size, WORDSIZE), 2)<<endl;
	args_list <int , float , doublewordtype  , long , volatile int , const double ,int &, const int& ,int* , const int& >::out_stackword_cost(cout);
	args_list <int , float , doublewordtype  , long , volatile int , const double ,int &, const int& ,int* , const int& >::out_stack_padding_reporter(cout); */

    char *d = (char*)malloc(sizeof(buf));
    call_in_stack_safe(d, sizeof(buf), bb );
	call_in_stack(buf, sizeof(buf) , bb );
	call_in_stack(buf, bb );
	call_in_stack( buf, &bb1, z );
	call_in_stack(buf, bb2, z, 134);
	//call_with_stack_exp_class<void>::call_with_stack_exp(z, 134,  buf + sizeof(buf) - 8, bb2, bb1);
	t1(12345.6789,2,3,4,5,6,++z, z, &z, z);
	call_in_stack( buf, sizeof(buf), t1, 12345.6789,2,3,4,5,6,++z, z, &z, z);

	//call_in_stack_impl::function_property<typeof(&t1)>::arg_list_type::out_stackword_cost(cout);
	call_in_stack(buf ,t0, 1,2,3,4,5,6,++z, z, &z, z);
	call_in_stack(buf ,t1 ,9.87654321,2,3,4,5,6,++z, z, &z, z);
	call_in_stack(buf ,t8 ,1,2,3,4,5,6,++z, z, &z, z);
	call_in_stack(buf ,sizeof(buf), t2, 1,2,3,4,5,6,++z, z, &z, 5 )("This is a jock");
	call_in_stack(buf ,t2, 1,2,3,4,5,6,++z, z, &z, 5);

	call_in_stack(d, sizeof(buf), bb );
	call_in_stack(d, sizeof(buf), t1 , 1,2,3,4,5,6,++z, z, &z, z );
	call_in_stack(d, sizeof(buf), bb );

	free(d);
	call_in_stack_safe(buf, sizeof(buf), double_jump, 1,2,3,4,5,6,++z, z, &z, z);
	call_in_stack_safe(buf, double_jump, 1,2,3,4,5,6,++z, z, &z,z);
	call_in_stack_safe(buf, double_jump1, 1,2,3,4,5,6,++z, z, &z, z);

	// cout << args_list<int , long long , short, long, volatile int, long long ,int &, const int& ,int* >::stackword_cost << endl;
	// cout << args_list<int , long long , short, long, volatile int, long long ,int &, const int& ,int* , doublewordtype>::stackword_cost << endl;
	t3(1,2,3,4,5,6,z, z, &z, z );
	call_in_stack(buf , t3, 1,2,3,4,5,6,++z, z, &z, z) ;
	call_in_stack_safe(buf, t4, 1,2,3,4,5,6,++z, z, &z, z);

	call_in_stack(8*1024, t1, 1,2,3,4,5,6,++z, z, &z, z );

	cout << call_in_stack(buf, t3, 1,2,3,4,5,6,++z, z, &z, z ) << endl;
	//cout << function_property<typeof(&printf)>::arguments_count << endl;
	printf( "%d%d%p%f%d\n", 1, z, &z, (double)z, z+1);
	cout << call_in_stack_impl::args_list<char const*, int, int, int*, double, int>::stackword_cost << endl;
	call_in_stack(buf, &printf, "%d%d%p%f%d\n", 1, z, &z, (double)z, z+1);
	call_in_stack(buf, &printf, "%d%d%p%f%d%s\n", 1, 2, buf, 3.0, 5, "Hello world!");
	call_in_stack(buf, &printf, "%d%d%p%Lf%d%s\n", 1, 2, buf, 3.0L, 5, "Hello world!");
	double (*plog)( double ) = &log;
	cout << call_in_stack(4*1024, plog, 2.0) << endl;
	float (*pflog)( float ) = &log;
	cout << call_in_stack(12*1024,pflog, (float)2.0) << endl;
	cout << call_in_stack((float (*)( float ))(&log), (float)2.0) << endl;
	call_in_stack(4*1024, long_double_test);

	string test_string("Hello world");
	cout << call_in_stack(from_member_fun(test_string,size)) << endl;
    cout << call_in_stack(from_nonvirtual_member_fun(test_string,size)) << endl;
    cout << call_in_stack(from_member_fun(test_string,c_str)) << endl;
    //sorry I can not support overloaded member function like following "at", because I can not give a convenient interface for you.
    //cout << call_in_stack(from_member_fun(test_string,at) , 5) << endl;
    call_in_stack(from_member_fun(test_string, reserve , 8));
    call_in_stack(from_nonvirtual_member_fun(test_string, reserve , 8));
    cout << call_in_stack(from_member_fun(test_string,data)) << endl;
	functor_test.src = 1024;
	cout << call_in_stack(from_functor(functor_test,"%s%d\n", "Hello boy,", 1024))<< endl;
	functor_test.src = 4201;
	cout << call_in_stack(from_functor(functor_test,"%s%d\n", "Hello boy,", 1024)) << endl;

	cout << call_in_stack(from_virtual_member_fun(functor_test,vprinta, "%s%d\n", "Hello boy,", 1024)) << endl;
	cout << call_in_stack(from_virtual_member_fun(functor_test,vprintb, "%s%d\n", "Hello boy,", 1024)) << endl;
    cout << call_in_stack(from_virtual_member_fun(functor_test,vdummy)) << endl;

    cout << call_in_stack(from_nonvirtual_member_fun(functor_test,dummy)) << endl;
    cout << call_in_stack(from_nonvirtual_member_fun(functor_test,printa, "%s%d\n", "Hello boy,", 1024)) << endl;
	cout << call_in_stack(from_nonvirtual_member_fun(functor_test,printb, "%s%d\n", "Hello boy,", 1024)) << endl;

	cout << "Ok !" << endl;
    cout << "GCC_VERSION:" << GCC_VERSION << endl;
	cout << "\"TEST PASSED\""  << endl;

	return 0;
}
