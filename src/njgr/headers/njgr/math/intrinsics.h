#if defined(_MSC_VEC)
	#define INTRINSICS_SUPPORT
	#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
	#define INTRINSICS_SUPPORT
	#include <x86intrin.h>
#elif defined(__GNUC__) && defined(__ARM_NEON__)
	#define INTRINSICS_SUPPORT
	#include <arm_neon.h>
#elif defined(__GNUC__) && defined(__IWMMXT__)
	#define INTRINSICS_SUPPORT
	#include <mmintrin.h>
#elif (defined(__GNUC__) || defined(__xlC__)) && (defined(__VEC__) || defined(__ALTIVEC__))
	#define INTRINSICS_SUPPORT
	#include <altivec.h>
#elif defined(__GNUC__) && defined(__SPE__)
	#define INTRINSICS_SUPPORT
	#include <spe.h>
#endif

//#undef INTRINSICS_SUPPORT
