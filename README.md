# PuffDyn
Discrete-continuous model of puff dynamics.

This program simulates a queue (Queue) of puffs (Node) that move on a periodic line, i.e. a ring. Each puff moves upstream with a velocity given by the distance to the next puff upstream. If the distance upstream is large, it moves with a velocity 1; if the distance is small, it is slowed down to 0.

To simplify a bit, we consider that the puffs have a certain upstream interaction distance.
