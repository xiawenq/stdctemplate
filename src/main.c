#include <stdio.h>
#include <elog.h>
#include <unistd.h>
#include <argtable3.h>
#include <pthread.h>

static void test_elog(void);
static void mymain(int n, int e, int E, const char** strings, int nstrings);

static void log_init() {
    setbuf(stdout, NULL);

    elog_init();

    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~ELOG_FMT_FUNC);
#ifdef ELOG_COLOR_ENABLE
    elog_set_text_color_enabled(true);
#endif
    elog_start();
}

static int arg_process_init(int argc, char **argv) {
    /* Define the allowable command line options, collecting them in argtable[] */
    struct arg_lit *n     = arg_lit0("n", NULL,         "do not output the trailing newline");
    struct arg_lit *e     = arg_lit0("e", NULL,         "enable interpretation of the backslash-escaped characters listed below");
    struct arg_lit *E     = arg_lit0("E", NULL,         "disable interpretation of those sequences in <string>s");
    struct arg_lit *help  = arg_lit0(NULL,"help",       "print this help and exit");
    struct arg_lit *vers  = arg_lit0(NULL,"version",    "print version information and exit");
    struct arg_str *strs  = arg_strn(NULL,NULL,"STRING",0,argc+2,NULL);
    struct arg_end *end   = arg_end(20);
    void* argtable[] = {n,e,E,help,vers,strs,end};
    const char* progname = "echo";
    int exitcode=0;
    int nerrors;

    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
    {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n",progname);
        exitcode=1;
        goto exit;
    }

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout,argtable,"\n");
        printf("Echo the STRINGs to standard output.\n\n");
        arg_print_glossary(stdout,argtable,"  %-10s %s\n");
        printf("\nWithout -E, the following sequences are recognized and interpolated:\n\n"
               "  \\NNN   the character whose ASCII code is NNN (octal)\n"
               "  \\\\     backslash\n"
               "  \\a     alert (BEL)\n"
               "  \\b     backspace\n"
               "  \\c     suppress trailing newline\n"
               "  \\f     form feed\n"
               "  \\n     new line\n"
               "  \\r     carriage return\n"
               "  \\t     horizontal tab\n"
               "  \\v     vertical tab\n\n"
               "Report bugs to <foo@bar>.\n");
        exitcode=0;
        goto exit;
    }

    /* special case: '--version' takes precedence error reporting */
    if (vers->count > 0)
    {
        printf("'%s' example program for the \"argtable\" command line argument parser.\n",progname);
        printf("September 2003, Stewart Heitmann\n");
        exitcode=0;
        goto exit;
    }

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,progname);
        printf("Try '%s --help' for more information.\n",progname);
        exitcode=1;
        goto exit;
    }

    /* Command line parsing is complete, do the main processing */
    mymain(n->count, e->count, E->count, strs->sval, strs->count);

    exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));

    return exitcode;
}

int main(int argc, char **argv) {
    printf("Hello, World! pid:%04d, tid:%04ld\n", getpid(), pthread_self());

    log_init();

    test_elog();
#if 1
    int exitcode = arg_process_init(argc, argv);

    sleep(1);
    return exitcode;
#else
    return 0;
#endif
}

void test_elog(void) {
    uint8_t buf[256]= {0};
    int i = 0;

    for (i = 0; i < sizeof(buf); i++)
    {
        buf[i] = i;
    }
    do {
        /* test log output for all level */
        log_a("Hello EasyLogger!");
        log_e("Hello EasyLogger!");
        log_w("Hello EasyLogger!");
        log_i("Hello EasyLogger!");
        log_d("Hello EasyLogger!");
        log_v("Hello EasyLogger!");
        elog_raw("Hello EasyLogger!\n");
        elog_hexdump("test", 16, buf, sizeof(buf));
//        sleep(1);
    } while(false);
}

/* Here we only approximate the echo functionality */
void mymain(int n, int e, int E, const char** strings, int nstrings)
{
    int j;

    log_d("option -n = %s", ((n)?"YES":"NO"));
    log_d("option -e = %s", ((e)?"YES":"NO"));
    log_d("option -E = %s", ((E)?"YES":"NO"));
    for (j=0; j<nstrings; j++)
        elog_raw("%s ", strings[j]);
    elog_raw("\n");
}