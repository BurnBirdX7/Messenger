#ifndef ASIOAPPLICATION_CONTEXT_HPP
#define ASIOAPPLICATION_CONTEXT_HPP


class Context {
public:

    Context();
    Context(Context&&);

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;

};


#endif //ASIOAPPLICATION_CONTEXT_HPP
