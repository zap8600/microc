int syscall_NR;
int syscall_return;
int syscall_arg0;
int syscall_arg1;
int syscall_arg2;
int syscall_arg3;
int syscall_arg4;
int syscall_arg5;

void syscall() {
    asm 83;
    asm 85;

    syscall_arg5 = syscall_arg5;
    asm 137; asm 197;
    syscall_arg4 = syscall_arg4;
    asm 137; asm 199;
    syscall_arg3 = syscall_arg3;
    asm 137; asm 198;
    syscall_arg2 = syscall_arg2;
    asm 137; asm 194;
    syscall_arg1 = syscall_arg1;
    asm 137; asm 193;
    syscall_arg0 = syscall_arg0;
    asm 80;
    syscall_NR = syscall_NR;

    asm 91;
    asm 205; asm 128;

    asm 93;
    asm 91;

    asm 129; asm 195; asm 4; asm 0; asm 0; asm 0;
    asm 137; asm 3;
    asm 129; asm 235; asm 4; asm 0; asm 0; asm 0;
}

int exit_errorcode;
void exit() {
    syscall_NR = 1;
    syscall_arg0 = exit_errorcode;
    syscall();
}

int read_fd;
int read_buf;
int read_count;
int read_return;
void read() {
    syscall_NR = 3;
    syscall_arg0 = read_fd;
    syscall_arg1 = read_buf;
    syscall_arg2 = read_count;
    syscall();
    read_return = syscall_return;
}

int write_fd;
int write_buf;
int write_count;
int write_return;
void write() {
    syscall_NR = 4;
    syscall_arg0 = write_fd;
    syscall_arg1 = write_buf;
    syscall_arg2 = write_count;
    syscall();
    write_return = syscall_return;
}

int open_filename;
int open_flags;
int open_mode;
int open_return;
void open() {
    syscall_NR = 5;
    syscall_arg0 = open_filename;
    syscall_arg1 = open_flags;
    syscall_arg2 = open_mode;
    syscall();
    open_return = syscall_return;
}

int close_fd;
int close_return;
void close() {
    syscall_NR = 6;
    syscall_arg0 = close_fd;
    syscall();
    close_return = syscall_return;
}

int brk_break;
int brk_return;
void brk() {
    syscall_NR = 45;
    syscall_arg0 = brk_break;
    syscall();
    brk_return = syscall_return;
}

int breakstart;
int breakcurrent;

int symboltable;
int symbolamount;

int c;
int out;

int code1;
int code2;
int code3;

int out1;
int out2;

int tmp1;
void main() {
    code1 = 1634545454;
    code2 = 1663987305;
    code3 = 0;

    out1 = 1970220846;
    out2 = 116;

    brk_break = 0;
    brk();
    breakstart = brk_return;

    breakcurrent = breakstart + 8;

    brk_break = breakcurrent;
    brk();

    *(int*) breakstart = 3;

    tmp1 = *(int*) breakstart;

    breakcurrent = breakstart;
    brk_break = breakcurrent;
    brk();
}

void _start() {
    asm 187;
    asm 0;
    asm 0;
    asm 0;
    asm 0;
    
    asm 85;
    asm 137;
    asm 229;
    asm 131;
    asm 236;
    asm 16;

    main();
}