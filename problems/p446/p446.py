import math

EPS = 1e-9


def vector_length(v):
    return math.sqrt(v[0] ** 2 + v[1] ** 2)


def vector_angle(v):
    return math.atan2(v[1], v[0])


def rotate_vector(v, angle):
    cos_a = math.cos(angle)
    sin_a = math.sin(angle)
    return (v[0] * cos_a - v[1] * sin_a, v[0] * sin_a + v[1] * cos_a)


def normalize_angle(angle):
    if angle < 0:
        angle += 2 * math.pi
    angle = angle % (2 * math.pi)
    return angle


def get_all_vectors(stars):
    n = len(stars)
    if n == 0:
        return []

    cx = sum(star[0] for star in stars) / n
    cy = sum(star[1] for star in stars) / n

    star_data = []
    for i in range(n):
        dx = stars[i][0] - cx
        dy = stars[i][1] - cy
        vector = (dx, dy)

        if abs(dx) < EPS and abs(dy) < EPS:
            angle = float("inf")
        else:
            angle = math.atan2(dy, dx)
            angle = normalize_angle(angle)

        distance = math.sqrt(dx * dx + dy * dy)
        star_data.append((angle, distance, vector))

    star_data.sort(key=lambda x: (x[0], x[1]))
    vectors = [item[2] for item in star_data]
    return vectors


def vectors_match(vectors1, vectors2, rotation_angle, offset):
    if len(vectors1) != len(vectors2):
        return False

    n = len(vectors1)
    rotated_vectors2 = [rotate_vector(v, rotation_angle) for v in vectors2]

    for i in range(n):
        v1 = vectors1[i]
        v2 = rotated_vectors2[(i + offset) % n]
        if abs(v1[0] - v2[0]) > EPS or abs(v1[1] - v2[1]) > EPS:
            return False
    return True


def solve_rotation_estimation():
    n = int(input())

    stars1 = []
    for _ in range(n):
        x, y = map(float, input().split())
        stars1.append((x, y))

    stars2 = []
    for _ in range(n):
        x, y = map(float, input().split())
        stars2.append((x, y))

    if n == 1:
        return 0.0

    vectors1 = get_all_vectors(stars1)
    vectors2 = get_all_vectors(stars2)

    min_angle = float("inf")

    for i in range(n):
        v1 = vectors1[0]
        v2 = vectors2[i]

        v1_len = vector_length(v1)
        v2_len = vector_length(v2)

        if v1_len < EPS or v2_len < EPS:
            continue

        if abs(v1_len - v2_len) > EPS:
            continue

        v1_angle = vector_angle(v1)
        v2_angle = vector_angle(v2)
        rotation_angle = v1_angle - v2_angle

        rotation_angle = normalize_angle(rotation_angle)
        if vectors_match(
            vectors1, vectors2, rotation_angle, i
        ) or vectors_match(
            vectors1, vectors2, 2 * math.pi - rotation_angle, i
        ):
            min_angle = min(min_angle, rotation_angle)
            min_angle = min(min_angle, 2 * math.pi - rotation_angle)

    if min_angle == float("inf"):
        raise ValueError("No valid rotation found")

    return min_angle


if __name__ == "__main__":
    try:
        result = solve_rotation_estimation()
        print(f"{result:.11f}")
    except ValueError as e:
        print(f"Error: {e}")
        raise
