#include "PID.h"

#include <iostream>
#include <math.h>
#include <limits>

#define TWIDDLE false
#define STEPS_INIT 100
#define STEPS_EVAL 2000
#define STEPS_TWIDDLE (STEPS_INIT+STEPS_EVAL)

PID::PID() {
    best_error = std::numeric_limits<double>::max();
    tried_subtracting = false;
}

PID::~PID() = default;

void PID::Init(double Kp, double Ki, double Kd) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;
    p_error = d_error = i_error = 0.;

    twiddle_dp = {0.1 * Kp, 0.1 * Kd, 0.1 * Ki};
    twiddle_p = {Kp, Kd, Ki};
}

void PID::UpdateError(double cte) {
    d_error = cte - p_error;
    p_error = cte;
    i_error += cte;

#if TWIDDLE == true
    ++step;

    if (step > STEPS_INIT) {
        // after some initial steps to transition from effects of last PID coefficients
        // to new PID coefficients we can add the error for every step
        //std::cout << "step=" << step << "; error sum: " << twiddle_error << "; best error=" << best_error << std::endl;

        // TODO add tolerance of cte 3 since we primarily want to avoid that the vehicle is off track
        twiddle_error += pow(cte, 2); // use quadratic to penalize high cte
    }

    if (step > STEPS_TWIDDLE) {

        // after several race trace loops we finalize the evaluation of the chosen PID coefficients
        if (twiddle_error < best_error) {
            std::cout << "twiddle " << twiddles << " improvement: P=" << this->Kp << "; I=" << this->Ki << "; D=" << this->Kd
                      << "; error=" << twiddle_error << "; best_error=" << best_error << std::endl;

            best_error = twiddle_error;

            // increase step for this coefficient
            twiddle_dp[twiddle_param_index] *= 1.1;

            // proceed with next parameter and try positive move
            twiddle_param_index = (twiddle_param_index + 1) % 2; // do not twiddle coefficient I since we use 0
            twiddle_p[twiddle_param_index] += twiddle_dp[twiddle_param_index];
            tried_subtracting = false;
        }
        else {
            std::cout << "twiddle " << twiddles << " no improvement: P=" << this->Kp << "; I=" << this->Ki << "; D=" << this->Kd
                      << "; error=" << twiddle_error << "; best_error=" << best_error << std::endl;

            if (!tried_subtracting) {
                // stay at this paramter and effectively try negative move
                twiddle_p[twiddle_param_index] -= 2 * twiddle_dp[twiddle_param_index];
                tried_subtracting = true;
            } else {
                // revert coefficient to last best value, reduce step
                twiddle_p[twiddle_param_index] += twiddle_dp[twiddle_param_index];
                twiddle_dp[twiddle_param_index] *= 0.9;

                // proceed with next parameter and try positive move
                twiddle_param_index = (twiddle_param_index + 1) % 2; // do not twiddle param I since we use I=0
                twiddle_p[twiddle_param_index] += twiddle_dp[twiddle_param_index];
                tried_subtracting = false;
            }
        }

        step = 0;
        ++twiddles;
        twiddle_error = 0.;

        // init to new PID coefficients
        Kp = twiddle_p[0];
        Kd = twiddle_p[1];
        Ki = twiddle_p[2];
        // p_error = d_error = i_error = 0.; // TODO let to vehicle left track

        //std::cout << "twiddle: new coefficients: P=" << Kp << "; I=" << Ki << "; D=" << Kd << std::endl;
    }

#endif
}

double PID::TotalError() {
    return -Kp * p_error - Kd * d_error - Ki * i_error;
}

