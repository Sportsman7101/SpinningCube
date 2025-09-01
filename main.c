#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include ".\constants.h"


//VARIABLES FOR RUNNING GAME
int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


typedef struct {
    mat3x8F game_pos;
    mat2x8F rendered_pos;
} cube;

cube shown_cube = {
    .game_pos = {
        .vec1 = {
            .x = -100,
            .y = -100,
            .z = -100
        },
        .vec2 = {
            .x = -100,
            .y = 100,
            .z = -100
        },
        .vec3 = {
            .x = 100,
            .y = 100,
            .z = -100
        },
        .vec4 = {
            .x = 100,
            .y = -100,
            .z = -100
        },
        .vec5 = {
            .x = -100,
            .y = -100,
            .z = 100
        },
        .vec6 = {
            .x = -100,
            .y = 100,
            .z = 100
        },
        .vec7 = {
            .x = 100,
            .y = 100,
            .z = 100
        },
        .vec8 = {
            .x = 100,
            .y = -100,
            .z = 100
        }
    },
    .rendered_pos = {
        .vec1 = {
            .x = -200,
            .y = -10
        },
        .vec2 = {
            .x = -200,
            .y = 10
        },
        .vec3 = {
            .x = 200,
            .y = 10
        },
        .vec4 = {
            .x = 200,
            .y = -10
        },
        .vec5 = {
            .x = -200,
            .y = -10
        },
        .vec6 = {
            .x = -200,
            .y = 10
        },
        .vec7 = {
            .x = 200,
            .y = 10
        },
        .vec8 = {
            .x = 200,
            .y = -10
        }
    }
};

void print_vector(vec3F vector) {
    printf(" %f %f %f ", vector.x, vector.y, vector.z);
}
void print_cube(mat3x8F cube) {
    print_vector(cube.vec1);
    print_vector(cube.vec2);
    print_vector(cube.vec3);
    print_vector(cube.vec4);
    print_vector(cube.vec5);
    print_vector(cube.vec6);
    print_vector(cube.vec7);
    print_vector(cube.vec8);

}

int render_map(type render_type, float value) {
    int return_value = -1;
    switch(render_type) {
    case TYPE_X:
        return_value = value + WINDOW_WIDTH / 2;
        break;
    case TYPE_Y:
        return_value = value + WINDOW_HEIGHT / 2;
        break;
    default:
        break;
    }
    return return_value;
}

void perspective_projection_vector(vec3F game_vector, vec2F* projected_vector) {
    // if(game_vector.z != 0) {
        projected_vector -> x = game_vector.x / game_vector.z;
        projected_vector -> y = game_vector.y / game_vector.z;
    // }
}

void weak_perspective_projection_vector(vec3F game_vector, vec2F* projected_vector) {
    // if(game_vector.z + FOCAL_LENGTH != 0) {
        projected_vector -> x = (game_vector.x * FOCAL_LENGTH) / ((game_vector.z - GAME_DEPTH / 2) + FOCAL_LENGTH);
        projected_vector -> y = (game_vector.y * FOCAL_LENGTH) / ((game_vector.z - GAME_DEPTH / 2) + FOCAL_LENGTH);
    // }
}

void perspective_projection_matrix(mat3x8F game_matrix, mat2x8F* projected_matrix) {
    perspective_projection_vector(game_matrix.vec1, &projected_matrix -> vec1);
    perspective_projection_vector(game_matrix.vec2, &projected_matrix -> vec2);
    perspective_projection_vector(game_matrix.vec3, &projected_matrix -> vec3);
    perspective_projection_vector(game_matrix.vec4, &projected_matrix -> vec4);
    perspective_projection_vector(game_matrix.vec5, &projected_matrix -> vec5);
    perspective_projection_vector(game_matrix.vec6, &projected_matrix -> vec6);
    perspective_projection_vector(game_matrix.vec7, &projected_matrix -> vec7);
    perspective_projection_vector(game_matrix.vec8, &projected_matrix -> vec8);
}

void weak_perspective_projection_matrix(mat3x8F game_matrix, mat2x8F* projected_matrix) {
    weak_perspective_projection_vector(game_matrix.vec1, &projected_matrix -> vec1);
    weak_perspective_projection_vector(game_matrix.vec2, &projected_matrix -> vec2);
    weak_perspective_projection_vector(game_matrix.vec3, &projected_matrix -> vec3);
    weak_perspective_projection_vector(game_matrix.vec4, &projected_matrix -> vec4);
    weak_perspective_projection_vector(game_matrix.vec5, &projected_matrix -> vec5);
    weak_perspective_projection_vector(game_matrix.vec6, &projected_matrix -> vec6);
    weak_perspective_projection_vector(game_matrix.vec7, &projected_matrix -> vec7);
    weak_perspective_projection_vector(game_matrix.vec8, &projected_matrix -> vec8);
}

void multiply_vector(vec3F* vector, mat3x3F multiply_matrix) {
    vec3F copy_vector = *vector;
    vector -> x = multiply_matrix.vec1.x * copy_vector.x +
                  multiply_matrix.vec2.x * copy_vector.y +
                  multiply_matrix.vec3.x * copy_vector.z;
    vector -> y = multiply_matrix.vec1.y * copy_vector.x +
                  multiply_matrix.vec2.y * copy_vector.y +
                  multiply_matrix.vec3.y * copy_vector.z;
    vector -> z = multiply_matrix.vec1.z * copy_vector.x +
                  multiply_matrix.vec2.z * copy_vector.y +
                  multiply_matrix.vec3.z * copy_vector.z;
}

void multiply_matrix(mat3x8F* matrix, mat3x3F multiply_matrix) {
    multiply_vector(&matrix -> vec1, multiply_matrix);
    multiply_vector(&matrix -> vec2, multiply_matrix);
    multiply_vector(&matrix -> vec3, multiply_matrix);
    multiply_vector(&matrix -> vec4, multiply_matrix);
    multiply_vector(&matrix -> vec5, multiply_matrix);
    multiply_vector(&matrix -> vec6, multiply_matrix); 
    multiply_vector(&matrix -> vec7, multiply_matrix);
    multiply_vector(&matrix -> vec8, multiply_matrix);
}

void rotate_matrix(mat3x8F* matrix, type rotation_type, float degrees) {
    float radians = PI * degrees / 180;
    mat3x3F rotation_matrix = {
        .vec1 = {
            .x = 1,
            0,
            0
        },
        .vec2 = {
            0,
            1,
            0
        },
        .vec3 = {
            0,
            0,
            1
        }
    };
    switch(rotation_type) {
    case TYPE_X:
        rotation_matrix.vec1.x = 1;
        rotation_matrix.vec1.y = 0;
        rotation_matrix.vec1.z = 0;

        rotation_matrix.vec2.x = 0;
        rotation_matrix.vec2.y = cos(radians);
        rotation_matrix.vec2.z = -sin(radians);

        rotation_matrix.vec3.x = 0;
        rotation_matrix.vec3.y = sin(radians);
        rotation_matrix.vec3.z = cos(radians);
        break;
    case TYPE_Y:
        rotation_matrix.vec1.x = cos(radians);
        rotation_matrix.vec1.y = 0;
        rotation_matrix.vec1.z = -sin(radians);

        rotation_matrix.vec2.x = 0;
        rotation_matrix.vec2.y = 1;
        rotation_matrix.vec2.z = 0;

        rotation_matrix.vec3.x = sin(radians);
        rotation_matrix.vec3.y = 0;
        rotation_matrix.vec3.z = cos(radians);         
        break;
    case TYPE_Z:
        rotation_matrix.vec1.x = cos(radians);
        rotation_matrix.vec1.y = sin(radians);
        rotation_matrix.vec1.z = 0;

        rotation_matrix.vec2.x = -sin(radians);
        rotation_matrix.vec2.y = cos(radians);
        rotation_matrix.vec2.z = 0;

        rotation_matrix.vec3.x = 0;
        rotation_matrix.vec3.y = 0;
        rotation_matrix.vec3.z = 1;
        break;
    }
    multiply_matrix(matrix, rotation_matrix);
}

int initialize_window(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return FALSE;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if(!window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }

    return TRUE;
}

void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_QUIT:
            game_is_running = FALSE;
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    game_is_running = FALSE;
                    break;
            }
            break;
    }   
}

void setup() {
    rotate_matrix(&shown_cube.game_pos, TYPE_Y, 20);
    weak_perspective_projection_matrix(shown_cube.game_pos, &shown_cube.rendered_pos);
}

void update() {
    rotate_matrix(&shown_cube.game_pos, TYPE_Y, 0.1);
    weak_perspective_projection_matrix(shown_cube.game_pos, &shown_cube.rendered_pos);
}

void draw_line(vec2F start_point, vec2F end_point) {
    SDL_RenderDrawLineF(
        renderer, 
        render_map(TYPE_X, start_point.x),
        render_map(TYPE_Y, start_point.y),
        render_map(TYPE_X, end_point.x),
        render_map(TYPE_Y, end_point.y)
    );
}

void render_cube() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    draw_line(shown_cube.rendered_pos.vec1, shown_cube.rendered_pos.vec2);
    draw_line(shown_cube.rendered_pos.vec1, shown_cube.rendered_pos.vec4);
    draw_line(shown_cube.rendered_pos.vec1, shown_cube.rendered_pos.vec5);
    draw_line(shown_cube.rendered_pos.vec6, shown_cube.rendered_pos.vec2);
    draw_line(shown_cube.rendered_pos.vec6, shown_cube.rendered_pos.vec5);
    draw_line(shown_cube.rendered_pos.vec6, shown_cube.rendered_pos.vec7);
    draw_line(shown_cube.rendered_pos.vec3, shown_cube.rendered_pos.vec2);
    draw_line(shown_cube.rendered_pos.vec3, shown_cube.rendered_pos.vec4);
    draw_line(shown_cube.rendered_pos.vec3, shown_cube.rendered_pos.vec7);
    draw_line(shown_cube.rendered_pos.vec8, shown_cube.rendered_pos.vec4);
    draw_line(shown_cube.rendered_pos.vec8, shown_cube.rendered_pos.vec5);
    draw_line(shown_cube.rendered_pos.vec8, shown_cube.rendered_pos.vec7);
    SDL_RenderPresent(renderer);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_cube();
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main (int argc, char* argv[]) {
    // printf("What is the direction you would like the cube to spin (0 - X, 1 - Y, 2 - Z): ");
    game_is_running = initialize_window();

    setup();
    while(game_is_running) {
        process_input();
        update();
        render();
    }
    print_cube(shown_cube.game_pos);
    destroy_window();

    return EXIT_SUCCESS;
}
