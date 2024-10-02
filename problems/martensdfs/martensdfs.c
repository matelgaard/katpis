#include <stdlib.h>
#include <stdio.h>

int num_verts;
int num_edges;

typedef struct {
    int size;
    int length;
    int *verts;
} verts_list;

verts_list *edges;
verts_list traversal;
int *edge_hash;
int hash_size = 16777216;

int *visited;

int hash(int k)
{
    return ((123456UL * 2654435769UL * k) & 0xFFFFFFFFUL) >> 8;
}

void hash_insert(int k)
{
    int q = hash(k);
    while (q < hash_size)
    {
        if (edge_hash[q] == 0)
        {
            edge_hash[q] = k;
            return;
        }
        q++;
    }
}

int hash_get(int k)
{
    int q = hash(k);
    while (edge_hash[q] && q < hash_size)
    {
        if (edge_hash[q] == k)
        {
            return 1;
        }
        q++;
    }
    return 0;
}

void add_vert(verts_list *list, int vert)
{
    if (list->size == 0)
    {
        list->size = 1;
        list->verts = malloc(list->size * sizeof(*list->verts));
    }
    else if (list->length == list->size)
    {
        list->size *= 2;
        list->verts = realloc(list->verts, list->size * sizeof(*list->verts));
    }

    list->verts[list->length] = vert;
    list->length++;
}

void read_input()
{
    // Read number of verts and edges
    scanf("%i %i", &num_verts, &num_edges);

    edges = calloc(num_verts, sizeof(*edges));
    edge_hash = calloc(hash_size, sizeof(*edge_hash));

    // Read edges
    for (int e = 0; e < num_edges; e++)
    {
        int u, v;
        scanf("%i %i\n", &u, &v);

        add_vert(&edges[u], v);
        add_vert(&edges[v], u);

        hash_insert(u * num_verts + v);
    }

    traversal.size = num_verts;
    traversal.length = 0;
    traversal.verts = calloc(num_verts, sizeof(traversal));

    // Read DFS traversal
    int d;
    char ch;
    while (scanf("%i%c", &d, &ch) > 0)
    {
        add_vert(&traversal, d);
        if (ch == '\n')
            break;
    }
}

void free_input()
{
    for (int i = 0; i < num_verts; i++)
    {
        if (edges[i].size > 0)
            free(edges[i].verts);
    }

    free(edges);
    free(traversal.verts);
}

int check_parentheses(int i)
{
    int j = i + 1;
    int u = traversal.verts[i];
    if (u >= num_verts)
        return traversal.length + 1;

    if (visited[u])
        return traversal.length + 1;

    visited[u] = 1;
    while (j < traversal.length)
    {
        int v = traversal.verts[j];
        if (v >= num_verts)
            return traversal.length + 1;

        int k;
        if (u > v)
        {
            k = traversal.verts[j] * num_verts + traversal.verts[i];
        }
        else
        {
            k = u * num_verts + v;
        }

        if (hash_get(k))
        {
            j = check_parentheses(j);
        }
        else
        {
            for (int l = 0; l < edges[u].length; l++)
            {
                if (!visited[edges[u].verts[l]])
                {
                    return traversal.length + 1;
                }
            }
            return j;
        }
    }

    return j;
}

int verify_dfs(void) {
    if (traversal.length != num_verts)
        return 0;

    visited = calloc(num_verts, sizeof(*visited));

    int p = check_parentheses(0);

    free(visited);

    return p == traversal.length;
}

int main()
{
    read_input();

    int valid = verify_dfs();

    if (valid)
    {
        printf("YES\n");
    }
    else
    {
        printf("NO\n");
    }

    free_input();

    return 0;
}
