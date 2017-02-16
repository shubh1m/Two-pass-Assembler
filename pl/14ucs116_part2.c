#include<stdio.h>
#include<stdlib.h>

int reg[256], tot = 0, mode[100], type[100], flag, pc = 254;
long long int opcode[30], op[100], operations[100][2];
char bin[100][35];
static FILE *inp;

void find_mode(int,int);
void pre();
void open_file(char);
void input_opcodes();
void find_opcodes();
long long int convert_to_Decimal(char *);
void find_number(int, int, int, int);
int check_opcodes(long long int);

// Initialize dummy values
void pre()
{
    int i = 255;
    while(i >= 0)
        reg[i--] = 0;
    i = 29;
    while(i >= 0)
        opcode[i--] = -1;
    i = 99;
    while(i >= 0)
    {
        type[i] = -1;
        mode[i] = -1;
        operations[i][0] = -1;
        operations[i][1] = -1;
        op[i] = -1;
        i--;
    }
    flag = 0;
}

// Function to open input files
void open_file(char ch)
{
    if(ch == '1')
        inp = fopen("binary1.txt", "r");
    else
        inp = fopen("binary2.txt", "r");
    input_opcodes();
    while(fscanf(inp, "%s", bin[tot]) == 1)
        tot++;
}

// Function to store all opcodes
void input_opcodes()
{
    int i = 0;
    char temp[35];
    char *o = (char *)malloc(35 * sizeof(char));
    freopen("opcode.txt","r",stdin);
    while(scanf("%s %s",temp,o) != EOF)
        opcode[i++] = convert_to_Decimal(o);
}

// Function to convert all opcodes to decimal
long long int convert_to_Decimal(char *s)
{
    long long int x = 0, two = 1;
    int i = 0;
    while(s[i])
        i++;
    i--;
    while(i >= 0)
    {
        x += two * (s[i] - '0');
        two *= 2;
        i--;
    }
    return x;
}

// Function to convert binary form of register/number to its decimal form
void find_number(int i, int j, int k, int limit)
{
    char *s = (char *)malloc(35 * sizeof(char));
    int x = 0;
    while(j < limit)
        s[x++] = bin[i][j++];
    operations[i][k] = convert_to_Decimal(s);
}

// Function to check for opcodes
int check_opcodes(long long int x)
{
    int i;
    for(i = 0;i < 30;i++)
        if(x == opcode[i])
            return 1;
    return 0;
}

// Function to find mode of input
void find_mode(int i,int j)
{
    if(bin[i][j] == '0')
    {
        if(bin[i][j + 1] == '0')
            mode[i] = 0;
        else
            mode[i] = 1;
    }
    else
        mode[i] = 3;
}

// Function to extract opcode, mode and values from binary instruction
void find_opcodes()
{
    int i = 0;
    char *s;
    s = (char *)malloc(35 * sizeof(char));
    for(;i < tot;i++)
    {
        int j = 0;
        while(j < 14)
            s[j] = bin[i][j++];
        s[j] = '\0';
        if(check_opcodes(convert_to_Decimal(s)))
        {
            op[i] = convert_to_Decimal(s);
            find_mode(i,j);
            find_number(i,j + 2, 0, j + 10);
            find_number(i,j + 10, 1, j + 18);
            type[i] = 2;
            continue;
        }
        while(j < 22)
            s[j] = bin[i][j++];
        s[j] = '\0';
        if(check_opcodes(convert_to_Decimal(s)))
        {
            op[i] = convert_to_Decimal(s);
            find_mode(i,j);
            find_number(i,j + 2, 0, j + 10);
            type[i] = 1;
            continue;
        }
        while(j < 30)
            s[j] = bin[i][j++];
        s[j] = '\0';
        op[i] = convert_to_Decimal(s);
        find_mode(i,j);
        type[i] = 0;
    }
}

void check_flag(int i)
{
    if(reg[i] < 0)
        flag = 9;
    else if(reg[i] == 0)
        flag = 12;
    else
        flag = 8;
}

int addition(int x,int y)
{
    while (y != 0)
    {
        int carry = x & y;
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}

int subtraction(int x,int y)
{
    while (y != 0)
    {
        int borrow = (~x) & y;
        x = x ^ y;
        y = borrow << 1;
    }
    return x;
}

int multiplication(int a,int b)
{
    int result = 0;
    while(b != 0)
    {
        if (b & 01)
            result = result + a;
        a<<=1;
        b>>=1;
    }
    return result;
}

void add(int i, int y)
{
    reg[operations[i][0]] = addition(y, reg[operations[i][0]]);
    //reg[operations[i][0]] += y;
    printf("ALU : 000\n");
}

void sub(int i, int y)
{
    //reg[operations[i][0]] -= y;
    reg[operations[i][0]] = subtraction(reg[operations[i][0]], y);
    printf("ALU : 001\n");
}

void mul(int i, int b)
{
    reg[operations[i][0]] = multiplication(reg[operations[i][0]], b);
    //reg[operations[i][0]] *= b;
    printf("ALU : 010\n");
}

void divi(int i, int b)
{
    reg[operations[i][0]] /= b;
    printf("ALU : 011\n");
}

void mod(int i, int b)
{
    reg[operations[i][0]] %= b;
    printf("ALU : 100\n");
}

void cmp(int i, int b)
{
    if(reg[operations[i][0]] == b)
        flag = 12;
    else if(reg[operations[i][0]] < b)
        flag = 9;
    else
        flag = 8;
}

void move(int i, int b)
{
    reg[operations[i][0]] = b;
}

void in(int i)
{
    scanf("%d",&reg[operations[i][0]]);
}

void out(int i)
{
    printf("OUTPUT : %lld\n",operations[i][1]);
}

void inc(int i)
{
    //reg[operations[reg[pc]][0]]++;
    check_flag(operations[reg[pc]][0]);
    reg[pc]++;
    printf("ALU : 101\n");
}

void dec(int i)
{
    reg[operations[reg[pc]][0]] = subtraction(reg[operations[reg[pc]][0]], 1);
    //reg[operations[reg[pc]][0]]--;
    check_flag(operations[reg[pc]][0]);
    reg[pc]++;
    printf("ALU : 110\n");
}

void jmp()
{
    reg[pc] = operations[reg[pc]][0] - 1;
}

void jne()
{
    if(flag != 12)
        reg[pc] = operations[reg[pc]][0] - 1;
    else
        reg[pc]++;
}

void jnz()
{
    if(flag != 12)
        reg[pc] = operations[reg[pc]][0] - 1;
    else
        reg[pc]++;
}

void jeq()
{
    if(flag == 12)
        reg[pc] = operations[reg[pc]][0] - 1;
    else
        reg[pc]++;
}

void jgt()
{
    if(flag == 8)
        reg[pc] = operations[reg[pc]][0] - 1;
    else
        reg[pc]++;
}

void jlt()
{
    if(flag == 9)
        reg[pc] = operations[reg[pc]][0] - 1;
    else
        reg[pc]++;
}

void jge()
{
    if(flag == 8 || flag == 12)
        reg[pc] = operations[reg[pc]][0] - 1;
    else
        reg[pc]++;
}

void jle()
{
    if(flag == 9 || flag == 12)
        reg[pc] = operations[reg[pc]][0] - 1;
    else
        reg[pc]++;
}

void halt()
{
    reg[pc] = tot;
}

void show_statistics()
{
    printf("FLAG VALUE : 00000000000000000000000000001");
    if(flag & 4)
        printf("1");
    else
        printf("0");
    if(flag & 1)
        printf("01\n");
    else
        printf("00\n");
    printf("Register values\n");
    int i;
    for(i = 0;i < 16;i++)
        printf("R%d : %d | ", i ,reg[i]);
    printf("\n\n");
}

void compute()
{
    reg[pc] = 0;
    while(reg[pc] < tot)
    {
        printf("Line number : %d\n", reg[pc] + 1);
        printf("PC : %d\n",(100 + reg[pc] * 4));
        if(type[reg[pc]] == 2)
        {
            int b = operations[reg[pc]][1];
            if(mode[reg[pc]] == 1)
                b = reg[b];
            switch(op[reg[pc]])
            {
                case 0:
                    add(reg[pc], b);
                    break;
                case 1:
                    sub(reg[pc], b);
                    break;
                case 2:
                    mul(reg[pc], b);
                    break;
                case 3:
                    divi(reg[pc], b);
                    break;
                case 4:
                    mod(reg[pc], b);
                    break;
                case 5:
                    cmp(reg[pc], b);
                    break;
                case 6:
                    move(reg[pc], b);
                    break;
                case 9:
                    if(operations[reg[pc]][1] == 252)
                        in(reg[pc]);
                    flag = 8;
                    break;
                case 10:
                    if(mode[reg[pc]] == 0)
                        operations[reg[pc]][1] = reg[b];
                    if(operations[reg[pc]][0] == 253)
                        out(reg[pc]);
                    flag = 8;
                    break;
            }
            if(op[reg[pc]] != 5 && op[reg[pc]] != 9 && op[reg[pc]] != 10)
                check_flag(operations[reg[pc]][0]);
            reg[pc]++;
            show_statistics();
            continue;
        }
        else if(type[reg[pc]] == 0)
        {
            if(op[reg[pc]] == 1073741823)
                halt();
            else
               reg[pc]++;
            show_statistics();
            flag = 8;
            continue;
        }
        switch(op[reg[pc]])
        {
            case 8192:
                inc(reg[pc]);
                check_flag(operations[reg[pc]][0]);
                break;
            case 8193:
                dec(reg[pc]);
                check_flag(operations[reg[pc]][0]);
                break;
            case 8194:
                jmp();
                break;
            case 8195:
                jne();
                break;
            case 8196:
                jnz();
                break;
            case 8197:
                jeq();
                break;
            case 8198:
                jgt();
                break;
            case 8199:
                jlt();
                break;
            case 8200:
                jge();
                break;
            case 8201:
                jle();
                break;
        }
        if(op[reg[pc]] != 8192 && op[reg[pc]] != 8193)
            flag = 8;
        show_statistics();
    }
}

int main()
{
    char tp[20];
    printf("Enter input file (input1.txt OR input2.txt)\n");
    scanf("%s",tp);
    pre();
    open_file(tp[5]);
    input_opcodes();
    find_opcodes();
    fclose(inp);

    freopen(tp,"r",stdin);

    if(tp[5] == '1')
    {
        printf("Output file is generated in output1.txt\n");
        freopen("output1.txt","w",stdout);
    }
    else
    {
        printf("Output file is generated in output2.txt\n");
        freopen("output2.txt","w",stdout);
    }
    compute();
}
