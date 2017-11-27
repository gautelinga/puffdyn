# PuffDyn
<p align="center">
 <img src="https://raw.githubusercontent.com/gautelinga/puffdyn/master/images/spacetime.png" width="100%" />
</p>
Discrete-continuous model of puff dynamics implemented in C++. Visualisation using Python/Matplotlib.

## Model
This program simulates a queue (Queue) of puffs (Node) that move on a periodic line, i.e. a ring.
Each puff moves upstream with a velocity <img src="http://chart.apis.google.com/chart?cht=tx&chl=v"/> given by the distance <img src="http://chart.apis.google.com/chart?cht=tx&chl=\ell"/> to the next puff upstream.
If the distance upstream is large (<img src="http://chart.apis.google.com/chart?cht=tx&chl=\ell\to\infty"/>), it moves with a velocity <img src="http://chart.apis.google.com/chart?cht=tx&chl=v=1"/>; if the distance is small (<img src="http://chart.apis.google.com/chart?cht=tx&chl=\ell\to0"/>), it is slowed down to <img src="http://chart.apis.google.com/chart?cht=tx&chl=v=0"/>.
In between it is smoothly interpolated.
Additionally, we have a probability of splitting and decay.
When the distance to the puff ahead is short, the splitting probability <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_s"/> is reduced and the decay rate <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_d"/> is enhanced.

To simplify a bit, we consider that the puffs have a characteristic upstream interaction distance <img src="http://chart.apis.google.com/chart?cht=tx&chl=\lambda"> which is the same for velocity, splitting and decay.
We assume that for <img src="http://chart.apis.google.com/chart?cht=tx&chl=\ell=0"/>, the splitting rate is multiplied (amplified) by a factor <img src="http://chart.apis.google.com/chart?cht=tx&chl=A>1"/> and the decay rate is divided by a the same factor.
With an exponential dependence, we can write:
<p align="center">
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=v(\ell)=1-e^{-\ell/\lambda}"/>,<br />
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_d(\ell)=\omega_d^0\left[A%2B(1-A)v(\ell)\right]"/>,<br />
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_s(\ell)=\omega_s^0\left[\frac{1}{A}%2B\left(1-\frac{1}{A}\right)v(\ell)\right]"/>.
</p>

We start with a configuration of <img src="http://chart.apis.google.com/chart?cht=tx&chl=N"/> puffs <img src="http://chart.apis.google.com/chart?cht=tx&chl=i\in[0,N)"/> with initial positions <img src="http://chart.apis.google.com/chart?cht=tx&chl=x_i\in[0,L)"/> ordered such that <img src="http://chart.apis.google.com/chart?cht=tx&chl=x_{i%2B1}\geq{}x_i\forall{}i"/> (subject to the periodic boundary condition).
The initial puffs are either equidistantly spaced according to the mean field ("mf") solution, or in a random order ("random").
Then the model consists of solving the ODEs:
<p align="center">
  <img src="http://chart.apis.google.com/chart?cht=tx&chl=\frac{\mathrm{d}x_i}{\mathrm{d}t}=v(x_{i%2B1}-x_i)%2B\sqrt{D}\xi_i(t)"/>,
</p>

in addition to having splitting and decay (removal) of puffs (i.e. nodes) in accordance with the above rates.
Here, <img src="http://chart.apis.google.com/chart?cht=tx&chl=\xi_i(t)"/> is a Gaussian noise such that <img src="http://chart.apis.google.com/chart?cht=tx&chl=\langle\xi_i(t)\xi_j(t')\rangle=\delta_{ij}\delta(t-t')"/>, and <img src="http://chart.apis.google.com/chart?cht=tx&chl=D"/> is a diffusion coefficient.
These are the equations of motion.

## Implementation
The equations of motion are solved with a (limited) explicit Euler method, or Ito formalism:
<p align="center">
  <img src="http://chart.apis.google.com/chart?cht=tx&chl={\Delta{}x_i^*}={\Delta{}t}v(x_{i%2B1}^k-x_i^k)%2B\sqrt{D\Delta{}t}N(0,1)"/>.
</p>

The spatial update <img src="http://chart.apis.google.com/chart?cht=tx&chl=\Delta{}x^{k%2B1}"/> is given by <img src="http://chart.apis.google.com/chart?cht=tx&chl=\Delta{}x_i^{k%2B1}=\max(0,\min(\Delta{}x_i^*,x_{i%2B1}^k-x_i^k))"/> (again subject to the periodic boundary condition).
Then the update is <img src="http://chart.apis.google.com/chart?cht=tx&chl=x_{i%2B1}^{k%2B1}=x_i^{k%2B1}%2B\Delta{}x_i^{k%2B1}"/>.
This is efficiently implemented as a double-linked list.

## Program
The program - only tested in Linux - is compiled by writing
``make``
in the root directory.
Then it can be run with default parameters by executing
`` ./puff ``.
Alternative parameters can be specified using the command line.
The above command is equivalent to writing:
```
./puff L=40.0 N=10 d=0.1 wd=0.1 ws=0.16 A=5.0 T=1000.0 dt=0.001 D=0.0 dump_pos=True results_folder=results/One/ verbose=False stat_intv=100 dump_intv=1000 init_mode=mf
```
These arguments are:
* ``L``: Length of periodic domain.
* ``N``: Number of initial puffs (not in use if ``ìnit_mode==mf``).
* ``d``: Interaction distance <img src="http://chart.apis.google.com/chart?cht=tx&chl=\lambda"/>.
* ``wd``: Long-distance decay rate <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_d^0"/>.
* ``ws``: Long-distance splitting rate <img src="http://chart.apis.google.com/chart?cht=tx&chl=\omega_s^0"/>
* ``A``: Amplifaction factor <img src="http://chart.apis.google.com/chart?cht=tx&chl=A"/>.
* ``T``: Total simulation time.
* ``dt``: Simulation time step <img src="http://chart.apis.google.com/chart?cht=tx&chl=\Delta{}t"/>.
* ``D``: Diffusion coefficient.
* ``dump_pos``: Dump the spacetime diagram of puffs to files for plotting.
* ``results_folder``: Folder in which to dump the results. The program erases all content in this folder upon initialization.
* ``verbose``: More information on runtime.
* ``stat_intv``: How many timesteps between each time to probe statistics.
* ``dump_intv``: How many probings between each dump to file.
* ``init_mode``: Initialization mode. Equals ``mf`` (mean field, default) or ``random``.

## Plotting
To plot, point to the ``results_folder`` (here as example, ``results/One/``) and run:
```
python plot.py results/One/
```
This will plot a lot of stuff!

## Universality class
Run and see! A Python script for parameter sweeping is underway.
