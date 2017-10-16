#ifndef STRING_IPP
#define STRING_IPP

template<class T>
concept bool String = requires(T x, size_t i)
{
    {
        x[i]
    }
    -> const char&;
}
&&
(std::is_const<typename std::remove_reference<T>::type>::value || requires(T x, size_t i)
{
    {
        x[i]
    }
    -> char&;
});


#endif//STRING_IPP
