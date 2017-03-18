Welcome to call in stack, goto https://github.com/yuanzhubi/call_in_stack/wiki/Welcome-to-call_in_stack for details.



Version 1.0.2: use a "type union" method to reduce the count of leaf template function instance via reducing types, including:

1.union all the pointer and reference type to word_int_t;

2.union unsigned and not unsigned integer type to not unsigned.
