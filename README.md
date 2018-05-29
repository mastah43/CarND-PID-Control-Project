# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

My solution for the PID controller project is a simple application of a PD controller
for the steering angle using a fixed throttle (of 0.3). The PD coefficients were 
manually tuned.

## Discussion of P, I and D effects

## P effect
The P coefficient is a factor on how strong the current track error affects the steering.
This parameter has the strongest direct effect on steering. A high value helps to react
fast in curves but also requires counter measurements via D parameter in order to prevent
massive overshooting.

## I effect
The I coefficient is a factor on how strong the incremental error over the entire drive
affects the steering. This could help to correct systemic deviations in steering 
e.g. a vehicle that slightly drives to th left although steering angle 0 is used.
I realized that the integral error (I error) always increases over time which is due 
to the fact that the vehicle drives on a (left oriented) circuit. Since there are more
left curves and the vehicle is always off to the right in those curves,
the CTE is always positive in average. Thus I chose to set I coefficient to exactly 0
otherwise I would have a changing steering behaviour the longer the vehicle drives
around the track.

## D effect
The D coefficient is a factor on how strong the delta error between last time stamp
and current time stamp affects the steering. This can help to prevent overshooting 
of the steering.

## Choosing final P, I and D coefficients and their effect.

I chose to use a constent throttle of 0.3 and based my PID parameter tuning on that.
I first manually tried out different combinations of P and D combinations based 
on my growing understanding of the P, I and D error as well as the coefficient effects 
(see below). 
Then I used automatic twiddling to find more fitting parameters while using
the manually found coefficients as start state.

### Manual selection of coefficients 

#### P coefficient
I initially chose P to be 0.1 since I realized that a magnitue CTE of 5 means the vehicle is just off 
track. 0.1*5=0.5 which i considered the maximum (safe) steering value magnitude I wanted to 
have. I manually tuned this to 1.3.

#### D coefficient
I chose 0.5 to prevent overshooting and oscillation by manual tuning and experiments.
A high value (3 and 5) lead to emphasized overshooting. 

#### I coefficient
Set to 0 (see discussion of I above).

### Automatic twiddling
I used automatic twiddling to find the final coefficients:
* P: 0.166938
* I: 0
* D: 0.943473  
With this the vehicle does not pop up on ledges even in sharp curves. 
It has a fast reaction to cross track error and also some oscillation.
[Here](twiddle_results.txt) is the log auto twiddle steps and results.

## Potential future work
The solution could be extended by using a PID controller for the throttle in order
to dramatically slow down in sharp curves allowing for slight turns in those curves
while having a high velocity on the straight or very slightly curved sections.
This could help to reduce the lateral oscillation while also improving the performance
in curves by beeing more in the center of the lane. But when using automatic 
fiddling of PID for throttle and steering, a slow down should be punished 
in addition to cross track error in order to prevent that coefficients are chosen
that lead to slow driving. 

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

## Editor Settings

We've purposefully kept editor configuration files out of this repo in order to
keep it as simple and environment agnostic as possible. However, we recommend
using the following settings:

* indent using spaces
* set tab width to 2 spaces (keeps the matrices in source code aligned)

## Code Style

Please (do your best to) stick to [Google's C++ style guide](https://google.github.io/styleguide/cppguide.html).

## Project Instructions and Rubric

Note: regardless of the changes you make, your project must be buildable using
cmake and make!

More information is only accessible by people who are already enrolled in Term 2
of CarND. If you are enrolled, see [the project page](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/f1820894-8322-4bb3-81aa-b26b3c6dcbaf/lessons/e8235395-22dd-4b87-88e0-d108c5e5bbf4/concepts/6a4d8d42-6a04-4aa6-b284-1697c0fd6562)
for instructions and the project rubric.

## Hints!

* You don't have to follow this directory structure, but if you do, your work
  will span all of the .cpp files here. Keep an eye out for TODOs.

## Call for IDE Profiles Pull Requests

Help your fellow students!

We decided to create Makefiles with cmake to keep this project as platform
agnostic as possible. Similarly, we omitted IDE profiles in order to we ensure
that students don't feel pressured to use one IDE or another.

However! I'd love to help people get up and running with their IDEs of choice.
If you've created a profile for an IDE that you think other students would
appreciate, we'd love to have you add the requisite profile files and
instructions to ide_profiles/. For example if you wanted to add a VS Code
profile, you'd add:

* /ide_profiles/vscode/.vscode
* /ide_profiles/vscode/README.md

The README should explain what the profile does, how to take advantage of it,
and how to install it.

Frankly, I've never been involved in a project with multiple IDE profiles
before. I believe the best way to handle this would be to keep them out of the
repo root to avoid clutter. My expectation is that most profiles will include
instructions to copy files to a new location to get picked up by the IDE, but
that's just a guess.

One last note here: regardless of the IDE used, every submitted project must
still be compilable with cmake and make./

## How to write a README
A well written README file can enhance your project and portfolio.  Develop your abilities to create professional README files by completing [this free course](https://www.udacity.com/course/writing-readmes--ud777).

