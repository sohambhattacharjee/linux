#include<stdio.h>

int main(){
    int a,b;
	__asm__("mov $0x4FFFFFFF, %eax\n\t");
    __asm__("cpuid\n\t");
	__asm__("mov %%eax, %0\n\t":"=r" (a));
	printf("Response for CPUID with %eax 0x4FFFFFFF is : %d\n",a);
    __asm__("mov $0x4FFFFFFE, %eax\n\t");
    __asm__("cpuid\n\t");
	__asm__("mov %%ebx, %0\n\t":"=r" (a));
    __asm__("mov %%ecx, %0\n\t":"=r" (b));
	printf("Response for CPUID with %eax 0x4FFFFFFE (high bits) is : %d\n",a);
    printf("Response for CPUID with %eax 0x4FFFFFFE (low bits) is : %d\n",b);
}