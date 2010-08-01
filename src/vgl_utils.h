#ifndef vgl_utils_h
#define vgl_utils_h

namespace vgl {

//
// FUNCTIONS
//

template <typename Num>
Num clamp(Num k)
{
  return std::max(Num(0), std::min(Num(1), k));
}


template <typename Num>
Num sqr(Num k)
{
  return k * k;
}

} // namespace vgl

#endif // vgl_utils_h

