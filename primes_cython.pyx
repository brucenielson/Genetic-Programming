import time

def primes(int nb_primes):
    start = time.time()
    cdef int n, i, len_p
    cdef int p[10000]
    if nb_primes > 10000:
        nb_primes = 10000

    len_p = 0  # The current number of elements in p.
    n = 2
    while len_p < nb_primes:
        # Is n prime?
        for i in p[:len_p]:
            if n % i == 0:
                break

        # If no break occurred in the loop, we have a prime.
        else:
            p[len_p] = n
            len_p += 1
        n += 1

    # Let's return the result in a python list:
    result_as_list  = [prime for prime in p[:len_p]]
    end = time.time()
    print("Time (sec):", end - start)
    return result_as_list