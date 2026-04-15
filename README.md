C++ Recursive Descent JSON Parser



A lightweight, dependency-free JSON parser written from scratch in modern C++. 



Overview

This project converts raw JSON strings into an Abstract Syntax Tree (AST) using a custom lexical analyzer and a recursive descent parsing algorithm. It maps JSON data types directly to native C++ types.



Technical Details

\-> Lexical Analysis: Custom tokenizer that processes strings, numbers, booleans, arrays, and nested objects.

\-> Modern C++ Features: Utilizes 'std::variant' for type-safe polymorphic data storage and 'std::shared\_ptr' to manage recursive memory structures without leaks.

\-> Error Handling: Implements robust syntax validation and throws descriptive runtime errors for malformed JSON inputs.

