#include <ugly.h>
#include <bad.h>

#include <vector>
#include <memory>

struct SplineImpl {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> answer;
    SplineImpl(const std::vector<double>& x1, const std::vector<double>& y1, double a, double b)
            : x(x1), y(y1) {
        answer.resize(x1.size());
        mySplineSnd(x.data(), y.data(), x.size(), a, b, answer.data());
    }

    double Interpolate(double xx) {
        double yy;
        mySplintCube(x.data(), y.data(), answer.data(), x.size(), xx, &yy);
        return yy;
    }
};

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b) {
    impl_ = std::make_shared<SplineImpl>(SplineImpl(x, y, a, b));
}

double Spline::Interpolate(double x) {
    return impl_->Interpolate(x);
}