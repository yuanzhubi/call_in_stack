Welcome to call in stack!

Goto https://github.com/yuanzhubi/call_in_stack/wiki/Welcome-to-call_in_stack for details.

Goto QQ Group :293767138(talk_in_stack) for further discussion.

---------------------------------------------------------------------------------------------------------------

Version 1.0.6: Important adaptive improvement for different version of GCC!

In this version:

We remove __builtin_unreachable in our library because GCC has different semantic in high version(since 4.5);

We disable the -fipa-sra optimize for "do_call" function (with no performance lost because it is an assemble function indeed) in our library because it breaks the system V ABI and arguments forwarding rule.

---------------------------------------------------------------------------------------------------------------

Version 1.0.2: Now we support call_in_stack for member function.

std::string a("Hello world");
call_in_stack(printf, "%s\n", call_in_stack(from_member_fun(a, c_str))) ;

And we support call_in_stack for functor or lambda in C++11!

std::string a("Hello world");
call_in_stack_safe(buf, from_functor(
	[=](int arg){
		printf("%s\n", a.c_str());
		return arg;
	}
),2) ;

That means you can write most of your codes of function in a new stack!
