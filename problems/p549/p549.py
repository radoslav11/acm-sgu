import sys
from collections import defaultdict


def solve_dumbbell_sets_corrected(k, dumbbells):
    massToCosts = defaultdict(list)

    for m, c in dumbbells:
        massToCosts[m].append(c)

    frequency = []

    for mass, costs in massToCosts.items():
        costs.sort(reverse=True)
        frequency.append((len(costs), mass))

    frequency.sort(reverse=True, key=lambda x: x[0])

    # Check if we have enough masses with at least C dumbbells
    if len(frequency) < k:
        return 0, 0

    C = frequency[min(k, len(frequency)) - 1][0]

    selectedMasses = []
    for count, mass in frequency:
        if count < C:
            break

        costs = massToCosts[mass]
        sumCosts = sum(costs[:C])
        selectedMasses.append(sumCosts)

    selectedMasses.sort(reverse=True)

    totalCost = sum(selectedMasses[:k])

    return C, totalCost


input_data = sys.stdin.read().splitlines()
n, k = map(int, input_data[0].split())
dumbbells = [tuple(map(int, line.split())) for line in input_data[1:]]

result = solve_dumbbell_sets_corrected(k, dumbbells)

print(result[0], result[1])
