#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include "message.h"
#include "logging.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>

#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

typedef struct bomber {
    int pid;
    int fileno;
    coordinate position;
    bool is_alive;
    int marked;
    int marked_loop_count;
} bomber;

typedef struct bomb {
    int pid;
    int fileno;
    coordinate position;
    int radius;
    bool is_alive;
} bomb;


int bomber_count = 0;
int bomb_count = 0;
int remaining_unmarked_bombers = 0;
int alive_bomber_count = 0;
int alive_bomb_count = 0;
int loop_count = 0;
int map_width, map_height;
int obstacle_count;
int bomb_array_size = 1024;
int status;
obsd *obstacles;
bomber *bombers;
bomb *bombs;
struct pollfd *fds;
int nfds; // total number of fds (bombers + bombs)

void set_poll(void){
    nfds = bomber_count;
    fds = malloc(sizeof( struct pollfd ) * bomb_array_size);
    for(int i = 0; i < nfds; i++){ // add bomber fd's into fds array
        fds[i].fd = bombers[i].fileno;
        fds[i].events = POLL_IN;
    }
}

void set_map(char game_map[map_height][map_width]){
    // set all map to empty
    for(int i = 0; i < map_height;i++){
        for(int j = 0; j < map_width;j++){
            game_map[i][j] = '-';
        }
    }
    // set bombers
    for(int i = 0; i < bomber_count; i++){
        game_map[bombers[i].position.y][bombers[i].position.x] = 'B';
    }
    // set obstacles
    for(int i = 0; i < obstacle_count; i++){
        game_map[obstacles[i].position.y][obstacles[i].position.x] = 'O';
    }
}

void set_pipes(int *pid, int *fileno, char *argv[]){
    int fd[2];
    PIPE(fd);
    *fileno = fd[1];
    *pid = fork();

    if (*pid < 0) { // fork failed
        exit(1);
    }
    else if(*pid == 0){ // bomber or bomb process
        close(fd[1]);
        //close(STDIN_FILENO);
        //close(STDOUT_FILENO);
        dup2(fd[0], STDIN_FILENO);
        dup2(fd[0], STDOUT_FILENO);
        close(fd[0]);
        execvp(argv[0], argv);
    }
    else{ // bgame process
        close(fd[0]);

    }
    return ;
}

void print_map(char game_map[map_height][map_width]){ // debug
    for(int i = 0 ; i < map_height; i++){
        for(int j = 0 ; j < map_width;j++){
            printf("%c:",game_map[i][j]);
        }
        printf("\n");
    }
}

void get_inputs(){
    int bomber_x_position, bomber_y_position, bomber_argument_count,bomber_exec_name;
    int  obstacle_location_x, obstacle_location_y, obstacle_durability;
    scanf(" %d %d %d %d", &map_width, &map_height, &obstacle_count, &bomber_count); // first line
    alive_bomber_count = bomber_count;
    remaining_unmarked_bombers = bomber_count;
    obstacles = malloc(sizeof (obsd) * obstacle_count);
    bombers = malloc(sizeof(bomber) * bomber_count);
    bombs = malloc(sizeof(bomb) * bomb_array_size);
    for(int i = 0; i < obstacle_count;i++){ // set obstacles
        scanf("%d %d %d",&obstacle_location_x,&obstacle_location_y,&obstacle_durability);
        obstacles[i].position.x = obstacle_location_x;
        obstacles[i].position.y = obstacle_location_y;
        obstacles[i].remaining_durability = obstacle_durability;
    }
    for(int i = 0 ; i < bomber_count; i++){ // set bombers
        scanf("%d %d %d",&bomber_x_position,&bomber_y_position,&bomber_argument_count);
        bombers[i].position.x = bomber_x_position;
        bombers[i].position.y = bomber_y_position;
        bombers[i].is_alive = true; //
        bombers[i].fileno = 0;
        bombers[i].pid = 0;
        bombers[i].marked = 0;
        bombers[i].marked_loop_count = 0;
        char *bomber_args[bomber_argument_count + 1];
        char temp[20];
        char *temp2;
        for(int j = 0 ; j < bomber_argument_count; j++) {
            scanf("%s", temp);
            temp2 = malloc(sizeof(char) * 20);
            strcpy(temp2,temp);
            bomber_args[j] = temp2;
        }
        bomber_args[bomber_argument_count] = NULL;
        // call pipe set func here
        set_pipes(&(bombers[i].pid), &(bombers[i].fileno),bomber_args); // set bombers

    }
    return;
}

void check_bomb_list_Size( bomb *bombs ){
    if( bomb_array_size - bomb_count < 64){
        bomb_array_size *= 2;
        bombs = realloc(bombs,bomb_array_size);
        fds = realloc(fds,bomb_array_size);
    }
    return ;
}

void read_bomb_messages(int i,char game_map[map_height][map_width]){
    im imsg;
    imp in;
    if( bombs[i].is_alive == true ) {
        read_data(bombs[i].fileno, &imsg);
        in.pid = bombs[i].pid;
        in.m = &imsg;
        print_output(&in, NULL, NULL, NULL);
        if (imsg.type == BOMB_EXPLODE) {
            //print_map(game_map);
            int count_marked_bombers = 0;
            if (game_map[bombs[i].position.y][bombs[i].position.x] == '#') { // there is bomber at this location
                for (int bomber = 0; bomber < bomber_count; bomber++) {
                    if (bombers[bomber].position.x == bombs[i].position.x && bombers[bomber].position.y == bombs[i].position.y && bombers[bomber].marked == 0) {
                        count_marked_bombers++;
                        game_map[bombs[i].position.y][bombs[i].position.x] = '-';
                        bombers[bomber].marked = 1;
                        bombers[bomber].marked_loop_count = loop_count;
                        break;
                    }
                }
            }
            else if( game_map[bombs[i].position.y][bombs[i].position.x] == 'b' ){
                game_map[bombs[i].position.y][bombs[i].position.x] = '-';
            }
            int x = bombs[i].position.y;
            int y = bombs[i].position.x;
            int flag = 0;
            for (int r = 1; r <= bombs[i].radius && (x - r >= 0); r++) { // check left of the bomb
                if (flag == 1) {
                    break;
                }
                if (game_map[x - r][y] == '#') { // there is bomb and a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == y && bombers[bomber].position.y == x - r && bombers[bomber].marked == 0){
                            count_marked_bombers++;
                            game_map[x - r][y] = 'b';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x - r][y] == 'B') { // there is a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == y && bombers[bomber].position.y == x - r && bombers[bomber].marked == 0) {
                            count_marked_bombers++;
                            game_map[x - r][y] = '-';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x - r][y] == 'O') { // there is an obstacle
                    for (int obstacle = 0; obstacle < obstacle_count; obstacle++) {
                        if (obstacles[obstacle].position.x == y && obstacles[obstacle].position.y == x - r) {
                            flag = 1;
                            obsd o_data; // obstacle_data
                            o_data.position.x = y;
                            o_data.position.y = x - r;
                            if (obstacles[obstacle].remaining_durability == -1) {
                                o_data.remaining_durability = -1;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            }
                            else if (obstacles[obstacle].remaining_durability == 1) {
                                game_map[x - r][y] = '-';
                                obstacles[obstacle].remaining_durability = 0;
                                obstacles[obstacle].position.x = -1; // do this instead of realloc
                                obstacles[obstacle].position.y = -1; // do this instead of realloc
                                o_data.remaining_durability = 0;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            }
                            else {
                                obstacles[obstacle].remaining_durability--;
                                o_data.remaining_durability = obstacles[obstacle].remaining_durability;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            }
                        }
                    }
                }
                else {
                    // do nothing
                }
            }

            flag = 0;
            for (int r = 1; r <= bombs[i].radius && (x + r < map_height); r++) { // check right of the bomb
                if (flag == 1) {
                    break;
                }
                if (game_map[x + r][y] == '#') { // there is bomb and a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == y && bombers[bomber].position.y == x + r && bombers[bomber].marked == 0) {
                            count_marked_bombers++;
                            game_map[x + r][y] = 'b';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x + r][y] == 'B') { // there is a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == y && bombers[bomber].position.y == x + r && bombers[bomber].marked == 0) {
                            count_marked_bombers++;
                            game_map[x + r][y] = '-';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x + r][y] == 'O') { // there is an obstacle
                    for (int obstacle = 0; obstacle < obstacle_count; obstacle++) {
                        if (obstacles[obstacle].position.x == y && obstacles[obstacle].position.y == x + r) {
                            flag = 1;
                            obsd o_data; // obstacle_data
                            o_data.position.x = y;
                            o_data.position.y = x + r;
                            if (obstacles[obstacle].remaining_durability == -1) {
                                o_data.remaining_durability = -1;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            } else if (obstacles[obstacle].remaining_durability == 1) {
                                game_map[x + r][y] = '-';
                                obstacles[obstacle].remaining_durability = 0;
                                obstacles[obstacle].position.x = -1; // do this instead of realloc
                                obstacles[obstacle].position.y = -1; // do this instead of realloc
                                o_data.remaining_durability = 0;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            } else {
                                obstacles[obstacle].remaining_durability--;
                                o_data.remaining_durability = obstacles[obstacle].remaining_durability;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            }
                        }
                    }
                }
                else {
                    // do nothing
                }
            }

            flag = 0;
            for (int r = 1; r <= bombs[i].radius && (y - r >= 0); r++) { // check upside of the bomb
                if (flag == 1) {
                    break;
                }
                if (game_map[x][y - r] == '#') { // there is bomb and a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == (y - r) && bombers[bomber].position.y == x && bombers[bomber].marked == 0) {
                            count_marked_bombers++;
                            game_map[x][y - r] = 'b';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x][y - r] == 'B') { // there is a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == (y - r) && bombers[bomber].position.y == x && bombers[bomber].marked == 0) {
                            count_marked_bombers++;
                            game_map[x][y - r] = '-';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x][y - r] == 'O') { // there is an obstacle
                    for (int obstacle = 0; obstacle < obstacle_count; obstacle++) {
                        if (obstacles[obstacle].position.x == (y - r) && obstacles[obstacle].position.y == x) {
                            flag = 1;
                            obsd o_data; // obstacle_data
                            o_data.position.x = y - r;
                            o_data.position.y = x;
                            if (obstacles[obstacle].remaining_durability == -1) {
                                o_data.remaining_durability = -1;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            } else if (obstacles[obstacle].remaining_durability == 1) {
                                game_map[x][y - r] = '-';
                                obstacles[obstacle].remaining_durability = 0;
                                obstacles[obstacle].position.x = -1; // do this instead of realloc
                                obstacles[obstacle].position.y = -1; // do this instead of realloc
                                o_data.remaining_durability = 0;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            } else {
                                obstacles[obstacle].remaining_durability--;
                                o_data.remaining_durability = obstacles[obstacle].remaining_durability;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            }
                        }
                    }
                }
                else {
                    // do nothing
                }
            }

            flag = 0;
            for (int r = 1; r <= bombs[i].radius && (y + r < map_width); r++) { // check downside of the bomb
                if (flag == 1) {
                    break;
                }
                if (game_map[x][y + r] == '#') { // there is bomb and a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == (y + r) && bombers[bomber].position.y == x && bombers[bomber].marked == 0) {
                            count_marked_bombers++;
                            game_map[x][y + r] = 'b';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x][y + r] == 'B') { // there is a bomber
                    for (int bomber = 0; bomber < bomber_count; bomber++) {
                        if (bombers[bomber].position.x == (y + r) && bombers[bomber].position.y == x && bombers[bomber].marked == 0) {
                            count_marked_bombers++;
                            game_map[x][y + r] = '-';
                            bombers[bomber].marked = 1;
                            bombers[bomber].marked_loop_count = loop_count;
                            break;
                        }
                    }
                }
                else if (game_map[x][y + r] == 'O') { // there is an obstacle
                    for (int obstacle = 0; obstacle < obstacle_count; obstacle++) {
                        if (obstacles[obstacle].position.x == (y + r) && obstacles[obstacle].position.y == x) {
                            flag = 1;
                            obsd o_data; // obstacle_data
                            o_data.position.x = y + r;
                            o_data.position.y = x;
                            if (obstacles[obstacle].remaining_durability == -1) {
                                o_data.remaining_durability = -1;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            } else if (obstacles[obstacle].remaining_durability == 1) {
                                game_map[x][y + r] = '-';
                                obstacles[obstacle].remaining_durability = 0;
                                obstacles[obstacle].position.x = -1; // do this instead of realloc
                                obstacles[obstacle].position.y = -1; // do this instead of realloc
                                o_data.remaining_durability = 0;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            } else {
                                obstacles[obstacle].remaining_durability--;
                                o_data.remaining_durability = obstacles[obstacle].remaining_durability;
                                print_output(NULL, NULL, &o_data, NULL);
                                break;
                            }
                        }
                    }
                }
                else {
                    // do nothing
                }
            }

            //printf("Count marked bombers: %d\n",count_marked_bombers);
            //printf("Remaining unmarked bombers: %d\n",remaining_unmarked_bombers);
            //printf("Alive bombs: %d\n",alive_bomb_count);
            if (count_marked_bombers == remaining_unmarked_bombers && remaining_unmarked_bombers != 0) {
                int max_distance = -1;
                bomber *furthest_bomber;
                for (int k = 0; k < bomber_count; k++) {
                    if (bombers[k].marked == 1 && (bombers[k].is_alive == true) && bombers[k].marked_loop_count == loop_count) {
                        if ((abs(bombers[k].position.y - bombs[i].position.y) +
                             abs(bombers[k].position.x - bombs[i].position.x)) > max_distance) {
                            max_distance = abs(bombers[k].position.y - bombs[i].position.y) +
                                           abs(bombers[k].position.x - bombs[i].position.x);
                            furthest_bomber = bombers + k;
                        }
                    }
                }
                furthest_bomber->marked = 2;
            }
            else if( remaining_unmarked_bombers - count_marked_bombers == 1 ){
                for(int k = 0; k < bomber_count; k++){
                    if( bombers[k].marked == 0 ){
                        bombers[k].marked = 2; // set winner
                    }
                }
            }
            bombs[i].is_alive = false;
            close(bombs[i].fileno);
            waitpid(bombs[i].pid, &status, 0);
            alive_bomb_count--;
            remaining_unmarked_bombers -= count_marked_bombers;
        }
        return;
    }
    return;
}

void read_bomber_messages(int i,char game_map[map_height][map_width]){
    im imsg;
    om omsg;
    imp in;
    omp out;
            // START - SEE - MOVE - PLANT
    if( bombers[i].is_alive == true ) {
        read_data(bombers[i].fileno, &imsg);
        in.pid = bombers[i].pid;
        in.m = &imsg;
        print_output(&in, NULL, NULL, NULL);
        if (bombers[i].marked == 1) { // kill the bomber process
            omsg.type = BOMBER_DIE;
            send_message(bombers[i].fileno, &omsg);
            out.pid = bombers[i].pid;
            out.m = &omsg;
            print_output(NULL, &out, NULL, NULL);
            bombers[i].is_alive = false;
            close(bombers[i].fileno);
            waitpid(bombers[i].pid, &status, 0);
            alive_bomber_count--;
        }
        else if (bombers[i].marked == 2) { // winner bomber
            omsg.type = BOMBER_WIN;
            send_message(bombers[i].fileno, &omsg);
            out.pid = bombers[i].pid;
            out.m = &omsg;
            print_output(NULL, &out, NULL, NULL);
            bombers[i].is_alive = false;
            close(bombers[i].fileno);
            waitpid(bombers[i].pid, &status, 0);
            alive_bomber_count--;
        }
        else if (imsg.type == BOMBER_START) {
            omsg.type = BOMBER_LOCATION;
            omsg.data.new_position.x = bombers[i].position.x;
            omsg.data.new_position.y = bombers[i].position.y;
            send_message(bombers[i].fileno, &omsg);
            out.pid = bombers[i].pid;
            out.m = &omsg;
            print_output(NULL, &out, NULL, NULL);
        }
        else if (imsg.type == BOMBER_SEE) {
            int count = 0;
            od *objects = malloc(25 * sizeof(od));
            int y = bombers[i].position.x;
            int x = bombers[i].position.y;
            if (game_map[x][y] == '#') { // check current position
                objects[count].type = BOMB;
                objects[count].position.x = bombers[i].position.x;
                objects[count].position.y = bombers[i].position.y;
                count++; // there is bomb as well
            }
            for (int j = y - 1; j >= y - 3 && j >= 0; j--) { // check left of the bomber
                if (game_map[x][j] == 'O') {
                    objects[count].type = OBSTACLE;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                    break; // obstacle blocks vision
                } else if (game_map[x][j] == 'B') {
                    objects[count].type = BOMBER;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                } else if (game_map[x][j] == 'b') {
                    objects[count].type = BOMB;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                } else if (game_map[x][j] == '#') { // contains both bomber and bomb
                    objects[count].type = BOMB;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                    objects[count].type = BOMBER;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                } else {
                    // empty cell
                }
            }
            for (int j = y + 1; j <= y + 3 && j < map_width; j++) { // check right of the bomber
                if (game_map[x][j] == 'O') {
                    objects[count].type = OBSTACLE;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                    break; // obstacle blocks vision
                } else if (game_map[x][j] == 'B') {
                    objects[count].type = BOMBER;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                } else if (game_map[x][j] == 'b') {
                    objects[count].type = BOMB;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                } else if (game_map[x][j] == '#') { // contains both bomber and bomb
                    objects[count].type = BOMB;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                    objects[count].type = BOMBER;
                    objects[count].position.x = j;
                    objects[count].position.y = x;
                    count++;
                } else {
                    // empty cell
                }
            }
            for (int k = x - 1; k >= x - 3 && k >= 0; k--) { // check up
                if (game_map[k][y] == 'O') {
                    objects[count].type = OBSTACLE;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                    break; // obstacle blocks vision
                } else if (game_map[k][y] == 'B') {
                    objects[count].type = BOMBER;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                } else if (game_map[k][y] == 'b') {
                    objects[count].type = BOMB;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                } else if (game_map[k][y] == '#') { // contains both bomber and bomb
                    objects[count].type = BOMB;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                    objects[count].type = BOMBER;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                } else {
                    // empty cell
                }
            }
            for (int k = x + 1; k <= x + 3 && k < map_height; k++) { // check down
                if (game_map[k][y] == 'O') {
                    objects[count].type = OBSTACLE;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                    break; // obstacle blocks vision
                } else if (game_map[k][y] == 'B') {
                    objects[count].type = BOMBER;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                } else if (game_map[k][y] == 'b') {
                    objects[count].type = BOMB;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                } else if (game_map[k][y] == '#') { // contains both bomber and bomb
                    objects[count].type = BOMB;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                    objects[count].type = BOMBER;
                    objects[count].position.x = y;
                    objects[count].position.y = k;
                    count++;
                } else {
                    // empty cell
                }
            }
            omsg.type = BOMBER_VISION;
            omsg.data.object_count = count;
            send_message(bombers[i].fileno, &omsg); // send count
            out.pid = bombers[i].pid;
            out.m = &omsg;
            send_object_data(bombers[i].fileno, count, objects); // send objects
            print_output(NULL, &out, NULL, objects);
            free(objects);
        }
        else if (imsg.type == BOMBER_MOVE) {
            omsg.type = BOMBER_LOCATION;
            // if move is not valid
            if (abs(imsg.data.target_position.x - bombers[i].position.x) > 1 ||
                abs(imsg.data.target_position.y - bombers[i].position.y) > 1
                || (abs(imsg.data.target_position.x - bombers[i].position.x) == 1 &&
                    abs(imsg.data.target_position.y - bombers[i].position.y) == 1)
                || game_map[imsg.data.target_position.y][imsg.data.target_position.x] == '#' ||
                game_map[imsg.data.target_position.y][imsg.data.target_position.x] == 'B'
                || game_map[imsg.data.target_position.y][imsg.data.target_position.x] == 'O' ||
                imsg.data.target_position.y >= map_height
                || imsg.data.target_position.x >= map_width || imsg.data.target_position.y < 0 ||
                imsg.data.target_position.x < 0) {

                omsg.data.new_position.y = bombers[i].position.y;
                omsg.data.new_position.x = bombers[i].position.x;
            } else { // if move is valid
                if (game_map[bombers[i].position.y][bombers[i].position.x] == '#') {
                    game_map[bombers[i].position.y][bombers[i].position.x] = 'b';
                } else if (game_map[bombers[i].position.y][bombers[i].position.x] == 'B') {
                    game_map[bombers[i].position.y][bombers[i].position.x] = '-';
                }
                if (game_map[imsg.data.target_position.y][imsg.data.target_position.x] == 'b') {
                    game_map[imsg.data.target_position.y][imsg.data.target_position.x] = '#';
                } else if (game_map[imsg.data.target_position.y][imsg.data.target_position.x] == '-') {
                    game_map[imsg.data.target_position.y][imsg.data.target_position.x] = 'B';
                }
                bombers[i].position.x = imsg.data.target_position.x;
                bombers[i].position.y = imsg.data.target_position.y;
                omsg.data.new_position.y = imsg.data.target_position.y;
                omsg.data.new_position.x = imsg.data.target_position.x;

            }
            send_message(bombers[i].fileno, &omsg);
            out.pid = bombers[i].pid;
            out.m = &omsg;
            print_output(NULL, &out, NULL, NULL);
        }
        else if (imsg.type == BOMBER_PLANT) {
            omsg.type = BOMBER_PLANT_RESULT;
            if (game_map[bombers[i].position.y][bombers[i].position.x] == '#') { // there is bomb and bomber in this location, no plant
                omsg.data.planted = false;
            }
            else {
                char *bomb_args[3];
                int length = snprintf(NULL, 0, "%ld", imsg.data.bomb_info.interval);
                char *interval = malloc(sizeof(char) * (length + 1));
                snprintf(interval, length + 1, "%ld", imsg.data.bomb_info.interval);
                bomb_args[0] = "./bomb";
                bomb_args[1] = interval;
                bomb_args[2] = NULL;
                set_pipes(&bombs[bomb_count].pid, &bombs[bomb_count].fileno, bomb_args);
                free(interval);
                bombs[bomb_count].is_alive = true;
                bombs[bomb_count].radius = imsg.data.bomb_info.radius;
                bombs[bomb_count].position.x = bombers[i].position.x;
                bombs[bomb_count].position.y = bombers[i].position.y;
                fds[nfds].fd = bombs[bomb_count].fileno;
                fds[nfds].events = POLL_IN;
                bomb_count++;
                nfds++;
                game_map[bombers[i].position.y][bombers[i].position.x] = '#';
                omsg.data.planted = true;
                alive_bomb_count++;
            }
            send_message(bombers[i].fileno, &omsg);
            out.pid = bombers[i].pid;
            out.m = &omsg;
            print_output(NULL, &out, NULL, NULL);

        }
        else {

        }
        return;
    }
    return;
}

void wait_bombers_start(void){
    im imsg;
    om omsg;
    imp in;
    omp out;
    for(int i = 0 ; i < bomber_count; i++){
        read_data(bombers[i].fileno, &imsg);
        in.pid = bombers[i].pid;
        in.m = &imsg;
        print_output(&in, NULL, NULL, NULL);
        if( imsg.type == BOMBER_START ){
            omsg.type = BOMBER_LOCATION;
            omsg.data.new_position.x = bombers[i].position.x;
            omsg.data.new_position.y = bombers[i].position.y;
            send_message(bombers[i].fileno, &omsg);
            out.pid = bombers[i].pid;
            out.m = &omsg;
            print_output(NULL, &out, NULL, NULL);
        }
    }
    return;
}

int poll_from_bomb(void){
    for(int fd = 0; fd < nfds; fd++){
        if( fds[fd].revents & POLL_IN ){
            for(int i = 0; i < bomb_count; i++){
                if( fds[fd].fd == bombs[i].fileno && bombs[i].is_alive == true){
                    return i;
                }
            }
            return -1;
        }
    }
}

int poll_from_bomber(void){
    for(int fd = 0; fd < nfds; fd++){
        if( fds[fd].revents & POLL_IN ){
            for(int i = 0; i < bomber_count; i++){
                if( fds[fd].fd == bombers[i].fileno && bombers[i].is_alive == true){
                    return i;
                }
            }
            return -1;
        }
    }
}

int main(void)
{
    get_inputs();

    int ready;
    int ind;
    char game_map[map_height][map_width]; // B for bomber, b for bomb, O for obstacle, - for empty, # for both bomber and bomb
    set_map(game_map);

    set_poll();

    wait_bombers_start();

    while(alive_bomber_count || alive_bomb_count){

        check_bomb_list_Size(bombs);

        ready = poll(fds,nfds,-1);
        ind = poll_from_bomb();
        if( ind != -1){
            read_bomb_messages(ind,game_map);
        }

        ind = poll_from_bomber();
        if( ind != -1 ){
            read_bomber_messages(ind,game_map);
        }
        loop_count++;
        sleep(0.001);
    }
	return 0;
}