#include "math.hpp"

namespace math
{

    int cos(int i) { return cosf(i); }
    long cos(long l) { return cosf(l); }
    float cos(float f) { return cos(f); }
    double cos(double d) { return cos(d); }

    int sin(int i) { return sinf(i); }
    long sin(long l) { return cosf(l); }
    float sin(float f) { return cosf(f); }
    double sin(double d) { return cos(d); }

    int tan(int i) { return tanf(i); }
    long tan(long l) { return tanf(l); }
    float tan(float f) { return tanf(f); }
    double tan(double d) { return tanf(d); }

}
