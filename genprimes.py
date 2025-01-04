
def primes():
    """
    Generate prime numbers.

    This is a variant of Dijkstra's prime number finding algorithm,
    from Tannenbaum's paper.

    Note: Prime numbers and multiples of those prime numbers are
          kept in memory, so it's not extremely memory efficient.
    """
    P = [0]
    V = [0]

    P.append(2)
    yield 2

    x = 1
    square = 4

    while True:
        x += 2

        if x >= square:
            V.append(square)
            square = P[len(V)] ** 2

        for k in range(len(V)):
            if x > V[k]:
                V[k] += P[k]

            if x == V[k]:
                break

        else:
            P.append(x)
            yield x


if __name__ == "__main__":
    import sys
    try:
        N = int(sys.argv[1])
    except (KeyError, ValueError):
        print(f"Usage: {sys.argv[0]} N", file=sys.stderr)
        sys.exit(2)

    for i, p in enumerate(primes()):
        if p >= N:
            break

        print(p, file=sys.stdout)
