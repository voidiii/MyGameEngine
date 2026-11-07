# TODO

- Compare current position Verlet with semi-implicit Euler plus contact impulse solving, using identical initial conditions, gravity and fixed steps. Measure step cost, penetration, resting jitter and energy behavior at comparable stability. Address collision/boundary velocity handling and overlapping spawns; do not assume switching integrators alone fixes stability.
- Add repeatable benchmarks with fixed random seeds and uniform, piled and dense distributions.
