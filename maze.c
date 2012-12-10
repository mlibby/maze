#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEIGHT 36
#define WIDTH 28

enum {
    TOP = 1,
    RIGHT = 2,
    BOTTOM = 4,
    LEFT = 8,
    VISITED = 16
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

int rnd(int min, int max) {
    return ((double)rand() / ((double)RAND_MAX + 1.0)) * (max - min + 1) + min;
}

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

node *next_node(maze *m, int cx, int cy)
{
    node *n = NULL;
    int possibles[4] = { 0, 0, 0, 0 };
    int count = 0;
    int current_cell = 0;
    int pc = 0;
    int nx, ny;
    int back;

    printf("next node from %d, %d = %d\n", cx, cy, m->cells[cx][cy]);

    if((cx > 0) && (m->cells[cx - 1][cy] & VISITED) == 0) {
	possibles[count] = LEFT;
	count++;
    }

    if((cy > 0) && (m->cells[cx][cy - 1] & VISITED) == 0) {
	possibles[count] = TOP;
	count++;
    }

    if((cx < m->width - 1) && (m->cells[cx + 1][cy] & VISITED) == 0) {
	possibles[count] = RIGHT;
	count++;
    }

    if((cy < m->height - 1) && (m->cells[cx][cy + 1] & VISITED) == 0) {
	possibles[count] = BOTTOM;
	count++;
    }

    printf("Possibles %d, %d, %d, %d\n", possibles[0], possibles[1], possibles[2], possibles[3]);

    if(count > 0)    {
	pc = rnd(0, count - 1);
	printf("tunneling %d\n", possibles[pc]);

	switch(possibles[pc]){
	case LEFT:
	    nx = cx - 1;
	    ny = cy;
	    back = RIGHT;
	    break;
	case RIGHT:
	    nx = cx + 1;
	    ny = cy;
	    back = LEFT;
	    break;
	case BOTTOM:
	    nx = cx;
	    ny = cy + 1;
	    back = TOP;
	    break;
	case TOP:
	    nx = cx;
	    ny = cy - 1;
	    back = BOTTOM;
	    break;
	}

	n = new_node(nx, ny, NULL);
	m->cells[cx][cy] = m->cells[cx][cy] & ~possibles[pc];
	m->cells[n->x][n->y] = m->cells[n->x][n->y] & ~back;
	m->cells[n->x][n->y] = m->cells[n->x][n->y] | VISITED;
	
	printf("new node for %d, %d\n", n->x, n->y);
    } else {
	return NULL;
    }
    
/*     if(x < m->width - 1) {
	printf("checking right (%d & %d = %d)\n", m->cells[x][y], RIGHT, m->cells[x][y] & RIGHT);
	if((m->cells[x][y] & RIGHT) != 0) {
	    printf("tunneling right\n");
	    n = new_node(x + 1, y, NULL);
	    
	    m->cells[x][y] = m->cells[x][y] & ~RIGHT;
	    m->cells[n->x][n->y] = m->cells[n->x][n->y] & ~LEFT;
	    
	    printf("new node for %d, %d\n", n->x, n->y);
	}
    }
*/
 
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
    m->cells[0][0] = m->cells[0][0] | VISITED;

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

    srand(time(0));

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
