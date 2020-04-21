
volatile sig_atomic_t sig_child_flag;
volatile sig_atomic_t sig_int_flag;
volatile sig_atomic_t enabler;

sigset_t mask_all, mask_one, prev_one;

void sigchld_handler(int sig);
void sigint_handler(int sig);

int handler();