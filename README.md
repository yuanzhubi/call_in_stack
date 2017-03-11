For better page experience, see https://github.com/yuanzhubi/call_in_stack/wiki/Welcome-to-call_in_stack.

call_in_stack enables you calling function in a new stack! It is very useful in coroutines programming to save memory.

call_in_stack 让你在一个新的堆栈中调用函数！协程编程中，这对于节省内存可非常有用。

Coroutines programming is now widely used now in network programming. It reduces the complexity of non-blocking I/O multiplexing. Stackful coroutines enable you writing non-blocking network server like blocking network server. See libgo,libco.

协程编程现在已经在网络编程中广泛使用。它减少了非阻塞io多路复用的难度。有栈协程能让你用有阻塞网络服务器的语法去写一个无阻塞网络服务器。可以去看看libgo,libco。

But the stack of a stackful coroutine must pre-allocated before its running. For safety, it is usually a large number of memory(usually 64KB-128KB? To print/cout a long double number costs above 6KB stack in glibc of x64 platform). It lows downs the maximum number of coroutines.

但是我们必须为一个有栈协程在他执行之前分配好堆栈。为了安全，这个堆栈得是一大块内存（一般64KB到128KB？要知道在x64 glibc里，print或者cout一个long double浮点数会消耗超过6KB栈）。这降低了可创建的协程的最大数量、

Sometimes, the most expensive stack cost only happens in some rare case, for example, error dealings. So we must always waste memory to prepare for some unusual case. Another case is recursive function (for example, quick sort algorithm). The size of stack it costs is a variable(The worst case of quick sort of a n-length array is O(n)), but we may not know the variable before the coroutine running，so we have to prepare stack for its maximum value.

有时候，特别消耗堆栈的一般都是一些稀有场景，例如，错误处理。所以我们不得不总是为了预备处理一些不太容易发生的情况而浪费内存。还有一种情况是递归函数(例如快速排序算法)。它需要消耗的堆栈数量是一个变量（对一个n长数组执行快排最坏的时候消耗O(n)栈内存），但是我们可能在协程执行前无法知道这个变量的值（例如你需要parse一个用户输入的json并排序输出）, 所以我们不得不按它可能出现的最大值去准备堆栈。

call_in_stack is aimed at above problems. It enables you calling a function in a new stack and cost only O(1) stack of the caller. The new stack can be any writable memory, either global array or local array in stack of main function or result of new/malloc with variable size. It enables delaying the stack allocation for accurate allocation.

call_in_stack 就是用来解决上述问题的，他允许你用一个新的堆栈来调用一个函数，只需消耗调用者O(1)的堆栈。这个新堆栈可以是任何一块可写的内存，可能是一个全局数组或main函数堆栈下的局部数组，也可能是new或malloc的分配的运行时变长空间。它通过允许延迟堆栈分配来实现精确的堆栈分配。

Examples:
#include"call_in_stack.h"
char buf[12*1024];

void let_us_call_in_stack(){

int x = 5;

call_in_stack(buf, &printf, "%d%d%p%f%d%s\n", 1, x, buf, 3.0, x++, "Hello world!");

//printf will run using buf as stack.

char* new_buf = new char[12*1024];

call_in_stack(new_buf, 12*1024, &printf, "%d%d%p%f%d%s\n", 1, x, buf, 3.0, ++x, "Hello world!");

//printf will run using new_buf as stack.

delete []new_buf;

call_in_stack(12*1024, &printf, "%d%d%p%f%d%s\n", 1, x, buf, 3.0, ++x, "Hello world!");

//printf will run using auto allocated 12*1024 KB memory as stack.

call_in_stack(&printf, "%d%d%p%f%d%s\n", 1, x, buf, 3.0, ++x, "Hello world!");

//printf will run using auto allocated default 32*1024 KB memory as stack.

//call_in_stack may calls a function A that also call_in_stack function B while both call_in_stack calling use same global array as stack. This is dangerous!

call_in_stack_safe(buf, &printf, "%d%d%p%f%d%s\n", 1, x, buf, 3.0, ++x, "Hello world!");

//If let_us_call_in_stack does not run using buf as stack, printf will run using buf as stack, or else printf will run in the stack of let_us_call_in_stack.

//call_in_stack_safe will check whether the caller and the callee function use the same stack, which is useful in nested using same stack.

}

Tips:

1.call_in_stack now only supports x86 & x64 in sysv abi with g++ compiler(CLANG or INTEL c++ compiler may be also supported, but I do not test it for lacking utility. Warmly welcome for your help!).

2.call_in_stack is type safe.

要点：

1.call_in_stack 目前只支持在x86和x64上的sysv abi并使用g++编译器，clang和intel编译器也可能是支持的，但是我缺乏必要的测试设施。热烈欢迎你们的帮助!

2.call_in_stack是类型安全的。

Usage limit:

We define: the signed or unsigned integer type char, wchar_t, short, int, long, long long; float number type float, double, long double; pointer or C++ reference of any type; const or volatile modified type of all previous type; are "basic_type".

call_in_stack can call a function f in a new stack unless:

f is declared with all of its arguments in basic_type, either f is declared with fixed or not fixed number of arguments.

f is called with no more than 10 arguments, either f is declared with fixed or not fixed number of arguments.

You need to point out the template argument of f, if it is a template function.

The return type of f is either basic_type or void.

f throws C++ exception will leads to undefined behaviors. Because stack unwind will not work properly. You can use a function F wrapping f to catch exception and do not re-throw. Then use call_in_stack with F instead of f.

If f has "function overloading", you should use a function pointer p with proper type to point at f, then use call_in_stack with p instead of the function name of f; or cast f to the type explicitly. For example,

#include "cmath"

double (*plog)( double ) = &log;

call_in_stack(4*1024, plog, 2.0);

call_in_stack((float (*)( float ))(&log), 2.0);

使用限制

我们定义：有符号或者无符号的整数类型char, wchar_t, short, int, long, long long; 浮点数类型float, double, long double; 任何指针或者引用类型;任何前述类型的const 或者volatile类型 统称为“基本类型”。

call_in_stack 可以在一个新的栈调用一个函数f, 只要能满足以下条件：

f 被声明为只拥有基本类型参数，不管他拥有定长或者变长参数列表。

f 被调用时只有不超过10个参数，不管他拥有定长或者变长参数列表。

如果f是一个模版函数，你需要显示指定模版参数。

f 的返回类型需要是基本类型或者void。

f 如果抛出异常会导致未定义行为。因为堆栈回卷不能正确的工作。你可以定义一个包装函数F把f包起来捕获异常并且不让异常被重新抛出。此时call_in_stack就应该调用F而不是f了。

如果f有被函数重载，那你需要用一个有合适类型的函数指针p指向f, 然后call_in_stack去调用p; 或者直接对f做类型转换。例如

#include "cmath"

double (*plog)( double ) = &log;

call_in_stack(4*1024, plog, 2.0);

call_in_stack((float (*)( float ))(&log), 2.0);
