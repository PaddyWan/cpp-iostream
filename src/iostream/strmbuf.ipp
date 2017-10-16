#ifndef STRM_IPP
#define STRM_IPP

template<class T>
concept bool Istrm = requires(T x)
{
//    {x.strm.avail_in} ->
//    {x.strm.next_in} ->
//    {x.strm.avail_out} ->
//    {x.strm.next_out} ->
    {
        x.init()
    };
    {
        x.end()
    };
    {
        x.inflate()
    };
};

template<class T>
concept bool Ostrm = requires(T x, int level, int flush)
{
//    {x.strm.avail_in} ->
//    {x.strm.next_in} ->
//    {x.strm.avail_out} ->
//    {x.strm.next_out} ->
//    {x::generic_t}
//    {x.init(level)};
    {
        x.end()
    };
//    {x.no_flush()} -> int;
//    {x.finish()} -> int;
//    {x.deflate(flush)};
};

#endif//STRM_IPP
