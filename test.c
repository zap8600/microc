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

int lseek_fd;
int lseek_offset;
int lseek_whence;
int lseek_return;
void lseek() {
    syscall_arg0 = lseek_fd;
    syscall_arg1 = lseek_offset;
    syscall_arg2 = lseek_whence;
    syscall();
    lseek_return = syscall_return;
}

int brk_break;
int brk_return;
void brk() {
    syscall_NR = 45;
    syscall_arg0 = brk_break;
    syscall();
    brk_return = syscall_return;
}



int c;
int out;

int semi;
int ch;
void getch() {
    if( semi == 1 ){
        semi = 0;
        ch = 59;
    }
    if( semi != 1 ){
        read_buf = & ch;
        read_count = 1;
        read();

        if( ch == 59 ){
            semi = 1;
            ch = 0;
        }
    }
}

int toknext_tokisnum;
int toknext_tokiscall;
int toknext_lasttwo;
int toknext_return;
void toknext() {
    toknext_return = 0;
    toknext_lasttwo = 0;
    toknext_tokisnum = 0;
    toknext_tokiscall = 0;

    getch();
    while( ch <= 32 ){
        getch();
    }

    if( ch <= 57 ){
        toknext_tokisnum = 1;
    }

    while( ch > 32 ){
        toknext_lasttwo = ( ( toknext_lasttwo << 8 ) | ch ) & 65535;
        toknext_return = ( 10 * toknext_return ) + ( ( ch - 48 ) & 255 );
        getch();
    }

    if( toknext_lasttwo == 10281 ){
        toknext_tokiscall = 1;
    }
}

int breakstart;
int breakcurrent;

int symboltable;
int symbolamount;
int varamt;

int findsymbol_token;
int findsymbol_i;
int findsymbol_error;
int findsymbol_isub;
int findsymbol_notfound;
int findsymbol_return;
void findsymbol() {
    findsymbol_i = 0;
    findsymbol_notfound = 0;
    findsymbol_isub = 0;
    while( findsymbol_i < symbolamount ){
        symboltable = breakstart + ( findsymbol_i * 8 );
        findsymbol_return = *(int*) symboltable;
        if( findsymbol_return == findsymbol_token ){
            findsymbol_isub = symbolamount - findsymbol_i;
            findsymbol_i = symbolamount - 1;
        }
        findsymbol_i = findsymbol_i + 1;
    }
    findsymbol_i = findsymbol_i - findsymbol_isub;
    if( findsymbol_i >= symbolamount ){
        if( findsymbol_error == 1 ){
            close_fd = c;
            close();
            close_fd = out;
            close();
            brk_break = breakstart;
            brk();
            exit_errorcode = 1;
            exit();
        }
        if( findsymbol_error == 0 ){
            findsymbol_notfound = 1;
            findsymbol_i = 0;
        }
    }

    symboltable = breakstart + ( ( i * 8 ) + 4 );
    findsymbol_return = *(int*) symboltable;
}

int tmp;

//void compileexpr();

void compileunary() {
    if( toknext_return != 4288806029 ){
        if( toknext_return != 4294967288 ){
            if( toknext_return != 4294967286 ){
                if( toknext_tokisnum == 1 ){
                    tmp = 184;
                    write_count = 1;
                    write();
                    tmp = toknext_return;
                    write_count = 4;
                    write();
                }
                if( toknext_tokisnum == 0 ){
                    findsymbol_token = dest;
                    findsymbol_error = 1;
                    findsymbol();
                    tmp = 50049;
                    write_count = 2;
                    write();
                    tmp = findsymbol_return * 4;
                    write_count = 4;
                    write();
                    tmp = 907;
                    write_count = 2;
                    write();
                    tmp = 60289;
                    write();
                    tmp = findsymbol_return * 4;
                    write_count = 4;
                    write();
                }
            }
            if( toknext_return == 4294967286 ){
                toknext();
                findsymbol_token = toknext_return;
                findsymbol_error = 1;
                findsymbol();
                tmp = 50049;
                write_count = 2;
                write();
                tmp = findsymbol_return * 4;
                write_count = 4;
                write();
                tmp = 909;
                write_count = 2;
                write();
                tmp = 60289;
                write();
                tmp = findsymbol_return * 4;
                write_count = 4;
                write();
            }
        }
        if( toknext_return == 4294967288 ){
            toknext();
            compileexpr();
        }
    }
    if( toknext_return == 4288806029 ){
        toknext();
        findsymbol_token = toknext_return;
        findsymbol_error = 1;
        findsymbol();
        tmp = 50049;
        write_count = 2;
        write();
        tmp = findsymbol_return * 4;
        write_count = 4;
        write();
        tmp = 13195;
        write_count = 2;
        write();
        tmp = 60289;
        write();
        tmp = findsymbol_return * 4;
        write_count = 4;
        write();
        tmp = 1675;
        write_count = 2;
        write();
    }
    toknext();
}

int op;
void compileexpr() {
    compileunary();
    op = 0;
    if( toknext_return == 4294967293 ){
        op = 4294967293;
    }
    if( toknext_return == 4294967291 ){
        op = 4294967291;
    }
    if( toknext_return == 4294967295 ){
        op = 4294967295;
    }
    if( toknext_return == 4294967285 ){
        op = 4294967285;
    }
    if( toknext_return == 4294967289 ){
        op = 4294967289;
    }
    if( toknext_return == 4294967286 ){
        op = 4294967286;
    }
    if( toknext_return == 76 ){
        op = 76;
    }
    if( toknext_return == 46 ){
        op = 46;
    }
    if( toknext_return == 132 ){
        op = 132;
    }
    if( toknext_return == 154 ){
        op = 154;
    }
    if( toknext_return == 143 ){
        op = 143;
    }
    if( toknext_return == 4294967159 ){
        op = 4294967159;
    }
    if( toknext_return == 12 ){
        op = 12;
    }
    if( toknext_return == 14 ){
        op = 14;
    }
    if( toknext_return == 133 ){
        op = 133;
    }
    if( toknext_return == 153 ){
        op = 153;
    }

    if( op != 0 ){
        if( toknext_return == 4294967293 ){
            tmp = 51241;
            write_count = 2;
            write();
        }
        if( toknext_return == 4294967291 ){
            tmp = 51201;
            write_count = 2;
            write();
        }
        if( toknext_return == 4294967295 ){
            tmp = 4059550258;
            write_count = 4;
            write();
        }
        if( toknext_return == 4294967285 ){
            tmp = 4059550258;
            write_count = 4;
            write();
            tmp = 53385;
            write_count = 2;
            write();
        }
        if( toknext_return == 4294967289 ){
            tmp = 57847;
            write_count = 2;
            write();
        }
        if( toknext_return == 4294967286 ){
            tmp = 51233;
            write_count = 2;
            write();
        }
        if( toknext_return == 76 ){
            tmp = 51209;
            write_count = 2;
            write();
        }
        if( toknext_return == 46 ){
            tmp = 51249;
            write_count = 2;
            write();
        }
        if( toknext_return == 132 ){
            tmp = 57555;
            write_count = 2;
            write();
        }
        if( toknext_return == 154 ){
            tmp = 59603;
            write_count = 2;
            write();
        }
        if( toknext_return == 143 ){
            tmp = 12109881;
            write_count = 4;
            write();
            tmp = 15;
            write();
            tmp = 49300;
            write_count = 2;
            write();
        }
        if( toknext_return == 4294967159 ){
            tmp = 12109881;
            write_count = 4;
            write();
            tmp = 15;
            write();
            tmp = 49301;
            write_count = 2;
            write();
        }
        if( toknext_return == 12 ){
            tmp = 12109881;
            write_count = 4;
            write();
            tmp = 15;
            write();
            tmp = 49308;
            write_count = 2;
            write();
        }
        if( toknext_return == 14 ){
            tmp = 12109881;
            write_count = 4;
            write();
            tmp = 15;
            write();
            tmp = 49311;
            write_count = 2;
            write();
        }
        if( toknext_return == 133 ){
            tmp = 12109881;
            write_count = 4;
            write();
            tmp = 15;
            write();
            tmp = 49310;
            write_count = 2;
            write();
        }
        if( toknext_return == 153 ){
            tmp = 12109881;
            write_count = 4;
            write();
            tmp = 15;
            write();
            tmp = 49309;
            write_count = 2;
            write();
        }
    }
}

int dest;
void compileassign() {
    if( toknext_return != 4288806029 ){
        dest = toknext_return;
        toknext();
        toknext();
        compileexpr();
        findsymbol_token = dest;
        findsymbol_error = 1;
        findsymbol();
        tmp = 50049;
        write_count = 2;
        write();
        tmp = findsymbol_return * 4;
        write_count = 4;
        write();
        tmp = 905;
        write_count = 2;
        write();
        tmp = 60289;
        write();
        tmp = findsymbol_return * 4;
        write_count = 4;
        write();
    }
    if( toknext_return == 4288806029 ){
        toknext();
        dest = toknext_return;
        toknext();
        toknext();
        compileexpr();
        findsymbol_token = dest;
        findsymbol_error = 1;
        findsymbol();
        tmp = 50049;
        write_count = 2;
        write();
        tmp = findsymbol_return * 4;
        write_count = 4;
        write();
        tmp = 13195;
        write_count = 2;
        write();
        tmp = 60289;
        write();
        tmp = findsymbol_return * 4;
        write_count = 4;
        write();
        tmp = 1673;
        write_count = 2;
        write();
    }
    toknext();
}

//void compilestmt();

int patchfwd_patchloc;
void patchfwd() {
    lseek_offset = 0;
    lseek_whence = 1;
    lseek();
    tmp = lseek_return - patchfwd_patchloc;
    lseek_offset = patchfwd_patchloc - 4;
    lseek_whence = 0;
    lseek();
    write_count = 4;
    write();
    lseek_offset = 0;
    lseek_whence = 2;
    lseek();
}

int patchback_loopstart;
void patchback() {
    tmp = 233;
    write_count = 1;
    write();
    lseek_offset = 0;
    lseek_whence = 1;
    lseek();
    tmp = patchback_loopstart - lseek_return;
    tmp = tmp - 4;
    write_count = 4;
    write();

    patchfwd();
}

int controlflowblock_return;
void controlflowblock() {
    toknext();
    compileexpr();
    tmp = 2215624837;
    write_count = 4;
    write();
    tmp = 0;
    write();
    lseek_offset = 0;
    lseek_whence = 1;
    lseek();
    controlflowblock_return = lseek_return;
    toknext();
    compile_stmt();
}

void compilestmt() {
    while( toknext_return != 77 ){
        if( toknext_tokiscall != 1 ){
            if( toknext_return != 5631 ){
                if( toknext_return != 6232 ){
                    if( toknext_return != 7723522 ){
                        compileassign();
                    }
                    if( toknext_return == 7723522 ){
                        lseek_offset = 0;
                        lseek_whence = 1;
                        lseek();
                        patchback_loopstart = lseek_return;
                        controlflowblock();
                        patchfwd_patchloc = controlflowblock_return;
                        patchback();
                        toknext();
                    }
                }
                if( toknext_return == 6232 ){
                    controlflowblock();
                    patchfwd_patchloc = controlflowblock_return;
                    patchfwd();
                    toknext();
                }
            }
            if( toknext_return == 5631 ){
                toknext();
                tmp = toknext_return;
                write_count = 1;
                write();
                toknext();
                toknext();
            }
        }
        if( toknext_tokiscall == 1 ){
            tmp = 233;
            write_count = 1;
            write();
            findsymbol_token = toknext_return;
            findsymbol_error = 1;
            findsymbol();
            lseek_offset = 0;
            lseek_whence = 1;
            lseek();
            tmp = findsymbol_return - lseek_return;
            tmp = tmp - 4;
            write_count = 4;
            write();
            toknext();
            toknext();
        }
    }
}

int code1;
int code2;
int code3;

int out1;
int out2;

int functionname;
int breakloop;
void main() {
    code1 = 1634545454;
    code2 = 1663987305;
    code3 = 0;

    out1 = 1970220846;
    out2 = 116;

    open_filename = & code1;
    open_flags = 0;
    open_mode = 0;
    open();
    c = open_return;

    open_filename = & out1;
    open_flags = 65;
    open_mode = 493;
    open();
    out = open_return;

    brk_break = 0;
    brk();
    breakstart = brk_return;

    breakcurrent = breakstart + 8;
    brk_break = breakcurrent;
    brk();

    read_fd = c;
    lseek_fd = out;
    write_fd = out;
    
    write_buf = & tmp;
    write_count = 4;

    tmp = 1129071999;
    write();
    tmp = 65793;
    write();
    tmp = 0;
    write();
    write();
    tmp = 3145760;
    write();
    tmp = 1;
    write();
    tmp = 0;
    write();
    tmp = 52;
    write();
    tmp = 0;
    write();
    write();
    tmp = 2097204;
    write();
    tmp = 2;
    write();
    tmp = 0;
    write();

    tmp = 1;
    write();
    tmp = 128;
    write();
    tmp = 134512768;
    write();
    tmp = 0;
    write();
    write();
    write();
    tmp = 5;
    write();
    tmp = 16;
    write();

    tmp = 1;
    write();
    tmp = 0;
    write();
    write();
    write();
    write();
    write();
    tmp = 7;
    write();
    tmp = 16;
    write();

    tmp = 0;
    write();
    write();
    write();
    write();
    write();
    write();
    write();
    write();
    write();
    write();

    while( breakloop == 0 ){
        toknext();
        if( toknext_return != 6388 ){
            toknext();
            functionname = toknext_return;

            symbolamount = symbolamount + 1;
            breakcurrent = breakstart + ( symbolamount * 8 );
            brk_break = breakcurrent;
            brk();
            if( toknext_return == 11 ){
                symboltable = breakstart + ( ( ( symbolamount * 2 ) - 2 ) * 4 );
                *(int*) symboltable = token;
                tmp = 233;
                write_count = 1;
                write();
                tmp = 0;
                write_count = 4;
                write();
                symboltable = symboltable + 4;
                lseek_offset = 0;
                lseek_whence = 1;
                lseek();
                *(int*) symboltable = lseek_return;
            }
            if( toknext_return != 11 ){
                findsymbol_token = toknext_return;
                findsymbol_error = 0;
                findsymbol();
                if( findsymbol_notfound == 0 ){
                    patchfwd_patchloc = findsymbol_return;
                    patchfwd();
                }
                if( findsymbol_notfound == 1 ){
                    symboltable = breakstart + ( ( ( symbolamount * 2 ) - 2 ) * 4 );
                    *(int*) symboltable = toknext_return;
                    symboltable = symboltable + 4;
                    lseek_offset = 0;
                    lseek_whence = 1;
                    lseek();
                    *(int*) symboltable = lseek_return;
                }
            }

            toknext();
            toknext();
            compilestmt();

            tmp = 89;
            write_count = 1;
            write();

            if( functionname = 544362713 ){
                breakloop = 1;
            }
        }
        if( toknext_return == 6388 ){
            toknext();
            symbolamount = symbolamount + 1;
            varamt = varamt + 1;
            breakcurrent = breakstart + ( symbolamount * 8 );
            brk_break = breakcurrent;
            brk();
            symboltable = symboltable + ( ( ( symbolamount * 2 ) - 2 ) * 4 );
            *(int*) symboltable = token;
            symboltable = symboltable + 4;
            *(int*) symboltable = varamt - 1;
            symboltable = breakstart;
        }
    }
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

    exit_errorcode = 0;
    exit();
}