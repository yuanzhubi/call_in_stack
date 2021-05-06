Welcome to call in stack!

Call a function in a new stack that allocated anywhere. Do not be afraid of stack limit in your coroutines! Try to make your stack shareable between all coroutines!

Attention! It is not a coroutine library, but a library to help your stackful coroutine work more effeicently in memory cost.

```bash
#include "call_in_stack.h"
char buffer[4096];
call_in_stack(buffer, &printf, "Hello world!\n");
//Now printf run in buffer as its stack!
```

Goto https://github.com/yuanzhubi/call_in_stack/wiki/Welcome-to-call_in_stack for details.

Goto QQ Group :293767138(talk_in_stack) for further discussion.

---------------------------------------------------------------------------------------------------------------
Q:Why it is called "call_in_stack"?

A:It behaves like the impelment of linux system call(the kernel space codes must run in a new stack to avoid user codes know its temporate result saved in memeroy) but it supports more(either the count or the type) arguments and type safe. What's more, the X86 Linux kernel uses the name "call_on_stack" while the arm kernel uses "call_with_stack". So we choose "call_in_stack" ^_^!

Q:When it is used?

A:Using it, you can mannually control your stack allocation to avoid "stack over flow"(You can call_in_stack your function in heap!). It can be used for function calling that never leads to stackful coroutines switching then the function call do not cost stack of current coroutine. It is also useful for deep recursive calling like regular expressions analysis. 

What's more, it can be used for designing a new stackless coroutine library by caller allocating new stack for callee(but the caller do not know the accurate stack need of the callee so the size is rough)  in c++03, while in std::experimental::coroutine the callee allocating storage only for the arguments and local variables that will be used(resumed) after await or yield, with more accurate space cost but c++17 needed.


---------------------------------------------------------------------------------------------------------------

Version 1.0.6: Important adaptive improvement for lower version of GCC!

In this version:

We add support of using call_in_stack in x64 of old GCC version ( tested since GCC 3.4),  compiled with no optimization option.
(In previous version of call_in_stack, compiling with no optimization option in x64 of old GCC version will fail to complete compile.)


---------------------------------------------------------------------------------------------------------------

Version 1.0.5: Important adaptive improvement for higher version of GCC!

In this version:

We remove __builtin_unreachable in our library because GCC has different semantic in high version(since 4.5);

We disable the -fipa-sra optimize for "do_call" function (with no performance lost because it is an assemble function indeed) in our library because it breaks the system V ABI and arguments forwarding rule.

---------------------------------------------------------------------------------------------------------------

Version 1.0.2: Now we support call_in_stack for member function.

```c++
std::string a("Hello world");
call_in_stack(printf, "%s\n", call_in_stack(from_member_fun(a, c_str))) ;
```

And we support call_in_stack for functor or lambda in C++11!

```c++
std::string a("Hello world");
call_in_stack_safe(buf, from_functor(
	[=](int arg){
		printf("%s\n", a.c_str());
		return arg;
	}
),2) ;
```

That means you can write most of your codes of function in a new stack!
