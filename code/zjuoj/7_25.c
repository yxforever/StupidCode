#include<stdio.h>
#include<string.h>
#define ret printf

int main()
{
    char str[20];
    int i,j;
//    char echo(char);
    scanf("%s",str);

    j=strlen(str);
    for(i=0;i<j;i++){
    //    printf("%c\n",str[i]);
    //    echo(str[i]);
        switch (str[i])
    {
    case '-':
        printf("fu");
        break;
    case'1':
        printf("yi");
        break;
    case'2':
        printf("er");
        break;
    case'3':
        printf("san");
        break;
    case'4':
        printf("4");
        break;
    case'5':
        printf("5");
        break;
    case'6':
        printf("liu");
        break;
    case'7':
        printf("qi");
        break;
    case'8':
        printf("ba");
        break;
    case'9':
        printf("jiu");
        break;
    case'0':
        printf("ling");
        break;
    default:
      //  printf("wrong number");
        break;
    }
        if(i<j-1) printf(" ");
    }


return 0;
}
/*
char echo (char number){
    char ret[10];
    switch (number)
    {
    case '-':
        ret="fu";
        break;
    case'1':
        ret="yi";
        break;
    case'2':
        ret="er";
        break;
    case'3':
        ret="san";
        break;
    case'4':
        ret="si";
        break;
    case'5':
        ret="wu";
        break;
    case'6':
        ret="liu";
        break;
    case'7':
        ret="qi";
        break;
    case'8':
        ret="ba";
        break;
    case'9':
        ret="jiu";
        break;
    case'0':
        ret="ling";
        break;
    default:
        ret="wrong number";
        break;
    }

return ret;
}*/