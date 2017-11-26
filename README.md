# PuffDyn
Discrete-continuous model of puff dynamics implemented in C++. Visualisation using Python/Matplotlib.

## Model
This program simulates a queue (Queue) of puffs (Node) that move on a periodic line, i.e. a ring.
Each puff moves upstream with a velocity <img src="http://chart.apis.google.com/chart?cht=tx&chl=v"/> given by the distance <img src="http://chart.apis.google.com/chart?cht=tx&chl=\ell"/> to the next puff upstream.
If the distance upstream is large (<img src="http://chart.apis.google.com/chart?cht=tx&chl=v\to\infty"/>), it moves with a velocity <img src="http://chart.apis.google.com/chart?cht=tx&chl=v=1"/>; if the distance is small (<img src="http://chart.apis.google.com/chart?cht=tx&chl=v\to0"/>), it is slowed down to <img src="http://chart.apis.google.com/chart?cht=tx&chl=v=0"/>.
In between it is smoothly interpolated.
Additionally, we have a probability of splitting and decay.
When the distance to the puff ahead is short, the splitting probability <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_s"/> is reduced and the decay rate <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_d"/> is enhanced.

To simplify a bit, we consider that the puffs have a characteristic upstream interaction distance <img src="http://chart.apis.google.com/chart?cht=tx&chl=\lambda"> which is the same for velocity, splitting and decay.
We assume that for <img src="http://chart.apis.google.com/chart?cht=tx&chl=\ell=0"/>, the splitting rate is multiplied (amplified) by a factor <img src="http://chart.apis.google.com/chart?cht=tx&chl=A>1"/> and the decay rate is divided by a the same factor.
With an exponential dependence, we can write:
<p align="center">
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=v(\ell)=1-e^{-\ell/\delta}"/>,<br />
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_d(\ell)=\omega_d^0\left[F%2B(1-F)v(l)\right]"/>,<br />
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_s(\ell)=\omega_s^0\left[\frac{1}{F}%2B\left(1-\frac{1}{F}\right)v(l)\right]"/>.
</p>

We start with a configuration of <img src="http://chart.apis.google.com/chart?cht=tx&chl=N"/> puffs <img src="http://chart.apis.google.com/chart?cht=tx&chl=i\in[0,N)"/> with initial positions <img src="http://chart.apis.google.com/chart?cht=tx&chl=x_i\in[0,L)"/> ordered such that <img src="http://chart.apis.google.com/chart?cht=tx&chl=x_{i%2B1}\geq{}x_i\forall{}i"/> (subject to the periodic boundary condition).
The initial puffs are either equidistantly spaced according to the mean field ("mf") solution, or in a random order ("random").
Then the model consists of solving the ODEs:
<p align="center">
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=\frac{\mathrm{d}x_i}{\mathrm{d}t}=v(x_{i%2B1}-x_i)"/>,
</p>
in addition to having splitting and decay (removal) of puffs (i.e. nodes) in accordance with the above rates.
These are the equations of motion.

## Implementation
The equations of motion are solved with a (limited) explicit Euler method:
<p align="center">
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=\frac{\Delta{}x_i^k}{\Delta{}t}=v(x_{i%2B1}^k-x_i^k)"/>.
</p>
