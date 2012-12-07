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

typedef struct _maze {
    int width;
    int height;
    int **cells;
} maze;

typedef struct _node {
    int x;
    int y;
    struct _node *prev;
} node;

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

maze *new_maze(int width, int height)
{
    maze *m;
    int x, y;

    m = malloc(sizeof(maze));

    m->width = width;
    m->height = height;

    m->cells = malloc(m->width * sizeof(int*));
    for(x = 0; x < m->width; x++) {
    	m->cells[x] = malloc(m->height * sizeof(int));
    }

    for(y = 0; y < m->height; y++) {
	for(x = 0; x < m->width; x++) {
	    m->cells[x][y] = 0 | LEFT | RIGHT | TOP | BOTTOM;
	}
    }

    return m;
}

node *new_node(int x, int y, node *prev)
{
    node *n;
    n = malloc(sizeof(node));
    n->x = x;
    n->y = y;
    n->prev = prev;
    return n;
}

node *next_node(maze *m, int x, int y)
{
    node *n = NULL;
    int possibles[4] = { 0, 0, 0, 0 };
    int count = 0;
    int current_cell = 0;

    printf("next node from %d, %d = %d\n", x, y, m->cells[x][y]);

    current_cell = m->cells[x][y];

    if((x > 0) && (current_cell & LEFT) != 0) {
	possibles[count] = LEFT;
	count++;
    }

    if((y > 0) && (current_cell & TOP) != 0) {
	possibles[count] = TOP;
	count++;
    }

    if((x < m->width - 1) && (current_cell & RIGHT) != 0) {
	possibles[count] = RIGHTT;
	count++;
    }

    if((y > m->height - 1) && (current_cell & BOTTOM) != 0) {
	possibles[count] = BOTTOM;
	count++;
    }

    
    
    if(x < m->width - 1) {
	printf("checking right (%d & %d = %d)\n", m->cells[x][y], RIGHT, m->cells[x][y] & RIGHT);
	if((m->cells[x][y] & RIGHT) != 0) {
	    printf("tunneling right\n");
	    n = new_node(x + 1, y, NULL);
	    
	    m->cells[x][y] = m->cells[x][y] & ~RIGHT;
	    m->cells[n->x][n->y] = m->cells[n->x][n->y] & ~LEFT;
	    
	    printf("new node for %d, %d\n", n->x, n->y);
	}
    }
    
    return n;
}

maze *build_maze(int width, int height)
{
    maze *m;
    int x = 0;
    int y = 0;
    node *tail;
    node *n;

    m = new_maze(width, height);
    tail = new_node(0, 0, NULL);

    while(tail != NULL) {
	if((n = next_node(m, tail->x, tail->y)) == NULL)
	{
	    tail = tail->prev;
	} else {
	    n->prev = tail;
	    tail = n;
	}
    }

    return m;
}

int main(int argc, char *argv[])
{
    FILE *f;
    maze *m;
    int height,width,x,y;
    char *class;

    height = HEIGHT;
    width = WIDTH;

    printf("building maze\n");
    m = build_maze(width, height);

    printf("saving maze\n");
    f = fopen("maze.html", "w");
    fprintf(f, "<html><head><title>maze</title><link rel=\"stylesheet\" href=\"css.css\" /></head><body><div class='table'>\n");

    for(y = 0; y < m->height; y++) {
	fprintf(f, "<div class='row'>");
	for(x = 0; x < m->width; x++) {
	    class = classify(m->cells[x][y]);
	    fprintf(f, "<div class='cell %s\'></div>", class);
	    free(class);
	}
	fprintf(f, "</div>\n");
    }

    fprintf(f, "<div style='clear:both'></div></div></body></html>\n");
    fclose(f);

    return 0;
}
