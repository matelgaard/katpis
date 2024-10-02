#include <stdio.h>
#include <stdlib.h>

int *board;
int board_size;

typedef struct square {
    int x;
    int y;
} square;

int KNIGHT_MOVES[] = {
     1,  2,
    -1,  2,
     1, -2,
    -1, -2,
     2,  1,
    -2,  1,
     2, -1,
    -2, -1,
};

int main(int argc, char *argv[]) {
    scanf("%i", &board_size);

    board = malloc(board_size * board_size * sizeof(int));

    square knight_start_square;

    char *row = malloc(board_size * sizeof(char));

    // Set up board
    for (int i = 0; i < board_size; i++) {
        scanf("%s", row);
        for (int j = 0; j < board_size; j++) {
            char c = row[j];
            // printf("%c\n", c);
            switch (c) {
                case '.': board[i * board_size + j] = 1; break;
                case '#': board[i * board_size + j] = 0; break;
                case 'K': knight_start_square = (square){ j, i }; break;
            }
        }
    }

    free(row);

    // printf("%i, %i\n", knight_start_square.x, knight_start_square.y);

    square queue[65536];
    int queue_in = 1;
    int queue_out = 0;
    
    queue[queue_out] = knight_start_square;
    
    int start_index = knight_start_square.y * board_size + knight_start_square.x;
    board[start_index] = 0;
    
    int num_moves = -1;

    // printf("Hello");

    int depth = -1;
    int next_depth_index = 0;
    while (queue_in != queue_out) {
        square knight_square = queue[queue_out];

        if (queue_out == next_depth_index) {
            depth++;
            next_depth_index = queue_in;
            // printf("%i\n", depth);
        }

        if (knight_square.x == 0 &&
            knight_square.y == 0) {
            num_moves = depth;
            break;
        }

        queue_out = (queue_out + 1) & 0xFFFF;

        // printf("%i, %i\n", knight_square.x, knight_square.y);

        for (int i = 0; i < 8; i++) {
            square neighbor_square = { knight_square.x + KNIGHT_MOVES[i * 2],
                                       knight_square.y + KNIGHT_MOVES[i * 2 + 1] };
            
            if (neighbor_square.x < 0 || neighbor_square.x >= board_size ||
                neighbor_square.y < 0 || neighbor_square.y >= board_size)
                continue;
            
            if (!board[neighbor_square.y * board_size + neighbor_square.x])
                continue;
            
            queue[queue_in] = neighbor_square;
            queue_in = (queue_in + 1) & 0xFFFF;
            
            int index = neighbor_square.y * board_size + neighbor_square.x;
            board[index] = 0;
        }
    }
    
    printf("%i", num_moves);
    
    free(board);
    
    return 0;
}
