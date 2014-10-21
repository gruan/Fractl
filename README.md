Fractl
======

Using OpenGL and SFML created a realtime image of a quaternion fractal.

Two similar images are projected each corresponding to their respective eye that they project onto.

Leap Motion was mounted onto the Oculus Rift for motion sensing interactivity.

Using the Leap Motion API, the quaternion fractal could be interacted with in three ways.

1. Enlarging the quaternion fractal by placing both hands into the vision field of the Leap Motion and moving each hand away from each other while having the palms of each hand face each other.
2. Rotating the quaternion fractal in the x-axis by swiping a hand in the desired direction across the vision field of the Leap Motion.
3. Changing the four parameters of the quaternion fractal. Each finger corresponds to a parameter, excluding the thumb. By pointing and moving a finger to its respective counterpart on the other hand to a relatively close distance without touching, the parameters can be changed by moving the fingers up and down simultaneously.
