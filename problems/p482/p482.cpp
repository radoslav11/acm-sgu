#include <algorithm>
#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

using namespace std;

const int16_t MAX_HEIGHT = 101;
const int16_t MAX_PERIMETER = 5300;

int16_t calculate_initial_perimeter(const vector<int16_t>& heights) {
    int16_t perimeter = 2;
    for(int16_t i = 0; i < static_cast<int16_t>(heights.size()) - 1; ++i) {
        perimeter += 2 + abs(heights[i] - heights[i + 1]);
    }
    perimeter += heights[0] + heights.back();
    return perimeter;
}

vector<int16_t> reconstruct_stolen_boards(
    const vector<vector<vector<int16_t>>>& dp, const vector<int16_t>& heights,
    int16_t max_height_last_height, int16_t max_height_perimeter
) {
    vector<int16_t> stolen_boards;
    int16_t current_height = max_height_last_height;
    int16_t current_perimeter = max_height_perimeter;

    for(int16_t pos = static_cast<int16_t>(heights.size()); pos > 0; --pos) {
        if(dp[pos][current_height][current_perimeter] ==
               dp[pos - 1][current_height][current_perimeter] +
                   heights[pos - 1] &&
           dp[pos - 1][current_height][current_perimeter] != -1) {
            stolen_boards.push_back(pos);
        } else {
            for(int16_t prev_height = 0; prev_height < MAX_HEIGHT;
                ++prev_height) {
                int16_t prev_perimeter =
                    current_perimeter - abs(prev_height - current_height) - 2;
                if(prev_perimeter >= 0 &&
                   dp[pos - 1][prev_height][prev_perimeter] ==
                       dp[pos][current_height][current_perimeter]) {
                    current_height = prev_height;
                    current_perimeter = prev_perimeter;
                    break;
                }
            }
        }
    }
    sort(stolen_boards.begin(), stolen_boards.end());
    return stolen_boards;
}

tuple<int16_t, int16_t, vector<int16_t>> solve_board_theft(
    int16_t n, const vector<int16_t>& heights
) {
    vector<vector<vector<int16_t>>> dp(
        n + 1,
        vector<vector<int16_t>>(MAX_HEIGHT, vector<int16_t>(MAX_PERIMETER, -1))
    );
    dp[0][0][0] = 0;

    int16_t initial_perimeter = calculate_initial_perimeter(heights);
    int16_t target_perimeter = (initial_perimeter + 1) / 2;

    for(int16_t pos = 0; pos < n; ++pos) {
        for(int16_t last_height = 0; last_height < MAX_HEIGHT; ++last_height) {
            for(int16_t perimeter = 0; perimeter < MAX_PERIMETER; ++perimeter) {
                if(dp[pos][last_height][perimeter] == -1) {
                    continue;
                }

                int16_t new_height = heights[pos];
                dp[pos + 1][last_height][perimeter] = max<int16_t>(
                    dp[pos + 1][last_height][perimeter],
                    dp[pos][last_height][perimeter] + new_height
                );

                int16_t new_perimeter =
                    perimeter + abs(last_height - new_height) + 2;
                if(new_perimeter < MAX_PERIMETER) {
                    dp[pos + 1][new_height][new_perimeter] = max<int16_t>(
                        dp[pos + 1][new_height][new_perimeter],
                        dp[pos][last_height][perimeter]
                    );
                }
            }
        }
    }

    int16_t max_height = -1, max_height_perimeter = 0,
            max_height_last_height = 0;
    for(int16_t last_height = 0; last_height < MAX_HEIGHT; ++last_height) {
        for(int16_t perimeter = 0; perimeter < MAX_PERIMETER; ++perimeter) {
            if(perimeter + last_height >= target_perimeter &&
               dp[n][last_height][perimeter] > max_height) {
                max_height = dp[n][last_height][perimeter];
                max_height_perimeter = perimeter;
                max_height_last_height = last_height;
            }
        }
    }

    vector<int16_t> stolen_boards = reconstruct_stolen_boards(
        dp, heights, max_height_last_height, max_height_perimeter
    );
    return {
        max_height, static_cast<int16_t>(stolen_boards.size()), stolen_boards
    };
}

int main() {
    int16_t n;
    cin >> n;
    vector<int16_t> heights(n);
    for(int16_t i = 0; i < n; ++i) {
        cin >> heights[i];
    }

    auto [total_height, num_boards, board_indices] =
        solve_board_theft(n, heights);
    cout << total_height << endl;
    cout << num_boards << endl;
    for(int16_t index: board_indices) {
        cout << index << " ";
    }
    cout << endl;

    return 0;
}
