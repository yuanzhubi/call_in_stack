Welcome to call in stack, goto https://github.com/yuanzhubi/call_in_stack/wiki/Welcome-to-call_in_stack for details.



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
