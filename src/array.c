#include <bfdev/bfdev.h>
#include <bfdev/array.h>

/* TODO: let lib user can call this function*/
static inline int
bfdev_array_init(bfdev_array_t *a, allocator_t *allor, unsigned n, size_t size)
{
    if (allor == NULL) {
        allor = gallocator_create();
    }

    a->nelts = 0;
    a->size = size;
    a->nalloc = n;
    a->allocator = allor;

    a->elts = allor->malloc(size * n, allor->pdata);
    if (unlikely(a->elts == NULL)) {
        return BFDEV_ENOMEM;
    }

    return BFDEV_ENOERR;
}

bfdev_array_t *
bfdev_array_create(allocator_t *allor, unsigned n, size_t size)
{
    bfdev_array_t *a;
    if (allor == NULL) {
        allor = gallocator_create();
    }

    a = allor->malloc(sizeof(bfdev_array_t), allor->pdata);
    if (unlikely(a == NULL)) {
        return NULL;
    }

    if (bfdev_array_init(a, allor, n, size) != BFDEV_ENOERR) {
        return NULL;
    }

    return a;
}

void bfdev_array_destroy(bfdev_array_t *a)
{
    allocator_t *allor;
    allor = a->allocator;

    allor->free(a->elts, allor->pdata);
    allor->free(a, allor->pdata);
}

void *bfdev_array_push(bfdev_array_t *a)
{
    void *elt, *new;
    unsigned nalloc;
    allocator_t *allor;

    allor = a->allocator;

    if (a->nelts == a->nalloc) {

        nalloc = a->nalloc * 2;
        new = allor->realloc(a->elts, nalloc * a->size, allor->pdata);
        if (unlikely(new == NULL)) {
            return NULL;
        }

        a->elts = new;
        a->nalloc = nalloc;
    }

    elt = (char *) a->elts + a->size * a->nelts;
    a->nelts++;

    return elt;
}

void *bfdev_array_push_n(bfdev_array_t *a, unsigned n)
{
    void *elt, *new;
    unsigned   nalloc;
    allocator_t *allor;

    allor = a->allocator;

    if (a->nelts == a->nalloc) {

        nalloc = 2 * ((n >= a->nalloc) ? n : a->nalloc);
        new = allor->realloc(a->elts, nalloc * a->size, allor->pdata);
        if (unlikely(new == NULL)) {
            return NULL;
        }

        a->elts = new;
        a->nalloc = nalloc;
    }


    elt = (char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    return elt;
}
