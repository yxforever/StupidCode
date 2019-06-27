#include <stdio.h>
#include<time.h>

int main()
{
    int is_runnian(int x);
    int howmanydays(int y,int m,int d);
    int year,month,date;    
    int i,j,k,years=0,year_days=0,days1,days2,p;//age_real=year_days+days1-days2
    time_t t;
    struct tm *local;
    t=time(NULL);
    local=localtime(&t);
    printf("今天是%d年%d月%d日\n",local->tm_year+1900,local->tm_mon+1,local->tm_mday);
  //  int year,month,date;
    printf("请输入你的生日（格式：2000年1月1日）：");
    scanf("%d年%d月%d日",&year,&month,&date);

    for (i=year;i<(local->tm_year+1900);i++){
        p=is_runnian(i);
        if(p==1){
            year_days=year_days+366;
        }
        else {
            year_days=year_days+365;
        }
        years++;
    }
    if(month>(local->tm_mon+1)||month==(local->tm_mon+1)&&date>(local->tm_mday))
        years--;

    days2=howmanydays(year,month,date);
    days1=howmanydays(local->tm_year,local->tm_mon+1,local->tm_mday);
    year_days=year_days+days1-days2;

    printf("你今年%d周岁了！!你来到这个世界上已经有%d天\n",years,year_days+1);

    return 0;
}

int is_runnian(int year){
    int flag=0,i;
    if(year%100==0){
       if(year%400==0)
           flag=1;
    }
    else if(year%4==0)
    {
        flag=1;//闰年flag==1,Feb==29;
    }
    return flag;
}

int howmanydays(int year,int month,int date)
{
    int i,flag,days=0;
    int arr1[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int arr2[12]={31,29,31,30,31,30,31,31,30,31,30,31};
        flag=0;
        days=0;
        flag=is_runnian(year);
                 if(flag==1){
                     for(i=0;i<month-1;i++){
                     days=days+arr2[i];
                     }
                 }

                 if(flag==0){
                     for(i=0;i<month-1;i++){
                     days=days+arr1[i];
                     }
                 }

        days=days+date;
            //printf("%d\n",flag);
            //printf("%d\n",days);


return days;

}


