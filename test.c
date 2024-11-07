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

int msg;
void main() {
    msg = 270625096;

    syscall_NR = 4;
    syscall_arg0 = 1;
    syscall_arg1 = & msg;
    syscall_arg2 = 4;
    syscall();
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

    syscall_NR = 1;
    syscall_arg0 = 0;
    syscall();
}