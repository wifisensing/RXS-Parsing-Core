
#ifndef _mw_coder_bounded_array_h
#define _mw_coder_bounded_array_h

namespace coder {

#ifndef CODER_ARRAY_SIZE_TYPE_DEFINED
#if __cplusplus >= 201103L
using SizeType = int;
#else
typedef int SizeType;
#endif
#endif

// Bounded array
template <typename T, SizeType UpperBoundSize, SizeType NumDims>
struct bounded_array {
    T data[UpperBoundSize];
    SizeType size[NumDims];
};

template <typename T, SizeType NumDims>
struct empty_bounded_array {
    SizeType size[NumDims];
};
} // namespace coder

#endif
