/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2022 John Sanpe <sanpeqf@gmail.com>
 */

#include <bfdev.h>
#include <bfdev/minmax.h>
#include <bfdev/json.h>
#include <export.h>

#define PASER_TEXT_DEF 64
#define PASER_NODE_DEPTH 32
#define PASER_STATE_DEPTH 32

enum json_state {
    JSON_STATE_NULL,
    JSON_STATE_ESC,
    JSON_STATE_BODY,
    JSON_STATE_COLON,
    JSON_STATE_WAIT,

    /* Is struct */
    JSON_STATE_ARRAY,
    JSON_STATE_OBJECT,

    /* Is record */
    JSON_STATE_NAME,
    JSON_STATE_STRING,
    JSON_STATE_NUMBER,
    JSON_STATE_OTHER,
};

struct json_transition {
    enum json_state form, to;
    char code, ecode;
    int nstack, sstack;
    bool cross;
};

static const struct
json_transition transition_table[] = {
    {JSON_STATE_OBJECT,   JSON_STATE_NAME,     '"',   '"',  + 1,  + 1,  false},
    {JSON_STATE_NAME,     JSON_STATE_COLON,    '"',   '"',    0,    0,  false},
    {JSON_STATE_COLON,    JSON_STATE_BODY,     ':',   ':',    0,    0,  false},

    {JSON_STATE_NAME,     JSON_STATE_ESC,     '\\',  '\\',    0,  + 1,  false},
    {JSON_STATE_STRING,   JSON_STATE_ESC,     '\\',  '\\',    0,  + 1,  false},
    {JSON_STATE_ESC,      JSON_STATE_NULL,    '\0',  '\0',    0,  - 1,   true},

    {JSON_STATE_BODY,     JSON_STATE_ARRAY,    '[',   '[',    0,    0,  false},
    {JSON_STATE_BODY,     JSON_STATE_OBJECT,   '{',   '{',    0,    0,  false},
    {JSON_STATE_BODY,     JSON_STATE_NUMBER,   '0',   '9',    0,    0,   true},
    {JSON_STATE_BODY,     JSON_STATE_STRING,   '"',   '"',    0,    0,  false},
    {JSON_STATE_BODY,     JSON_STATE_OTHER,   '\0',  '\0',    0,    0,   true},

    {JSON_STATE_ARRAY,    JSON_STATE_ARRAY,    '[',   '[',  + 1,  + 1,  false},
    {JSON_STATE_ARRAY,    JSON_STATE_OBJECT,   '{',   '{',  + 1,  + 1,  false},
    {JSON_STATE_ARRAY,    JSON_STATE_NUMBER,   '0',   '9',  + 1,  + 1,   true},
    {JSON_STATE_ARRAY,    JSON_STATE_STRING,   '"',   '"',  + 1,  + 1,  false},
    {JSON_STATE_ARRAY,    JSON_STATE_OTHER,   '\0',  '\0',  + 1,  + 1,   true},

    {JSON_STATE_ARRAY,    JSON_STATE_WAIT,     ']',   ']',  - 1,    0,  false},
    {JSON_STATE_OBJECT,   JSON_STATE_WAIT,     '}',   '}',  - 1,    0,  false},
    {JSON_STATE_STRING,   JSON_STATE_WAIT,     '"',   '"',  - 1,    0,  false},

    {JSON_STATE_NUMBER,   JSON_STATE_NULL,     ',',   ',',  - 1,  - 1,  false},
    {JSON_STATE_OTHER,    JSON_STATE_NULL,     ',',   ',',  - 1,  - 1,  false},
    {JSON_STATE_WAIT,     JSON_STATE_NULL,     ',',   ',',    0,  - 1,  false},

    {JSON_STATE_NUMBER,   JSON_STATE_WAIT,     ']',   ']',  - 2,  - 1,  false},
    {JSON_STATE_OTHER,    JSON_STATE_WAIT,     ']',   ']',  - 2,  - 1,  false},
    {JSON_STATE_WAIT,     JSON_STATE_WAIT,     ']',   ']',  - 1,  - 1,  false},
    {JSON_STATE_NUMBER,   JSON_STATE_WAIT,     '}',   '}',  - 2,  - 1,  false},
    {JSON_STATE_OTHER,    JSON_STATE_WAIT,     '}',   '}',  - 2,  - 1,  false},
    {JSON_STATE_WAIT,     JSON_STATE_WAIT,     '}',   '}',  - 1,  - 1,  false},
};

static inline bool
is_struct(enum json_state state)
{
    return JSON_STATE_ARRAY <= state && state <= JSON_STATE_OBJECT;
}

static inline bool
is_record(enum json_state state)
{
    return JSON_STATE_NAME <= state && state <= JSON_STATE_OTHER;
}

static inline const char *
skip_lack(const char *string)
{
    while (*string && (!isprint(*string) || *string == ' '))
        string++;
    return string;
}

static inline const struct json_transition *
next_state(const char code, enum json_state state)
{
    const struct json_transition *major, *minor = NULL;
    unsigned int count;

    for (count = 0; count < ARRAY_SIZE(transition_table); ++count) {
        major = &transition_table[count];
        if (major->form == state) {
            if (major->code <= code && code <= major->ecode) {
                count = 0;
                break;
            } else if (!major->code)
                minor = major;
        }
    }

    if (count && minor)
        major = minor;
    else if (count)
        major = NULL;

    return major;
}

export int
bfdev_json_decode(const struct bfdev_alloc *alloc, const char *buff,
                  struct bfdev_json_node **root)
{
    enum json_state cstate = JSON_STATE_ARRAY, nstate = JSON_STATE_ARRAY;
    struct bfdev_json_node *nstack[PASER_NODE_DEPTH], *node = NULL;
    enum json_state sstack[PASER_STATE_DEPTH];
    unsigned int tpos = 0, tsize = PASER_TEXT_DEF;
    int cspos = 0, nspos = 0, cnpos = -1, nnpos = -1;
    char *tbuff, *nblock;
    int retval = -BFDEV_ENOERR;
    const char *walk;
    bool cross = false;

    tbuff = bfdev_malloc(alloc, tsize);
    if (!tbuff)
        return -BFDEV_ENOMEM;

    for (walk = buff; *walk; ++walk) {
        const struct json_transition *tran;

        if (!is_record(cstate)) {
            walk = skip_lack(walk);
            if (!*walk)
                break;
        }

        tran = next_state(*walk, cstate);
        if (tran) {
            nnpos += tran->nstack;
            nspos += tran->sstack;
            nstate = tran->to;
            cross = tran->cross;

            if (nnpos >= PASER_NODE_DEPTH ||
                nspos >= PASER_STATE_DEPTH) {
                retval = -BFDEV_EOVERFLOW;
                goto error;
            }
        }

        if (nspos > cspos && cstate != JSON_STATE_NULL)
            sstack[cspos] = cstate;
        else if (nspos < cspos && nstate == JSON_STATE_NULL)
            nstate = sstack[nspos];

        if (nnpos < cnpos && nnpos < 0)
            break;
        else if (nnpos > cnpos) {
            struct bfdev_json_node *parent = node;

            node = bfdev_zalloc(alloc, sizeof(*node));
            if (!node) {
                retval = -BFDEV_ENOMEM;
                goto error;
            }
            if (cnpos >= 0) {
                nstack[cnpos] = parent;
                bfdev_list_add_prev(&nstack[cnpos]->child, &node->sibling);
            }
            node->parent = parent;
            bfdev_list_head_init(&node->child);
        }

        if (is_struct(nstate)) {
            switch (*walk) {
                case '[':
                    bfdev_json_set_array(node);
                    break;

                case '{':
                    bfdev_json_set_object(node);
                    break;

                default:
                    break;
            }
        }

        if (nstate != JSON_STATE_ESC) {
            if (is_record(cstate) && !is_record(nstate)) {
                tbuff[tpos] = '\0';
                switch (cstate) {
                    case JSON_STATE_NAME:
                        node->name = bfdev_strdup(alloc, tbuff);
                        if (!node->name) {
                            retval = -BFDEV_ENOMEM;
                            goto error;
                        }
                        break;

                    case JSON_STATE_STRING:
                        node->string = bfdev_strdup(alloc, tbuff);
                        if (!node->string) {
                            retval = -BFDEV_ENOMEM;
                            goto error;
                        }
                        bfdev_json_set_string(node);
                        break;

                    case JSON_STATE_NUMBER:
                        node->number = atol(tbuff);
                        bfdev_json_set_number(node);
                        break;

                    case JSON_STATE_OTHER:
                        if (!strcmp(tbuff, "null"))
                            bfdev_json_set_null(node);
                        else if (!strcmp(tbuff, "true"))
                            bfdev_json_set_true(node);
                        else if (!strcmp(tbuff, "false"))
                            bfdev_json_set_false(node);
                        break;

                    default:
                        break;
                }
                tpos = 0;
            } else if (cross || is_record(cstate)) {
                if (unlikely(tpos >= tsize)) {
                    tsize *= 2;
                    nblock = bfdev_realloc(alloc, tbuff, tsize);
                    if (!nblock) {
                        bfdev_free(alloc, tbuff);
                        retval = -BFDEV_ENOMEM;
                        goto error;
                    }
                    tbuff = nblock;
                }
                tbuff[tpos++] = *walk;
                cross = false;
            }
        }

        if (nnpos < cnpos)
            node = nstack[nnpos];

        cnpos = nnpos;
        cspos = nspos;
        cstate = nstate;
    }

error:
    bfdev_free(alloc, tbuff);
    if (cnpos > 0)
        node = *nstack;

    if (retval)
        bfdev_json_release(alloc, node);
    else if (root)
        *root = node;

    return retval;
}

static int
encode_depth(struct bfdev_json_node *parent, char *buff,
             int size, int len, unsigned int depth)
{
    #define json_sprintf(fmt, ...) len += snprintf(buff + len, bfdev_max(0, size - len), fmt, ##__VA_ARGS__)
    struct bfdev_json_node *child;
    unsigned int count;

    if (bfdev_json_test_array(parent))
        json_sprintf("[\n");
    else if (bfdev_json_test_object(parent))
        json_sprintf("{\n");
    else
        return 0;

    bfdev_list_for_each_entry(child, &parent->child, sibling) {
        for (count = 0; count < depth + 1; ++count)
            json_sprintf("\t");
        if (bfdev_json_test_object(parent))
            json_sprintf("%s: ", child->name);
        if (bfdev_json_test_array(child) || bfdev_json_test_object(child))
            len = encode_depth(child, buff, size, len, depth + 1);
        else if (bfdev_json_test_number(child))
            json_sprintf("%ld,\n", child->number);
        else if (bfdev_json_test_string(child))
            json_sprintf("\"%s\",\n", child->string);
        else if (bfdev_json_test_null(child))
            json_sprintf("null,\n");
        else if (bfdev_json_test_true(child))
            json_sprintf("true,\n");
        else /* bfdev_json_test_false(child) */
            json_sprintf("false,\n");
    }

    if (!bfdev_list_check_empty(&parent->child)) {
        len -= 2;
        json_sprintf("\n");
    }

    for (count = 0; count < depth; ++count)
        json_sprintf("\t");
    if (bfdev_json_test_array(parent))
        json_sprintf("],\n");
    else if (bfdev_json_test_object(parent))
        json_sprintf("},\n");

    if (!parent->parent) {
        len -= 2;
        json_sprintf("\n");
    }

    return len;
}

export int
bfdev_json_encode(struct bfdev_json_node *root, char *buff, int size)
{
    return encode_depth(root, buff, size, 0, 0) + 1;
}

export void
bfdev_json_release(const struct bfdev_alloc *alloc, struct bfdev_json_node *root)
{
    struct bfdev_json_node *node, *tmp;

    if (unlikely(!root))
        return;

    bfdev_list_for_each_entry_safe(node, tmp, &root->child, sibling) {
        bfdev_list_del(&node->sibling);
        if (bfdev_json_test_array(node) || bfdev_json_test_object(node)) {
            bfdev_json_release(alloc, node);
            continue;
        }
        if (node->name)
            bfdev_free(alloc, node->name);
        if (bfdev_json_test_string(node))
            bfdev_free(alloc, node->string);
        bfdev_free(alloc, node);
    }

    bfdev_free(alloc, root);
}
