#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

class Interval {
public:
    Interval() : min(0), max(0) {}
    Interval(double min, double max) : min(min), max(max) {}

    bool contains(double x) const {
        return x >= min && x <= max;
    }

    bool surrounds(double x) const {
        return x > min && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    static const Interval Empty, Universe;

public:
    double min, max;
};

const static double _infinity = std::numeric_limits<double>::infinity();

const static Interval Empty (0.0, 0.0);
const static Interval Universe (-_infinity, +_infinity);

#endif // INTERVAL_H
