# BeBOT
Bernstein/Beziér Optimal Trajectories (BeBOT) toolkit for generating optimal trajectories for single or multiple autonomous vehicles.

A comprehensive review of the techniques employed by the toolkit can be found in [BeBOT: Bernstein Polynomial Toolkit for Trajectory Generation](https://doi.org/10.1109/IROS40897.2019.8967564).

# Running Examples
An Anaconda environment YAML file is provided to help make the process of running the examples smooth. On Linux you should be able to run `conda env create -f bebotenv.yml`. On Windows, you will need to use the Anaconda Navigator to import the environment file.

# General Explanation
The goal of this project is to put forward an algorithm to solve motion planning problems, enabling autonomous operations for cooperative vehicles navigating in complex environment in the presence of humans. The approach is to formulate the trajectory generation problem as an optimal control problem (OCP) and then use Bernstein polynomials to transcribe it into a nonlinear programming (NLP) problem which can be solved using off-the-shelf solvers. So, what does all of that mean?

One way to solve robot navigation problems is to generate a path from your current position to your desired position and then use a lower level controller to follow said path. Think of generating the path as using your phone to calculate a route and you following that route in your vehicle as the lower level controller. For this project we will be generating trajectories, which are slightly different. Instead of just having positions, trajectories can also include additional information such as speed and acceleration at a given time. This is useful because an algorithm computing a trajectory can take important factors such as the maximum speed of the vehicle into account.

There are many approaches to designing an algorithm that computes trajectories and they each have their own benefits and drawbacks. Formulating the trajectory generation problem as an OCP allows us to tweak all the inputs to our autonomous vehicle to find the best set of inputs to achieve a desired goal. However, we do need to have a good mathematical model of our system and a decent initial guess that is close to the most optimal solution. This formulation also makes it relatively easy to include additional constraints to our problem such as obstacle avoidance and a maximum turning rate. Unfortunately, despite our significant progress in mathematics, it is often impossible to solve an OCP by hand. This is where problem transcription comes in.

Nonlinear programming problems are a specific formulation where the goal is to minimize some function subject to equality and inequality constraints (be aware that linear programming problems also exist, but for this project we will use nonlinear programming). Suppose you work for a delivery company and you want to make as many deliveries as possible by the end of the day without running out of gas or getting pulled over for speeding. In this case, you would like to maximize the number of deliveries. To make that fit the NLP problem formulation, we can simply say that we want to minimize the negative of the number of deliveries (e.g., -10 deliveries would be preferred to -5 deliveries). Let’s say that no matter what, you want to finish your deliveries at exactly 5PM. That end time would be considered an equality constraint. Your inequality constraints are the amount of gas you have and the maximum speed. You don’t want to run out of gas, but you also don’t necessarily need to use up all the gas in your tank. Similarly, for speed, you can drive lower than the maximum speed, but you don’t want to go over it and risk a ticket. The nice thing about NLP problems is that they have been studied by many bright minds for years which means methods to solve them already exist. So, if you can represent your problem as an NLP problem, then you can easily solve it using one of the existing methods.

People have been converting problems into different forms for millennia. You could use geometry to find an unknown value on a shape or you could represent the shape using algebra and solve for the unknown value and reach the same conclusion. To convert the trajectory generation OCP into an NLP problem, you could simply represent the trajectory as a series of many points. The NLP solver would then need to tweak each one of those points in order to minimize the given function while adhering to the constraints. Increasing the number of points quickly makes the problem impossible for even the most powerful computers to solve but you might need many points to ensure safety and feasibility. A clever way to reduce the number of points is to instead represent the path as a polynomial. We could potentially create the same path represented by thousands of points by simply changing the coefficients A, B, and C in the equation f(x) = A*x^2 + B*x + C. There are different polynomial bases such as Legendre, Hermite, or in the last example, monomial. This work will use the Bernstein basis.

Bernstein polynomials were originally created by Sergei Natanovich Bernstein in 1912 but their modern form and popularity did not rise until the 1960’s when two French engineers, Paul de Faget de Casteljau and Pierre Étienne Bézier, employed at competing auto manufacturers used them for representing auto body shapes. Bernstein polynomials, frequently called Bézier curves, provide an intuitive way to build and modify a curved line. If you have ever used the spline tool in a vector graphics program like Adobe Illustrator, you likely used Bernstein polynomials. These polynomials provide many useful properties such as needing few points to represent a useful trajectory and making important mathematical computations very efficient. These properties make them an excellent choice for converting the OCP into an NLP problem since they reduce the computation time needed while still guaranteeing safety in continuous time, which is often not possible using other methods.

# How to acknowledge this work
We hope that you find this software useful and we would like to hear your feedback. We have put a great deal of time and effort into BeBOT's development. Please, cite the following publication if you are using BeBOT for your own research:

Calvin Kielas-Jensen, Venanzio Cichella, Thomas Berry, Isaac Kaminer, Claire Walton, and Antonio Pascoal. "Bernstein Polynomial-Based Method for Solving Optimal Trajectory Generation Problems." Sensors 22, no. 5 (2022): 1869.

BibTex entry:

@article{kielas2022bernstein,  
  title={Bernstein Polynomial-Based Method for Solving Optimal Trajectory Generation Problems},  
  author={Kielas-Jensen, Calvin and Cichella, Venanzio and Berry, Thomas and Kaminer, Isaac and Walton, Claire and Pascoal, Antonio},  
  journal={Sensors},  
  volume={22},  
  number={5},  
  pages={1869},  
  year={2022},  
  publisher={Multidisciplinary Digital Publishing Institute}  
}  
