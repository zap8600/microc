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

int semi;
int ch;
void getch() {
    if( semi == 1 ){
        semi = 0;
        ch = 59;
    }
    if( semi != 1){
        read_buf = &ch;
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
int findsymbol_isub;
int findsymbol_return;
void findsymbol() {
    findsymbol_i = 0;
    findsymbol_isub = 0;
    while( findsymbol_i < symbolamount ){
        symboltable = symboltable + ( i * 8 );
        findsymbol_return = *(int*) symboltable;
        if( findsymbol_return == findsymbol_token ){
            findsymbol_isub = symbolamount - findsymbol_i;
            findsymbol_i = symbolamount - 1;
        }
        symboltable = breakstart;
        findsymbol_i = findsymbol_i + 1;
    }
    findsymbol_i = findsymbol_i - findsymbol_isub;
    if( findsymbol_i >= symbolamount ){
        close_fd = c;
        close();
        close_fd = out;
        close();
        brk_break = breakstart;
        brk();
        exit_errorcode = 1;
        exit();
    }
    symboltable = symboltable + ( ( i * 8 ) + 4 );
    findsymbol_return = *(int*) symboltable;
    symboltable = breakstart;
}

int out;

int tmp;

void compilestmt() {
    while( toknext_return != 77 ){
        if( toknext_tokiscall != 1 ){
            //
        }
        if( toknext_tokiscall == 1 ){
            tmp = 232;
            write_count = 1;
            write();
            findsymbol_token = toknext_return;
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
    symboltable = breakstart;

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
            symboltable = symboltable + ( ( ( symbolamount * 2 ) - 2 ) * 4 );
            *(int*) symboltable = token;
            symboltable = symboltable + 4;
            *(int*) symboltable = varamt - 1;
            symboltable = breakstart;

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