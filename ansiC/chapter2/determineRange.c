#include <stdio.h>
#include <limits.h>
#include <float.h>

int main(void)
{
    printf("=== <limits.h> ===\n");
    printf("CHAR_BIT     : %d\n", CHAR_BIT);

    printf("SCHAR_MIN    : %d\n", SCHAR_MIN);
    printf("SCHAR_MAX    : %d\n", SCHAR_MAX);
    printf("UCHAR_MAX    : %u\n", (unsigned)UCHAR_MAX);

    printf("CHAR_MIN     : %d\n", CHAR_MIN);
    printf("CHAR_MAX     : %d\n", CHAR_MAX);

    printf("MB_LEN_MAX   : %d\n", MB_LEN_MAX);

    printf("SHRT_MIN     : %d\n", SHRT_MIN);
    printf("SHRT_MAX     : %d\n", SHRT_MAX);
    printf("USHRT_MAX    : %u\n", (unsigned)USHRT_MAX);

    printf("INT_MIN      : %d\n", INT_MIN);
    printf("INT_MAX      : %d\n", INT_MAX);
    printf("UINT_MAX     : %u\n", (unsigned)UINT_MAX);

    printf("LONG_MIN     : %ld\n", LONG_MIN);
    printf("LONG_MAX     : %ld\n", LONG_MAX);
    printf("ULONG_MAX    : %lu\n", (unsigned long)ULONG_MAX);

#ifdef LLONG_MIN
    printf("LLONG_MIN    : %lld\n", (long long)LLONG_MIN);
    printf("LLONG_MAX    : %lld\n", (long long)LLONG_MAX);
    printf("ULLONG_MAX   : %llu\n", (unsigned long long)ULLONG_MAX);
#endif

    printf("\n=== <float.h> ===\n");

    printf("FLT_RADIX    : %d\n", FLT_RADIX);

    printf("FLT_MANT_DIG : %d\n", FLT_MANT_DIG);
    printf("DBL_MANT_DIG : %d\n", DBL_MANT_DIG);
    printf("LDBL_MANT_DIG: %d\n", LDBL_MANT_DIG);

    printf("FLT_DIG      : %d\n", FLT_DIG);
    printf("DBL_DIG      : %d\n", DBL_DIG);
    printf("LDBL_DIG     : %d\n", LDBL_DIG);

    printf("FLT_MIN_EXP  : %d\n", FLT_MIN_EXP);
    printf("DBL_MIN_EXP  : %d\n", DBL_MIN_EXP);
    printf("LDBL_MIN_EXP : %d\n", LDBL_MIN_EXP);

    printf("FLT_MIN_10_EXP : %d\n", FLT_MIN_10_EXP);
    printf("DBL_MIN_10_EXP : %d\n", DBL_MIN_10_EXP);
    printf("LDBL_MIN_10_EXP: %d\n", LDBL_MIN_10_EXP);

    printf("FLT_MAX_EXP  : %d\n", FLT_MAX_EXP);
    printf("DBL_MAX_EXP  : %d\n", DBL_MAX_EXP);
    printf("LDBL_MAX_EXP : %d\n", LDBL_MAX_EXP);

    printf("FLT_MAX_10_EXP : %d\n", FLT_MAX_10_EXP);
    printf("DBL_MAX_10_EXP : %d\n", DBL_MAX_10_EXP);
    printf("LDBL_MAX_10_EXP: %d\n", LDBL_MAX_10_EXP);

    printf("FLT_MAX      : %e\n", (double)FLT_MAX);
    printf("DBL_MAX      : %e\n", DBL_MAX);
    printf("LDBL_MAX     : %Le\n", (long double)LDBL_MAX);

    printf("FLT_EPSILON  : %e\n", (double)FLT_EPSILON);
    printf("DBL_EPSILON  : %e\n", DBL_EPSILON);
    printf("LDBL_EPSILON : %Le\n", (long double)LDBL_EPSILON);

    printf("FLT_MIN      : %e\n", (double)FLT_MIN);
    printf("DBL_MIN      : %e\n", DBL_MIN);
    printf("LDBL_MIN     : %Le\n", (long double)LDBL_MIN);

#ifdef FLT_TRUE_MIN
    printf("FLT_TRUE_MIN : %e\n", (double)FLT_TRUE_MIN);
    printf("DBL_TRUE_MIN : %e\n", DBL_TRUE_MIN);
    printf("LDBL_TRUE_MIN: %Le\n", (long double)LDBL_TRUE_MIN);
#endif

#ifdef DECIMAL_DIG
    printf("DECIMAL_DIG  : %d\n", DECIMAL_DIG);
#endif

    return 0;
}
