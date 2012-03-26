#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define AUTO 10
#define FULL 100

int main(){
  int fansLevel = AUTO;
  int temp, safeTemp=57, criticalTemp=60,sleepTime=120;
  FILE* tempInput;
/*
  printf("Enter sleep time: ");
  scanf("%d",&sleepTime);
  
  printf("Enter safe temperature: ");
  scanf("%d",&safeTemp);
  
  printf("Enter critical temperature: ");
  scanf("%d",&criticalTemp);
*/
  printf("----------------------------------------\n");
  printf("   Safe: %d, Critical: %d, Sleep: %d\n",safeTemp, criticalTemp,sleepTime);
  printf("----------------------------------------\n");
  while(1){
    tempInput = fopen("/proc/acpi/ibm/thermal","r");
    fscanf(tempInput,"temperatures:	%d",&temp);
    fclose(tempInput);
//    printf("%d\n",temp);
    if(temp>=criticalTemp && fansLevel==AUTO){
      system("date '+.:: %H:%M:%S ::.'");
      printf("Temperature is %d, critical is %d\n",temp,criticalTemp);
      printf("Turning fans to full-speed!\n");
      printf("----------------------------------------\n");
      system("echo level full-speed > /proc/acpi/ibm/fan");
      fansLevel = FULL;
    }
    else if(temp<=safeTemp && fansLevel==FULL){
      system("date '+.:: %H:%M:%S ::.'");
      printf("Temperature is %d, safe is %d\n",temp,safeTemp);
      printf("Turning fans to auto!\n");
      printf("----------------------------------------\n");
      system("echo level auto > /proc/acpi/ibm/fan");
      fansLevel = AUTO;
    }
    sleep(sleepTime);
  }
}
