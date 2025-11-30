#include <bits/stdc++.h>
using namespace std;

#define int long long
#define all(x) x.begin(), x.end()

const int N = 2e5 + 5, M = 1e9 + 7, inf = 1e9, inf64 = 1e18;
int turn = 1, m = 3, k = 3;

// validate the move within the grid boundaries
bool valid_move(int x, int y) {
    return (x > 0 && x <= m && y > 0 && y <= m);
}

// determines the current player (X: 1, O: 0)
int player() {
    return (turn % 2 ? 1 : 0);
}

// function to print the current board state
void out(vector<vector<char>> board) {
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= m; j++) {
            cout << board[i][j] << ' ';
        }
        cout << '\n';
    }
}

// initializes the game board with '.' (empty spots)
vector<vector<char>> initial_state(int m = 3, int k = 3) {
    vector<vector<char>> board(m + 1, vector<char>(m + 1, '.'));
    return board;
}

// generates all legal moves (empty spots)
vector<array<int, 2>> actions(vector<vector<char>> board) {
    vector<array<int, 2>> res;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= m; j++) {
            if (board[i][j] == '.') res.push_back({i, j});
        }
    }
    return res;
}

// performs the move by the current player at position (x, y)
vector<vector<char>> result(vector<vector<char>> board, array<int, 2> action) {
    board[action[0]][action[1]] = (player() ? 'X' : 'O');
    return board;
}

// checks if there is a winner in the line starting from (x, y) with direction (dx, dy)
bool check(vector<vector<char>> board, int x, int y, int new_x, int new_y) {
    char temp = board[x][y];
    for (int l = 1; l < k; l++) {
        x += new_x;
        y += new_y;
        if (board[x][y] != temp) return 0;
    }
    return 1;
}

// determines the winner of the game ('X', 'O', 'D' for draw, 'C' for continuing)
char winner(vector<vector<char>> board) {
    // check rows and columns
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= m - k + 1; ++j) {
            if (board[i][j] != '.' && check(board, i, j, 0, 1)) return board[i][j];
            if (board[j][i] != '.' && check(board, j, i, 1, 0)) return board[j][i];
        }
    }
    // check diagonals
    for (int i = 1; i <= m - k + 1; ++i) {
        for (int j = 1; j <= m - k + 1; ++j) {
            if (board[i][j] != '.' && check(board, i, j, 1, 1)) return board[i][j];
            if (board[i][m - j - 1] != '.' && check(board, i, m - j - 1, 1, -1)) return board[i][m - j - 1];
        }
    }
    // 'D' -> draw
    if (actions(board).empty()) return 'D';
    // 'C' -> the game continues
    return 'C';
}

// determines if the game has ended (either a winner or draw)
bool terminal(vector<vector<char>> board) {
    return winner(board) != 'C';
}

// returns the utility of the board (-1 for O win, 1 for X win, 0 for draw)
int utility(vector<vector<char>> board) {
    char temp = winner(board);
    switch (temp) {
        case 'X':
            return 1;
        case 'O':
            return -1;
        case 'D':
            return 0;
        default:
            return inf64;
    }
}

// minimax implementation without pruning
int minimax(vector<vector<char>> board) {
    if (terminal(board)) {
        return utility(board);  // utility when terminal state is reached
    }
    vector<vector<char>> new_board;
    vector<array<int, 2>> moves = actions(board);
    int res, score;

    // X's turn (maximize)
    if (player() == 1) {
        res = -inf;
        for (auto &[x, y] : moves) {
            new_board = result(board, {x, y});
            turn++;
            score = minimax(new_board);
            res = max(res, score);
            turn--;
        }
    }
    // O's turn (minimize)
    else {
        res = inf;
        for (auto &[x, y] : moves) {
            new_board = result(board, {x, y});
            turn++;
            score = minimax(new_board);
            res = min(res, score);
            turn--;
        }
    }
    return res;
}

// alpha-beta pruning implementation
int minimax_ab(vector<vector<char>> board, int alpha = -inf, int beta = inf) {
    if (terminal(board)) return utility(board);

    int res = (player() == 1) ? -inf : inf;
    for (auto& action : actions(board)) {
        vector<vector<char>> new_state = result(board, action);
        turn++;
        int score = minimax_ab(new_state, alpha, beta);
        turn--;

        if (player() == 1) {  // Max's turn (X)
            res = max(res, score);
            alpha = max(alpha, res);
            if(res >= beta) break;
        } else {  // Min's turn (O)
            res = min(res, score);
            beta = min(beta, res);
            if(res <= alpha) break;
        }
    }
    return res;
}

// heuristic evaluation function
int heuristic_evaluation(vector<vector<char>> board) {
    int score = 0;
    auto evaluate_line = [&](int x, int y, int dx, int dy) {
        int x_count = 0, o_count = 0, empty_count = 0;

        // check the line for k consecutive spots
        for (int i = 0; i < k; i++) {
            int nx = x + i * dx;
            int ny = y + i * dy;
            if (valid_move(nx, ny)) {
                if (board[nx][ny] == 'X') x_count++;
                if (board[nx][ny] == 'O') o_count++;
                if (board[nx][ny] == '.') empty_count++;
            }
        }

        // evaluate potential wins or near wins
        if (x_count == k) return 10;   // X wins this line
        if (o_count == k) return -10;  // O wins this line

        return 0;
    };

    // evaluate rows, columns, and diagonals
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= m - k + 1; j++) {
            score += evaluate_line(i, j, 0, 1); 
        }
    }
    for (int j = 1; j <= m; j++) {
        for (int i = 1; i <= m - k + 1; i++) {
            score += evaluate_line(i, j, 1, 0); 
        }
    }
    for (int i = 1; i <= m - k + 1; i++) {
        for (int j = 1; j <= m - k + 1; j++) {
            score += evaluate_line(i, j, 1, 1);  // [\]
            score += evaluate_line(i, m - j + 1, 1, -1); // [/]
        }
    }
    return score;
}

vector<array<int, 2>> order_moves(vector<vector<char>> board) {
    vector<array<int, 2>> moves = actions(board);
    
    // center > corners > edges
    sort(moves.begin(), moves.end(), [&](auto &a, auto &b) {
        int priority_a = (a[0] == m / 2 && a[1] == m / 2) ? 3 : (a[0] % (m-1) == 0 && a[1] % (m-1) == 0) ? 2 : 1;
        int priority_b = (b[0] == m / 2 && b[1] == m / 2) ? 3 : (b[0] % (m-1) == 0 && b[1] % (m-1) == 0) ? 2 : 1;
        return priority_a > priority_b;
    });

    return moves;
}

// alpha-beta with move ordering
int minimax_ab_with_ordering(vector<vector<char>> board, int alpha = -inf, int beta = inf) {
    if (terminal(board)) return utility(board);

    vector<vector<char>> new_board;
    vector<array<int, 2>> moves = order_moves(board);
    int res = (player() == 1) ? -inf : inf;
    for (auto &[x, y] : moves) {
        new_board = result(board, {x, y});
        turn++;
        int score = minimax_ab_with_ordering(new_board, alpha, beta);
        turn--;

        if (player() == 1) {  // Max's turn (X)
            res = max(res, score);
            alpha = max(alpha, res);
            if(res >= beta) break;
        } else {  // Min's turn (O)
            res = min(res, score);
            beta = min(beta, res);
            if(res <= alpha) break;
        }
    }
    return res;
}


int search(vector<vector<char>> board, int depth, int max_depth) {
    if (depth == max_depth || terminal(board)) {
        return heuristic_evaluation(board);
    }
    vector<vector<char>> new_board;
    vector<array<int, 2>> moves = actions(board);
    int res, score;
    // X's turn (maximize)
    if (player() == 1) {
        res = -inf;
        for (auto &[x, y] : moves) {
            new_board = result(board, {x, y});
            turn++;
            score = search(new_board, depth + 1, max_depth);
            res = max(res, score);
            turn--;
        }
    }
    // O's turn (minimize)
    else {
        res = inf;
        for (auto &[x, y] : moves) {
            new_board = result(board, {x, y});
            turn++;
            score = search(new_board, depth + 1, max_depth);
            res = min(res, score);
            turn--;
        }
    }
    return res;
}

// tests
void test_initial_state() {
    vector<vector<char>> board = initial_state();
    for(int i = 1; i <= m; i++) {
        for(int j = 1; j <= m; j++) {
            assert(board[i][j] == '.');
        }
    }
    cout << "Test passed: initial_state\n";
}

void test_actions() {
    vector<vector<char>> board = initial_state();
    vector<array<int, 2>> actions_list = actions(board);
    assert(actions_list.size() == 9);
    cout << "Test passed: actions\n";
}

void test_result() {
    vector<vector<char>> board = initial_state();
    array<int, 2> move = {1, 1};
    vector<vector<char>> new_board = result(board, move);
    assert(new_board[1][1] == 'X');
    cout << "Test passed: result\n";
}

void test_winner() {
    vector<vector<char>> board = initial_state();
    board[1][1] = 'X';
    board[1][2] = 'X';
    board[1][3] = 'X';
    assert(winner(board) == 'X');
    cout << "Test passed: winner\n";
}

void test_terminal() {
    vector<vector<char>> board = initial_state();
    board[1][1] = 'X';
    board[1][2] = 'X';
    board[1][3] = 'X';
    assert(terminal(board));
    cout << "Test passed: terminal\n";
}

void test_minimax_3x3() {
    vector<vector<char>> board = initial_state();
    turn = 1;  // X's turn
    int minimax_result = minimax(board);
    turn = 1;
    int minimax_ab_result = minimax_ab(board);
    assert(minimax_result == minimax_ab_result);
    cout << "Test passed: minimax_3x3\n";
}

void test_minimax_ab_pruning() {
    vector<vector<char>> board = initial_state();
    board[1][1] = 'X';
    board[2][2] = 'O';
    turn = 1;
    int minimax_ab_result = minimax_ab(board);
    turn = 1;
    int minimax_result = minimax(board);
    assert(minimax_ab_result == minimax_result);
    cout << "Test passed: minimax_ab_pruning\n";
}

void test_heuristic_evaluation() {
    vector<vector<char>> board = initial_state();
    board[1][1] = 'X';
    board[1][2] = 'X';
    board[1][3] = 'X';
    turn = 1;
    int heuristic_score = heuristic_evaluation(board);
    assert(heuristic_score == 10);
    cout << "Test passed: heuristic_evaluation\n";
}

void test_move_ordering() {
    vector<vector<char>> board = initial_state();
    vector<array<int, 2>> ordered_moves = order_moves(board);
    assert(ordered_moves[0][0] == m / 2 && ordered_moves[0][1] == m / 2);
    cout << "Test passed: move_ordering\n";
}

void test_draw_scenario() {
    vector<vector<char>> board = initial_state();
    board[1][1] = 'X';
    board[1][2] = 'O';
    board[1][3] = 'X';
    board[2][1] = 'O';
    board[2][2] = 'X';
    board[2][3] = 'O';
    board[3][1] = 'O';
    board[3][2] = 'X';
    board[3][3] = 'O';
    assert(winner(board) == 'D');
    assert(terminal(board));
    cout << "Test passed: draw_scenario\n";
}

void test_search_function() {
    vector<vector<char>> board = initial_state();
    int max_depth = 3;
    turn = 1;  // X's turn
    int minimax_dls_result = search(board, 0, max_depth);
    cout << "Test passed: search function with max depth " << max_depth << "\n";
}

int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // run all tests
    test_initial_state();
    test_actions();
    test_result();
    test_winner();
    test_terminal();
    test_minimax_3x3();
    test_minimax_ab_pruning();
    test_heuristic_evaluation();
    test_move_ordering();
    test_draw_scenario();
    test_search_function();

    /*
        to run the functions for different 
        states you could use initial_state()
        function by providing it with m and k 
        or by creating the state manually (don't forget to change global m and k 
        variables)
    */
    return 0;
}
