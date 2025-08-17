import sys

sys.set_int_max_str_digits(100000)


class InputParser:
    def __init__(self):
        self.tokens = []
        self.index = 0

        # Read all input and split into tokens
        for line in sys.stdin:
            self.tokens.extend(line.strip().split())

    def next_int(self):
        """Read next integer from input."""
        result = int(self.tokens[self.index])
        self.index += 1
        return result

    def next_string(self):
        """Read next string from input."""
        result = self.tokens[self.index]
        self.index += 1
        return result

    def next_float(self):
        """Read next float from input."""
        result = float(self.tokens[self.index])
        self.index += 1
        return result


def main():
    # Problem: Count the number of strings of length N that are accepted by a DFA
    #          with non-absorbing edges.
    #
    # A DFA with non-absorbing edges has:
    # - Σ: input alphabet
    # - U: set of states
    # - s: initial state
    # - T: set of terminal states
    # - φ: transition function U × Σ → U
    # - χ: absorption function U × Σ → {0, 1}
    #
    # When χ(u, c) = 1, the character is not consumed (non-absorbing edge).
    # When χ(u, c) = 0, the character is consumed normally.
    #
    # Solution approach:
    # 1. Precompute effective transitions: For each (state, char), find the final state
    #    after following all non-absorbing transitions. This is to make the later DP easier.
    # 2. Use DP: dp[i][state] = number of ways to reach 'state' after consuming exactly i characters.
    #    We then simply try all other characters and figure out the state we reach using the
    #    precompute.
    #
    # The main reason we implement in Python is because in C++ we would need big integers.
    # Unfortunately, the original constraints are a bit too large for Python to pass, so we also implement
    # this in C++.

    # Read input alphabet
    alphabet = input().strip()
    alphabet_size = len(alphabet)

    parser = InputParser()
    k = parser.next_int()

    initial_state = parser.next_int() - 1
    num_terminal = parser.next_int()

    terminal_states = []
    for _ in range(num_terminal):
        terminal_states.append(parser.next_int() - 1)

    phi = [[0] * alphabet_size for _ in range(k)]
    for i in range(k):
        for j in range(alphabet_size):
            phi[i][j] = parser.next_int() - 1

    chi = [[0] * alphabet_size for _ in range(k)]
    for i in range(k):
        for j in range(alphabet_size):
            chi[i][j] = parser.next_int()

    n = parser.next_int()

    # Precompute effective transitions by resolving non-absorbing edges
    # trans[state][char] = final state after following all non-absorbing transitions
    trans = [[None] * alphabet_size for _ in range(k)]

    def dfs(state, char):
        """
        Resolve the chain of non-absorbing transitions starting from (state, char).
        Sets trans[state][char] to the final state where the character
        will actually be consumed. Sets to -1 if there's an infinite loop of
        non-absorbing edges.
        """
        if chi[state][char] == 0:
            # This is an absorbing edge, character gets consumed here
            trans[state][char] = state
            return

        if trans[state][char] is not None:
            return

        trans[state][char] = -1

        next_state = phi[state][char]
        dfs(next_state, char)
        trans[state][char] = trans[next_state][char]

    # Compute all effective transitions
    for state in range(k):
        for char in range(alphabet_size):
            if trans[state][char] is None:
                dfs(state, char)

    # Dynamic Programming
    # dp[i][state] = number of ways to be in 'state' after consuming
    #                exactly i characters
    dp = [[0] * k for _ in range(n + 1)]
    dp[0][initial_state] = 1

    # Fill DP table
    for length in range(n):
        for current_state in range(k):
            if dp[length][current_state] == 0:
                continue

            # Try each character in the alphabet
            for char in range(alphabet_size):
                absorbing_state = trans[current_state][char]
                assert absorbing_state is not None
                if absorbing_state == -1:
                    # Infinite loop of non-absorbing edges - skip
                    continue

                # Find where we end up after consuming this character
                # We follow non-absorbing edges to trans[current_state][char],
                # then consume the character and transition normally
                next_state = phi[absorbing_state][char]
                dp[length + 1][next_state] += dp[length][current_state]

    # Count strings that end in terminal states
    result = 0
    for terminal_state in terminal_states:
        result += dp[n][terminal_state]

    print(result)


if __name__ == "__main__":
    main()
