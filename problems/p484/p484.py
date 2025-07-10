n, m = map(int, input().split())
grid = [list(input()) for _ in range(n)]
for i in range(n):
    for j in range(m):
        if grid[i][j] == 'P':
            x, y = i, j
pos = y
i = x + 1
while i < n:
    if grid[i][pos] == '\\':
        pos += 1
        if pos < m and grid[i][pos] == '/':
            pos = -1
    elif grid[i][pos] == '/':
        pos -= 1
        if pos >= 0 and grid[i][pos] == '\\':
            pos = -1
    if pos < 0 or pos >= m:
        pos = -2
        break
    i += 1
print(pos + 1)
