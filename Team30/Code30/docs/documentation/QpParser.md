## Intro
Started with a simple parser without a proper design. It got the job done, but its messy, difficult to extend, difficult to read & understand and difficult to bugfix. Attempted to source online for designs used by other people in building a parser.

## Designs
- interpreter pattern
    - recommended pattern for parsers
- visitor
    - each expression is visited and the interpret() method is called
    - benefits:
        - expressions are only decided at runtime. this method allows runtime decision of which interpret method to call
        - The overall design is easily extensible. when adding new expressions, original expressions are not affected. the interpreter only needs a new method, which is the new expression's interpret method.
        - if logic changes, 
        - ParsedQueyrHandler does not need to know exactly which expression's interpret method to call.
- chain of responsibility
- singleton for some classes
    e.g. QueryIterpreter
