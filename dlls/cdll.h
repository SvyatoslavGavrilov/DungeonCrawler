//#include "crusher.c"

#ifdef C_EXPORTS
  #define CAPI __declspec(dllexport)
#else
  #define CAPI __declspec(dllimport)
#endif

#define CCALL __cdecl

#ifdef __cplusplus
extern "C"
{
#endif

CAPI void CCALL crush_pc(int a);

#ifdef __cplusplus
} // __cplusplus defined.
#endif
