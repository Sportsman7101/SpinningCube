#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 600

#define GAME_DEPTH 500

#define FALSE 0
#define TRUE 1

#define PI 3.14159

#define FOCAL_LENGTH 50000

typedef struct {
    float x;
    float y;
} vec2F;

typedef struct {
    float x;
    float y;
    float z;
} vec3F;

typedef struct {
    vec2F vec1;
    vec2F vec2;
} mat2x2F;

typedef struct {
    vec2F vec1;
    vec2F vec2;
    vec2F vec3;
    vec2F vec4;    
    vec2F vec5;
    vec2F vec6;
    vec2F vec7;
    vec2F vec8;
} mat2x8F;

typedef struct {
    vec3F vec1;
    vec3F vec2;
    vec3F vec3;
} mat3x3F;

typedef struct {
    vec3F vec1;
    vec3F vec2;
    vec3F vec3;
    vec3F vec4;
    vec3F vec5;
    vec3F vec6;
    vec3F vec7;
    vec3F vec8;
} mat3x8F;

typedef enum {
    TYPE_X = 0,
    TYPE_Y,
    TYPE_Z,
} type;