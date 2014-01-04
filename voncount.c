/* voncount: A gcc plugin to report statistics about the code being compiled.
 * Copyright (C) 2014  Matt Davis (enferex)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <gcc-plugin.h>
#include <coretypes.h>
#include <tree.h>
#include <tree-flow.h>
#include <function.h>
#include <cgraph.h>
#include <gimple.h>

#define TAG "voncount"
#define PR(...) printf("-- " __VA_ARGS__)

/* This is required for the plugin to actually function */
int plugin_is_GPL_compatible;

/* Keep track of the count of source lines */
static int n_total_lines;

/* Count lines in the provided func decl */
static int count_lines(tree decl)
{
    int count, cur, prev;
    basic_block bb;
    gimple_stmt_iterator gsi;
    struct function *func;

    if (TREE_CODE(decl) != FUNCTION_DECL)
      return 0;
   
    if (!(func = DECL_STRUCT_FUNCTION(decl)))
      return 0;

    prev = -1;
    count = 0;

    FOR_EACH_BB_FN(bb, func)
      for (gsi=gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi))
      {
          cur = gimple_lineno(gsi_stmt(gsi));
          if (cur != prev)
            ++count;
          prev = cur;
      }

    if (prev == -1)
      return 0;

    /* Update global */
    --count;
    n_total_lines += count;
    return count;
}

/* Display function stats */
static void show_funcs(void)
{
    int lines;
    tree decl;
    struct cgraph_node *node;

    PR("Functions:\n");

    FOR_EACH_FUNCTION(node)
    {
        decl = node->symbol.decl;
        lines = count_lines(decl);
        PR("\t%s(), File: %s, Line: %d",
               get_name(decl),
               DECL_SOURCE_FILE(decl),
               DECL_SOURCE_LINE(decl));

        if (lines > 0)
          printf(", Source lines: %d", lines);
        putc('\n', stdout);
    }
}

#if 0
static void show_globals(void)
{
    int n_globals = 0;
    struct varpool_node *node;

    FOR_EACH_VARIABLE(node)
      if (is_global_var(node->symbol.decl))
        ++n_globals;

    PR("Global variables: %d\n", n_globals);
}
#endif

static void show_lines(void)
{
    PR("Total source lines: %d\n", n_total_lines);
}

static void stats_callback(void *gcc_data, void *user_data)
{
    printf("\n-----------------------------------------\n");
    PR("Stats: %s\n", main_input_filename);
    show_funcs();
    //show_globals();
    show_lines();
    printf("-----------------------------------------\n");
}

int plugin_init(
    struct plugin_name_args *plugin_info,
    struct plugin_gcc_version *version)
{
    //register_callback(TAG, PLUGIN_FINISH_UNIT, stats_callback, NULL);
    register_callback(TAG, PLUGIN_ALL_IPA_PASSES_END, stats_callback, NULL);
    return 0;
}
