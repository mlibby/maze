#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 8
#define WIDTH 48

enum {
    TOP = 1,
    RIGHT = 2,
    BOTTOM = 4,
    LEFT = 8
};

char *classify(int n)
{
    char *c;
    c = calloc(256, 1);

    if(n & TOP) {
	c = strcat(c, "top ");
    }

    if(n & RIGHT) {
	c = strcat(c, "right ");
    }

    if(n & BOTTOM) {
	c = strcat(c, "bottom ");
    }

    if(n & LEFT) {
	c = strcat(c, "left ");
    }

    return c;
}

int **initialize_maze(int width, int height)
{
    int **maze;
    int x, y;

    maze = malloc(width * sizeof(int*));
    for(x = 0; x < width; x++) {
    	maze[x] = malloc(height * sizeof(int));
    }

    for(y = 0; y < height; y++) {
	for(x = 0; x < width; x++) {
	    maze[x][y] = 0;

	    if(x == 0) {
		maze[x][y] = maze[x][y] | LEFT;
	    }

	    if(x == width - 1) {
		maze[x][y] = maze[x][y] | RIGHT;
	    }

	    if(y == 0) {
		maze[x][y] = maze[x][y] | TOP;
	    }

	    if(y == height - 1) {
		maze[x][y] = maze[x][y] | BOTTOM;
	    }
	}
    }

    return maze;
}

int **build_maze(int width, int height)
{
    int **maze;
    int x, y;

    maze = initialize_maze(width, height);

    for(y = 0; y < height; y++) {
	for(x = 0; x < width; x++) {
	}
    }

    return maze;
}

int main(int argc, char *argv[])
{
    FILE *f;
    int **maze;
    int height,width,x,y;
    char *class;

    height = HEIGHT;
    width = WIDTH;

    printf("building maze\n");
    maze = build_maze(width, height);

    printf("saving maze\n");
    f = fopen("maze.html", "w");
    fprintf(f, "<html><head><title>maze</title><link rel=\"stylesheet\" href=\"css.css\" /></head><body><div class='table'>\n");

    for(y = 0; y < height; y++) {
	fprintf(f, "<div class='row'>");
	for(x = 0; x < width; x++) {
	    class = classify(maze[x][y]);
	    fprintf(f, "<div class='cell %s\'></div>", class);
	    free(class);
	}
	fprintf(f, "</div>\n");
    }

    fprintf(f, "<div style='clear:both'></div></div></body></html>\n");
    fclose(f);

    return 0;
}
