# Tic-Tac-Toe Game
## To run the code:
```
g++ game.cpp
./a.exe 
```
## Implementation:
* Helper Functions: 
    > valid_move() checks if the point that we are willing to place 'X' or 'O' in is within our board's boundaries

    > player() returns the player who is the next to play according to the turn variable (which is global variable, and if it is an odd number, it indicates that it is X's turn and O's turn, otherwise)

    > out() is implemented to ease the debugging process

    > initial_state() initializes the board with dots

    > actions() takes a board as input and search for empty cells

    > result() returns the next state, given the current state and the action

    > check() returns if we get k consecutive cells with the same value (except '.')

    > winner() checks all cells and directions wo find if there is a winner

    > terminal() checks if the game continues

    > utility() returns 1 for X's win, -1 O's win, and 0 for a draw
* Evaluation Function:
    > eval_f() = 10 * (X's possible wins) - 10 * (O's possible wins)
* Ordering moves:
    > the function sorts all the moves according to their priorities, which is determined by its location in the board

## Performance Analysis: Minimax vs Alpha-Beta Pruning (3x3) & Heuristic Search (4x4)

### 1. **Minimax vs Alpha-Beta Pruning (3x3 Grid)**

| **Algorithm**       | **Game Tree Depth (d)** | **Branching Factor (b)** | **Nodes Explored (Approx.)** | **Time Complexity**     |
|---------------------|-------------------------|--------------------------|-----------------------------|-------------------------|
| **Minimax**         | 9                       | 9                        | 362,880                     | \( O(9^9) \)            |
| **Alpha-Beta**     | 9                       | 9                        | Much fewer                  | \( O(9^{4.5}) \)        |

- **Minimax** explores all possible states, resulting in an inefficient search.
- **Alpha-Beta Pruning** reduces the number of nodes explored, leading to much faster execution.

### 2. **Heuristic Search (4x4 Grid): With vs Without Move Ordering**

| **Algorithm**        | **Game Tree Depth (d)** | **Branching Factor (b)** | **Nodes Explored (Approx.)** | **Time Complexity**   |
|----------------------|-------------------------|--------------------------|-----------------------------|-----------------------|
| **Without Ordering** | 16                      | 16                       | \( 16^16 \)               | \( O(16^16) \)      |
| **With Ordering**    | 16                      | 16                       | Much fewer                  | \( O(16^8) \)       |

- **Without Ordering**: No intelligent move prioritization, leading to larger search trees.
- **With Ordering**: Prioritizing moves (e.g., center, corners) improves pruning and reduces nodes.

### 3. **Search Depth and Limits**

As the grid size and depth increase, the complexity grows exponentially. For example:

- **3x3 Grid**:
  - Depth 3: \( O(9^3) = 729 \) nodes
  - Depth 5: \( O(9^5) = 59,049 \) nodes
  
- **4x4 Grid**:
  - Depth 3: \( O(16^3) = 4,096 \) nodes
  - Depth 5: \( O(16^5) = 1,048,576 \) nodes

- For larger grids or deeper searches, the problem becomes computationally prohibitive.

### 4. **Limitations**

- **Minimax vs Alpha-Beta**: Alpha-Beta pruning significantly reduces the search space, making it more efficient.
- **Heuristic Evaluation**: Helps approximate board utility faster but doesn't fully replace exhaustive search.
- **Grid Size/Depth**: As grid size or search depth increases, the game becomes computationally intractable. Hybrid approaches (e.g., MCTS or neural networks) may be needed for larger grids.

