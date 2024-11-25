# UTF8 String

Simple C utf8 string library

https://github.com/imprity/UTF8String

## How to use the library?

Very Simple *(and probably unsafe)* c string libray.

Just include it in your code and build with it.

- Exmaple
```
UTF8String* str = utf8string_from_cstr(u8"Hello World");
UTF8StringView sv = utf8string_sv_from_str(str);

UTF8StringView hello = utf8string_sv_sub_sv(sv, 0, 5);
UTF8StringView world = utf8string_sv_sub_sv(sv, 6, sv.count);

//Do some kewl things with it
```
