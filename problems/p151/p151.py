import math


def find_triangle_coordinates(b, c, m):
    # m^2 = 1/4 (2b^2 + 2c^2 - a^2)

    inner_val = 2 * b**2 + 2 * c**2 - 4 * m**2
    if inner_val < 0:
        return None

    a = math.sqrt(inner_val)
    if a > b + c or b > a + c or c > a + b:
        return None

    # Use cosine law to find angles
    cos_C = (b**2 + c**2 - a**2) / (2 * b * c)

    Bx = c
    By = 0

    Cx = b * cos_C 
    Cy = b * math.sqrt(1 - cos_C**2)

    # Return coordinates of A, B, C
    return [(0, 0), (Bx, By), (Cx, Cy)]


def main():
    c, b, m = map(float, input().split())
    coords = find_triangle_coordinates(b, c, m)

    if coords is None:
        print("Mission impossible")
        return

    for x, y in coords:
        print(f"{x:.6f} {y:.6f}")


if __name__ == "__main__":
    main()
