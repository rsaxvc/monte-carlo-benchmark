monte-carlo-benchmark
=====================

monte-carlo statistics program that tests the scalablilty of certain operations

Of note, these programs won't be terribly accurate, as they rely on good random
numbers, but because I didn't feel like putting a good source of random data or
a good PRNG, it might not line up quite right. But that isn't the point of this
benchmark.

First, run `make -j whatever`
Next, cease any intensive work on the machine
Finally, run `make graph.png -j 1`
