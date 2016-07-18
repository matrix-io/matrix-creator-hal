#include <valarray>

class FIR {
 public:
  void Setup(std::valarray<float>& coeff) {
    coeff_ = coeff;
    x_.resize(coeff_.size());
  }

  float Filter(float xo) {
    float acc = 0;
    x_ = x_.shift(-1);
    x_[0] = xo;
    for (unsigned int i = 0; i < x_.size(); i++) {
      acc += coeff_[i] * x_[i];
    }
    return acc;
  }

 private:
  std::valarray<float> x_;
  std::valarray<float> coeff_;
};
