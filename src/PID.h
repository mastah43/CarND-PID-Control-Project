#ifndef PID_H
#define PID_H

#include <vector>

class PID {
public:
    /*
    * Errors
    */
    double p_error;
    double i_error;
    double d_error;

    /*
    * Coefficients
    */
    double Kp;
    double Ki;
    double Kd;

    int step = 0;
    int twiddles = 0;
    double twiddle_error;
    double best_error;
    int twiddle_param_index = 0;
    std::vector<double> twiddle_dp;
    std::vector<double> twiddle_p;
    bool tried_subtracting;

    /*
    * Constructor
    */
    PID();

    /*
    * Destructor.
    */
    virtual ~PID();

    /*
    * Initialize PID.
    */
    void Init(double Kp, double Ki, double Kd);

    /*
    * Update the PID error variables given cross track error.
    */
    void UpdateError(double cte);

    /*
    * Calculate the total PID error.
    */
    double TotalError();

};

#endif /* PID_H */