import time

def primes_python(nb_primes):
    start = time.time()
    p = []
    n = 2
    while len(p) < nb_primes:
        # Is n prime?
        for i in p:
            if n % i == 0:
                break

        # If no break occurred in the loop
        else:
            p.append(n)
        n += 1
    end = time.time()
    print("Time (sec):", end-start)
    return p



