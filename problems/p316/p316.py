def process_match():
    N, M = map(int, input().split())
    shots = [tuple(map(int, input().split())) for _ in range(M)]

    hit_points = [100] * N
    score_points = [0] * N

    for shot in shots:
        shooter, target = shot
        shooter -= 1
        target -= 1

        if hit_points[target] > 0:
            score_points[shooter] += 3

        hit_points[target] -= 8

    for i in range(N):
        if hit_points[i] > 0:
            score_points[i] += hit_points[i] // 2

    for i in range(N):
        print(hit_points[i], score_points[i])


process_match()
