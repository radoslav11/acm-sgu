# Stress Testing Skill

Help the user create a comprehensive stress testing setup to find bugs in their competitive programming solution by comparing it against a brute force solution.

## Process

1. **Understand the problem**: Ask the user about the problem they're solving if not already clear from context.

2. **Create a brute force solution**:
   - Ask the user if they already have a brute force solution
   - If not, create a `<problem>_brute.cpp` file with a simple, obviously correct O(N^3) or exponential solution
   - The brute force should prioritize correctness over efficiency
   - Use meaningful variable names and clear logic
   - For combinatorial problems, try all possibilities (e.g., 3^N state enumeration)
   - Compile and test it on the provided examples first

3. **Create a test generator**:
   - Create a `gen.cpp` (or `gen_small.cpp` for smaller cases) that generates random test cases
   - Take a seed as command line argument: `int seed = (argc > 1) ? atoi(argv[1]) : time(0);`
   - Use `mt19937 rng(seed);` for reproducible random generation
   - For stress testing, keep N small (3-10) so brute force runs quickly
   - Generate valid inputs according to problem constraints
   - Print the test case to stdout in the exact input format

4. **Create a stress test script**:
   - Create `stress.sh` that:
     - Compiles all solutions (main, brute, generator)
     - Runs tests in a loop with different seeds
     - Compares outputs using `diff -w`
     - Stops and shows the failing test case when a difference is found
   - Make it executable: `chmod +x stress.sh`

5. **Run the stress test**:
   - Execute `./stress.sh`
   - If a failing test is found, save it for debugging
   - Analyze the difference between expected and actual output
   - Debug the main solution based on the failing case

6. **Iterate**:
   - Fix bugs in the main solution
   - Re-run stress test
   - Repeat until many tests pass (e.g., 1000+ consecutive tests)

## Template for stress.sh

```bash
#!/bin/bash

g++ -std=c++17 -O2 <problem>.cpp -o <problem>
g++ -std=c++17 -O2 <problem>_brute.cpp -o <problem>_brute
g++ -std=c++17 -O2 gen.cpp -o gen

for i in {1..1000}; do
    echo "Test $i"
    ./gen $i > test.in

    ./<problem> < test.in > out1.txt
    ./<problem>_brute < test.in > out2.txt

    if ! diff -w out1.txt out2.txt > /dev/null; then
        echo "Found difference on test $i!"
        echo "Input:"
        cat test.in
        echo ""
        echo "Main solution output:"
        cat out1.txt
        echo ""
        echo "Brute force output:"
        cat out2.txt
        exit 1
    fi
done

echo "All tests passed!"
```

## Tips

- Start with small N values (3-7) for the brute force to run quickly
- If tests pass, gradually increase N or test count
- Save failing test cases for regression testing
- Consider creating both `gen.cpp` (larger N) and `gen_small.cpp` (smaller N) for different testing phases
- Use meaningful seed values to reproduce specific test cases
- Check both "wrong answer" and "false positive" bugs

## Common Brute Force Strategies

- **Graph/Tree problems**: Try all possible subsets/partitions
- **Optimization problems**: Enumerate all valid solutions and pick best
- **Yes/No problems**: Try all possibilities and check constraints
- **Combinatorial problems**: Use recursion with backtracking (3^N, 2^N)
- **Geometry**: Use brute force O(N^3) or O(N^4) algorithms

## When to Use

- After implementing a complex solution
- When getting Wrong Answer on online judge
- Before submission to catch edge cases
- When unsure about correctness of optimization
